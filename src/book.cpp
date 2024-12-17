//
// Created by Freewings on 2024/12/15.
//

#include "book.hpp"
#include<iostream>
#include<iomanip>
#include <vector>

#include "file.hpp"
//mode = 1  ISBN | mode 2 Name | mode 3 author | mode 4 keyword |

bool book::show_book(char *find_key, int mode) {
  //mode = 1  ISBN | mode 2 Name | mode 3 author | mode 4 keyword |
  bookIndex.showAll(find_key,mode);
}

bool book::buy(char *ISBN, int &quantity, double &total) {
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

bool book::selete(char *ISBN, int &pos) {
  bool find;
  Book tmp = bookIndex.findData(ISBN, find, pos);
  if (!find) {
    bookIndex.insertData(ISBN, tmp);

    bookIndex.findData(ISBN, find, pos);
  }
  return true;
}

bool book::modify(Book &tmp, char * ISBN ,char *new_data, int mode, int &pos) {
  //
  switch (mode) {
    case 1: { // ISBN 注意排序的修改
      if(strcmp(ISBN,new_data) == 0) {
        return false;
      }
      bookIndex.deleteData(ISBN,tmp);
      bookIndex.insertData(new_data,tmp);
      bool is;
      //TODO 能不能写一个insert函数直接返回对应 pos 的
      bookIndex.findData(new_data,is ,pos);
      break;
    }
    case 2: {
      strcpy(tmp.bookName,new_data);
      bookIndex.updateData(tmp,pos);
      break;
    }
    case 3: {
      strcpy(tmp.authorName,new_data);
      bookIndex.updateData(tmp,pos);
      break;
    }
    case 4: {
      if(tmp.findKey(new_data)) {
        return  false;
      }else {
        tmp.insertKey(new_data);
        bookIndex.updateData(tmp,pos);
        return true;
      }
      break;
    }
  }
  return true;
}

bool book::modify(Book &tmp, double & price, int &pos) {
  if(price < 0 )
    return false;
  tmp.price = price;
  return bookIndex.updateData(tmp,pos);
}

bool book::import(Book &tmp, int quantity, double total_cost, int &pos) {
  if(quantity < 0 || total_cost < 0) {
    return false;
  }
  tmp.amount += quantity;
  return bookIndex.updateData(tmp,pos);
}


template<>
void Index<book::Book, 20>::showAll(char * find_key, int mode) {
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
    if(mode == 1) {//ISBN 找到一个就弹出
      for (int i = 0; i < cur->now_size; i++) {
        if(strcmp(find_key,pools[i].key) == 0) {
          is_find = true;
          Datas.push_back(pools[i]);
          break;
        }
      }
    }
    else if(mode == 2){//bookname
      for (int i = 0; i < cur->now_size; i++) {
        if(strcmp(find_key,pools[i].value.bookName) == 0) {
          is_find = true;
          Datas.push_back(pools[i]);
        }
      }
    }else if(mode == 3) {//authorname
      for (int i = 0; i < cur->now_size; i++) {
        if(strcmp(find_key,pools[i].value.authorName) == 0) {
          is_find = true;
          Datas.push_back(pools[i]);
        }
      }
    }else if(mode == 4){
      for (int i = 0; i < cur->now_size; i++) {
        if(pools[i].value.findKey(find_key)) {
          is_find = true;
          Datas.push_back(pools[i]);
        }
      }
    }
    cur = cur->next;
  }
  if(!is_find) {
    std::cout<<"\n";
  }
  else {
    for(auto t = Datas.begin();t!= Datas.end();++t) {
      printf("%s\t%s\t%s\t%s\t%.2lf\t%d\n",
        t->key,t->value.bookName,t->value.authorName,
        t->value.keyWord,t->value.price,t->value.amount);
    }
  }
  delete[] pools;
}

