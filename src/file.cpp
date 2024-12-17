//
// Created by Freewings on 2024/12/6.
//
#include "file.hpp"
#ifndef MEMORYSEA_HPP
#define MEMORYSEA_HPP
#include<cmath>
#include <cstring>
#include<fstream>
#include<vector>
#include<algorithm>

//把索引中的文件读到内存里
template<typename T, int len,int info>
void Index<T, len,info>::Initialise() {
  Block *temp = new Block;
  temp->cur = -1;
  index_file.open(Indexnmame, std::ios::in | std::ios::binary);
  index_file.seekp(0, std::ios::end);
  bool noBlock = false;
  if (index_file.tellg() == 0) {//文件刚刚创建 需要写info
    noBlock = true;
    int temp = 0;
    for(int i = 0;i<info;i++) {
      block_file.write(reinterpret_cast<char *>(&temp),sizeof(int));
      info_data[i] = temp;
    }
  } else {
    //开头部分放 Blockcount
    index_file.seekg(0, std::ios::beg);
    index_file.read(reinterpret_cast<char *>(&Blockcount), sizeof(int));
    for(int i = 1;i<info;i++) {
      index_file.read(reinterpret_cast<char *>(&info_data[i]),sizeof(int));
    }
    if (Blockcount == 0) {
      noBlock = true;
    }
  }
  if (noBlock) {
    Blockcount = 1;
    head = temp;
    tail = temp;
    head->cur = 0;
    head->next = nullptr;
    head->now_size = 0;
    head->min[0] = '\0';
    head->max[0] = '\0';
  } else {
    index_file.seekg(offset, std::ios::beg);
    index_file.read(reinterpret_cast<char *>(temp), sizeof(Block));
    head = temp;
    tail = temp;
    head->next = nullptr;
    Block *cur = head;
    for (int i = 1; i < Blockcount; i++) {
      Block *newBlock = new Block;
      index_file.read(reinterpret_cast<char *>(newBlock), sizeof(Block));
      cur->next = newBlock;
      cur = newBlock;
      if (tail->cur < cur->cur) {
        tail = cur;
      }
    }
    //最后的cur 的next为空指针
    cur->next = nullptr;
  }
  index_file.close();
  block_file.open(Blockname, std::ios::out | std::ios::in | std::ios::binary);
}

//进行合并
template<typename T, int len,int info>
void Index<T, len,info>::Merge(Block *cur, Block *forcur) {
  //采用递归写法 进行一次次连续调用
  //总体来说就是都读入到内存里面然后进行放置
  //可能为头指针或者是在末尾
  if (cur == forcur) {
    return;
  }
  //优先合并前部指针 之后再进行后面的指针
  if (cur == nullptr || forcur == nullptr)
    return;
  if (cur->now_size > sqrBlocksize) {
    return;
  }
  //进行合并
  if (cur->now_size + forcur->now_size < 2 * sqrBlocksize + 4) {
    Data *pools = new Data[3 * sqrBlocksize];
    block_file.seekg(cur->cur, std::ios::beg);
    block_file.read(reinterpret_cast<char *>(pools), sizeof(Data) * cur->now_size);
    // for (int i = 0; i < cur->now_size; i++) {
    //   block_file.read(reinterpret_cast<char *>(&pools[i]), sizeof(Data));
    // }
    //定位到块的末尾
    block_file.seekp(forcur->cur + sizeof(Data) * forcur->now_size, std::ios::beg);
    block_file.write(reinterpret_cast<char *>(pools), sizeof(Data) * cur->now_size);
    // for (int i = 0; i < cur->now_size; i++) {
    //   block_file.write(reinterpret_cast<char *>(&pools[i]), sizeof(Data));
    // }
    //修改最大值和最小值
    if (strcmp(forcur->max, cur->max) < 0) {
      strcpy(forcur->max, cur->max);
    }
    if (strcmp(cur->min, forcur->min) < 0) {
      strcpy(forcur->min, cur->min);
    }
    forcur->now_size += cur->now_size;
    //删除对应的块
    if (tail == cur) {
      //说明cur 对应了指针最大块，需要找下一个位置。
      tail = forcur;
      Block *p = head;
      while (p != nullptr) {
        if (p != cur) {
          if (p->cur > tail->cur) {
            tail = p;
          }
        }
        p = p->next;
      }
    }
    forcur->next = cur->next;
    delete cur;
    delete[] pools;
    Blockcount--;
    ///合并后从forcur 继续查
    if (forcur->now_size >= 2 * sqrBlocksize) {
      Split(forcur);
    } else {
      Merge(forcur->next, forcur);
    }
    return;
  } else {
    //向前无法合并可以向后合并
    Merge(cur->next, cur);
    return;
  }
}

