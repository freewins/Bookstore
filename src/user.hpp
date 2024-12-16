//
// Created by Freewings on 2024/12/15.
//

#ifndef USER_HPP
#define USER_HPP
#include "file.hpp"

class user {
private:
  int total_user = 0;
  char userId[32];
  struct User {
    char password[32];
    char username[32];
    int root;
    User &operator=(const User &a) {
      strcmp(password, a.password);
      strcmp(username, a.username);
      root = a.root;
    }
  };
  Index<User,32> userIndex;

public:

  user(std::string index_path_,std::string block_path_):
  userIndex(1024,index_path_,block_path_)
  {
    userIndex.Initialise();//首先初始化文件
    total_user = userIndex.getInfo(1);//数量统一放在文件的第一个位置处
    if(total_user == 0) {
      //第一次启动程序，需要创建超级用户
    }
  }
  ~user() {
    userIndex.updateInfo(1,total_user);
  }

  bool login();

  bool logout();

  bool modifyPasswd();

  bool addUser();

  bool daleteUser();
};




#endif //USER_HPP
