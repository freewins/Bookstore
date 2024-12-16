//
// Created by Freewings on 2024/12/15.
//

#ifndef BOOK_HPP
#define BOOK_HPP
#include <string.h>

#include "file.hpp"


class book {
private:
  //---------Class Data
  int total_book;
  //---------Book Data
  char ISBN[20];
  struct Book{
    char bookName[60];
    char authorName[60];
    char keyWord[60];
    double price;
    int amount;

    Book &operator=(const Book & a) {
      strcpy(bookName,a.bookName);
      strcpy(authorName,a.authorName);
      strcpy(keyWord,a.keyWord);
      price = a.price;
      amount = a.amount;
    }
  };
  Index<Book,20> bookIndex;
public:
  book(std::string index_path_,std::string block_path_):bookIndex(1024,index_path_,block_path_) {
    bookIndex.Initialise();
    total_book = bookIndex.getInfo(1);

  }
  ~book() {
    bookIndex.updateInfo(1,total_book);
  }
  bool buy();
  bool selete();
  bool modify();
  bool import();
  bool show_book();
};



#endif //BOOK_HPP