//进行裂块
template<typename T, int len,int info >
void Index<T, len,info>::Split(Block *cur) {
  block_file.seekg(cur->cur, std::ios::beg);
  Data *temp_arr = new Data[cur->now_size + 4];
  Block *new_block = new Block; //创建新块
  int i = cur->now_size / 2;
  int j;

  block_file.read(reinterpret_cast<char *>(temp_arr), sizeof(Data) * cur->now_size);
  //std::sort(temp_arr, temp_arr + cur->now_size);

  //在当前位置写入前 1/2 的数据
  block_file.seekp(cur->cur, std::ios::beg);
  block_file.write(reinterpret_cast<char *>(temp_arr), sizeof(Data) * i);
  strcpy(cur->max, temp_arr[i - 1]);
  strcpy(cur->min, temp_arr[0]);
  //在文件末尾写入
  block_file.seekp(tail->cur + sizeof(Data) * (2 * sqrBlocksize + 16), std::ios::beg);
  new_block->cur = Index::block_file.tellp();

  //改变最大值最小值
  strcpy(new_block->min, temp_arr[i].key);
  strcpy(new_block->max, temp_arr[cur->now_size - 1].key);
  //存储
  j = cur->now_size - i;
  block_file.write(reinterpret_cast<char *>(&temp_arr[i]), sizeof(Data) * j);
  //更新size
  new_block->now_size = j;
  cur->now_size = i;
  //更新链状块的关系
  new_block->next = cur->next;
  tail = new_block;
  cur->next = new_block;
  Blockcount++;
  delete [] temp_arr;
}

//插入数据
//TODO

template<typename T, int len,int info>
bool Index<T, len,info>::insertData(const char *key, T &value) {
  Block *cur = nullptr;
  Block *forcur = nullptr;
  Block *p = head;
  Data temp;
  //把值放入temp中
  std::strcpy(temp.key, key);
  //TODO 注意每一个T要写赋值函数
  temp.value = value;

  while (p != nullptr) {
    //从最大值处插入
    if (strcmp(key, p->max) <= 0) {
      cur = p;
      break;
    }
    forcur = p;
    p = p->next;
  }

  if (cur == nullptr) {
    // 说明当前值为最大值
    cur = forcur;
    strcpy(cur->max, key); //修改最大值
  }

  //说明数据在某一个块中小于其最大值
  Data *pools = new Data[cur->now_size + 3];

  block_file.seekp(cur->cur, std::ios::beg);
  //读入
  if (cur->now_size) {
    block_file.read(reinterpret_cast<char *>(pools), sizeof(Data) * cur->now_size);
    //看是否有重复值
    auto t = std::lower_bound(pools, pools + cur->now_size, temp);
    if (strcmp(t->key, key) == 0) {
      delete [] pools;
      return false; //重复插入 错误
    }
  }
  //
  pools[cur->now_size++] = temp;
  std::sort(pools, pools + cur->now_size);
  block_file.seekp(cur->cur, std::ios::beg);
  block_file.write(reinterpret_cast<char *>(pools), sizeof(Data) * cur->now_size);
  if (strcmp(key, cur->min)) {
    strcpy(cur->min, key);
  }
  //TODO 可以直接把pools 的指针传过去，避免过多内存消耗
  if (cur->now_size >= 2 * sqrBlocksize) {
    Split(cur);
  }
  delete [] pools;
}

