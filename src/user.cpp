//
// Created by Freewings on 2024/12/15.
//

#include "user.hpp"


bool user::login(char * userId, char *password) {
  bool is_find = false;
  User tmp = userIndex.findData(userId,is_find);
  if(is_find) {
    if(password != "") {
      if(strcmp(password,tmp.password) == 0) {
        //TODO 需不需要密码的特判
        return true;
      }
      else return false;
    }
  }
  else return false;
}
bool user::logout() {
  return true;
}
bool user::daleteUser(char *userId) {
  bool is_find = false;
  User tmp;
  return userIndex.deleteData(userId,tmp);
}
bool user::addUser(char *userId, char *userName, int privilege, char *password) {
  User tmp(userName,password,privilege);
  return userIndex.insertData(userId,tmp);
}
bool user::modifyPasswd(char * userId, char *newPassword, char *currentPassword) {
  bool is_find;
  int pos;
  User tmp = userIndex.findData(userId,is_find,pos);
  if(is_find) {
    if(strcpy(tmp.password,currentPassword) == 0) {
      strcpy(tmp.password,newPassword);
      userIndex.updateData(userId,tmp,pos);
      return true;
    }
    else {
      return false;
    }
  }
  return false;

}







