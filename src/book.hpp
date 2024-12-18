//
// Created by Freewings on 2024/12/15.
//

#ifndef BOOK_HPP
#define BOOK_HPP
#include <string.h>

#include "file.hpp"


class book {
  //Info 1: 书籍总数
  //Info 0: 不可访问 Blockcount
private:
  //---------Class Data
  int total_book;
  //---------Book Data
  char ISBN[20];

public:
  struct Book{

    char bookName[62];
    char authorName[62];
    char keyWord[70];
    double price;
    int amount;

    Book () {
      bookName[0] = '\0';
      authorName[0] = '\0';
      keyWord[0] = '\0';
      price = 0;
      amount = 0;
    }

    Book &operator=(const Book & a) {
      strcpy(bookName,a.bookName);
      strcpy(authorName,a.authorName);
      strcpy(keyWord,a.keyWord);
      price = a.price;
      amount = a.amount;
    }
    bool findKey(const char * des) {
      char tmp[61];
      for(int i = 0,j = 0;i<strlen(keyWord);i++) {
        int k = 0;
        while(keyWord[j] !='|' && keyWord[j] != '\0') {
          tmp[k++] = keyWord[j++];
        }
        tmp[k] = '\0';
        if(strcmp(tmp,des) == 0) {
          return true;
        }
      }
      return false;
    }
    void insertKey(const char * des) {
      strcat(keyWord,des);
      int j = strlen(keyWord);
      keyWord[j] = '1';
      keyWord[j+1] = '\0';
    }
  };

  Index<Book,20> bookIndex;
  book(std::string index_path_,std::string block_path_):bookIndex(1024,index_path_,block_path_) {
    bookIndex.Initialise();
    total_book = bookIndex.getInfo(1);
  }
  ~book() {
    bookIndex.updateInfo(1,total_book);
  }
  bool buy(const char * ISBN ,int & quantity , double & total);
  bool show_book(const char * find_key,int mode = 0);

  bool selete(Book & tmp,const char * ISBN,int  & pos);
  bool modify(Book &,double & price,int & pos);
  bool modify(Book &,const char * ISBN,const char * new_data,int mode = 0,int & pos);
  bool import(Book &,int quantity,double total_cost,int & pos);
};



#endif //BOOK_HPP
