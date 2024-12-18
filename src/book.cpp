//
// Created by Freewings on 2024/12/15.
//
#ifndef BOOK_CPP
#define BOOK_CPP
#include<iostream>
#include<iomanip>
#include <vector>
#include "book.hpp"

//mode = 1  ISBN | mode 2 Name | mode 3 author | mode 4 keyword |

template<>
void Index<book::Book, 20>::showAll(const char *find_key, int mode) {
  std::vector<Data> Datas;
  Data *pools = new Data[sqrBlocksize * 3];
  auto cur = head;
  bool is_find = false;
  //存储数据 把所有相同的data数据存入其中
  while (cur != nullptr) {
    //循环中执行 可能有好几个块中存放了相同的数据
    //TODO 采用全部读入，二分比较的方法
    block_file.seekg(cur->cur, std::ios::beg);
    //key值在当前块的范围
    block_file.read(reinterpret_cast<char *>(pools), sizeof(Data) * cur->now_size);
    if (mode == 0) {
      //没有必要把全部数据放到一个数组里面
      for (int i = 0; i < cur->now_size; i++) {
        is_find = true;
        printf("%s\t%s\t%s\t%s\t%.2lf\t%d\n",
               pools[i].key, pools[i].value.bookName, pools[i].value.authorName,
               pools[i].value.keyWord, pools[i].value.price, pools[i].value.amount);
      }
    }
    if (mode == 1) {
      //ISBN 找到一个就弹出
      for (int i = 0; i < cur->now_size; i++) {
        if (strcmp(find_key, pools[i].key) == 0) {
          is_find = true;
          Datas.push_back(pools[i]);
          break;
        }
      }
    } else if (mode == 2) {
      //bookname
      for (int i = 0; i < cur->now_size; i++) {
        if (strcmp(find_key, pools[i].value.bookName) == 0) {
          is_find = true;
          Datas.push_back(pools[i]);
        }
      }
    } else if (mode == 3) {
      //authorname
      for (int i = 0; i < cur->now_size; i++) {
        if (strcmp(find_key, pools[i].value.authorName) == 0) {
          is_find = true;
          Datas.push_back(pools[i]);
        }
      }
    } else if (mode == 4) {
      for (int i = 0; i < cur->now_size; i++) {
        if (pools[i].value.findKey(find_key)) {
          is_find = true;
          Datas.push_back(pools[i]);
        }
      }
    }
    cur = cur->next;
  }

  if (!is_find) {
    std::cout << "\n";
  }
  if (mode != 0) {
    for (auto t = Datas.begin(); t != Datas.end(); ++t) {
      printf("%s\t%s\t%s\t%s\t%.2lf\t%d\n",
             t->key, t->value.bookName, t->value.authorName,
             t->value.keyWord, t->value.price, t->value.amount);
    }
  }

  delete[] pools;
}

bool book::show_book(const char *find_key, int mode) {
  //mode = 1  ISBN | mode 2 Name | mode 3 author | mode 4 keyword |
  bookIndex.showAll(find_key, mode);
  return true;
}

bool book::buy(const char *ISBN, int &quantity, double &total) {
  if (quantity < 0) {
    return false;
  }
  int pos;
  bool is_find;
  Book tmp = bookIndex.findData(ISBN, is_find, pos);
  if (!is_find) {
    return false;
  }
  if (quantity > tmp.amount) {
    return false;
  } else {
    tmp.amount -= quantity;
    bookIndex.updateData(tmp, pos);
    total = tmp.price * quantity;
    return true;
  }
}

bool book::selete(Book &tmp, const char *ISBN, int &pos) {
  bool find;
  tmp = bookIndex.findData(ISBN, find, pos);
  if (!find) {
    bookIndex.insertData(ISBN, tmp);

    tmp = bookIndex.findData(ISBN, find, pos);
  }
  return true;
}

bool book::modify(Book &tmp, const char *ISBN, const char *new_data, int &pos, int mode = 0) {
  //
  switch (mode) {
    case 1: {
      // ISBN 注意排序的修改
      if (strcmp(ISBN, new_data) == 0) {
        return false;
      }

      bool is;
      bookIndex.findData(new_data, is);
      if (is) {
        //有重复的ISBN
        return false;
      }
      bookIndex.deleteData(ISBN, tmp);
      bookIndex.insertData(new_data, tmp);
      //TODO 能不能写一个insert函数直接返回对应 pos 的
      bookIndex.findData(new_data, is, pos);
      break;
    }
    case 2: {
      strcpy(tmp.bookName, new_data);
      bookIndex.updateData(tmp, pos);
      break;
    }
    case 3: {
      strcpy(tmp.authorName, new_data);
      bookIndex.updateData(tmp, pos);
      break;
    }
    case 4: {
      return tmp.insertKey(new_data);
      break;
    }
  }
  return true;
}

bool book::modify(Book &tmp, double &price, int &pos) {
  if (price < 0)
    return false;
  tmp.price = price;
  return bookIndex.updateData(tmp, pos);
}

bool book::import(Book &tmp, int quantity, double total_cost, int &pos) {
  if (quantity < 0 || total_cost < 0) {
    return false;
  }
  tmp.amount += quantity;
  return bookIndex.updateData(tmp, pos);
}

#endif