//寻找数据
//TODO Read all and watch
///
/// @param key 查找时的唯一键值
/// @param check 检测是否找到
/// @param pos 返回找到对象的文件位置，仅为了在update的时候调用
/// @return
template<typename T, int len,int info>
T Index<T, len, info>::findData(const char *key, bool &check,int & pos) {
  Block *cur = head;
  Data *pools = new Data[sqrBlocksize * 3];
  T temp;
  int for_pos;
  check = false;
  //存储数据 把所有相同的data数据存入其中
  while (cur != nullptr) {
    //循环中执行 可能有好几个块中存放了相同的数据
    //TODO 采用全部读入，二分比较的方法
    if (strcmp(cur->min.key, key) <= 0 && strcmp(key, cur->max.key) <= 0) {
      block_file.seekg(cur->cur, std::ios::beg);
      //key值在当前块的范围
      block_file.read(reinterpret_cast<char *>(pools), sizeof(Data) * cur->now_size);
      for (int i = 0; i < cur->now_size; i++) {
        if (strcmp(pools[i].key, key) == 0) {
          temp = pools[i].value;
          check = true;
          pos = cur ->cur + i * sizeof(Data);
          break;
        }
      }
    }
    cur = cur->next;
  }
  delete[] pools;
  return temp;
}
//TODO 有没有必要
/// 重载函数 不需要返回pos
/// @param key
/// @param check
/// @return
template<typename T, int len,int info>
T Index<T, len, info>::findData(const char *key, bool &check) {
  Block *cur = head;
  Data *pools = new Data[sqrBlocksize * 3];
  T temp;
  check = false;
  //存储数据 把所有相同的data数据存入其中
  while (cur != nullptr) {
    //循环中执行 可能有好几个块中存放了相同的数据
    //TODO 采用全部读入，二分比较的方法
    if (strcmp(cur->min.key, key) <= 0 && strcmp(key, cur->max.key) <= 0) {
      block_file.seekg(cur->cur, std::ios::beg);
      //key值在当前块的范围
      block_file.read(reinterpret_cast<char *>(pools), sizeof(Data) * cur->now_size);
      for (int i = 0; i < cur->now_size; i++) {
        if (strcmp(pools[i].key, key) == 0) {
          temp = pools[i].value;
          check = true;
          break;
        }
      }
    }
    cur = cur->next;
  }
  delete[] pools;
  return temp;
}

template<typename T, int len,int info>
bool Index<T,len , info>::updateData(const char * key,T & value,int pos ) {
  block_file.seekp(pos,std::ios::beg);
  block_file.write(reinterpret_cast<char*>(&value));
  return true;
}

//删除数据
template<typename T, int len,int info>
bool Index<T, len,info>::deleteData(const char *key, T & value) {
  Block *cur = head;
  Block *forcur = head;
  Data *pool = new Data[3 * sqrBlocksize];
  Data temp;
  temp.value = value;
  strcpy(temp.key, key);
  bool exist = false;
  while (cur != nullptr) {
    //循环中执行 可能有好几个块中存放了相同的数据
    if (strcmp(cur->min, key) <= 0 && strcmp(key, cur->max) <= 0) {
      block_file.seekg(cur->cur, std::ios::beg);
      //key值在当前块的范围
      block_file.read(reinterpret_cast<char *>(pool), sizeof(Data) * cur->now_size);
      auto t = std::lower_bound(pool, pool + cur->now_size, temp);
      if (*t == temp) {
        exist = true;
      }
      //删除的尾部
      //说明存在要删除的对象 不会出现相同的value
      if (exist) {
        int startSize = t - pool;
        if (cur->now_size == 2) {
          cur->now_size = 1;
          if (!startSize) {
            strcpy(cur->max, pool[0].key);
          }
          cur->min.key[0] = '\0';
        } else if (cur->now_size == 1) {
          cur->max.key[0] = '\0';
          cur->now_size = 0;
        } else {
          if (startSize == 0) {
            strcpy(cur->min, pool[1].key);
          } else if (startSize == cur->now_size - 1) {
            strcpy(cur->max, pool[cur->now_size - 2].key);
          }
          block_file.seekp(cur->cur, std::ios::beg);
          block_file.write(reinterpret_cast<char *>(pool), sizeof(Data) * startSize);
          block_file.write(reinterpret_cast<char *>(&pool[startSize + 1]),
                           sizeof(Data) * (cur->now_size - startSize - 1));
          cur->now_size--;
          //Merge需要传入当前和上一个指针
          //TODO 会不会出现相同的key 和value 的情况
          Merge(cur, forcur);
        }
        break;
      }
    } else if (strcmp(temp.key,  cur->min) <0 ) {
      break;
    }
    forcur = cur; //存储一下上一个位置的指针
    cur = cur->next;
  }
  delete[] pool;
  return exist;
}

#endif //MEMORYSEA_HPP
