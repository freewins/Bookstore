//
// Created by Freewings on 2024/12/15.
//
#ifndef USER_CPP
#define USER_CPP

#include "user.hpp"


bool user::login(const char *userId, int &privilege, const char *password) {
  bool is_find = false;
  User tmp = userIndex.findData(userId, is_find);
  if (is_find) {
    if (password != nullptr) {
      if (strcmp(password, tmp.password) == 0) {
        //TODO 需不需要密码的特判
        privilege = tmp.privilege;
        return true;
      } else {
        return false;
      }
    } else {
      if(privilege > tmp.privilege) {
        privilege = tmp.privilege;
        return  true;
      }
      if(tmp.password[0] !='\0') {
        return false;
      }
      privilege = tmp.privilege;
      return true;
    }
  } else return false;
}

bool user::logout() {
  return true;
}

bool user::daleteUser(const char *userId) {
  bool is_find = false;
  User tmp;
  if( userIndex.deleteData(userId)) {
    total_user--;
    return true;
  }
  else return false;

}

bool user::addUser(const char *userId, const char *userName, int privilege, const char *password) {
  User tmp( password, userName,privilege);
  if( userIndex.insertData(userId, tmp)) {
    total_user++;
    return true;
  }
  else return false;
}

bool user::modifyPasswd(const char *userId, const char *newPassword, const char *currentPassword) {
  bool is_find;
  int pos;
  User tmp = userIndex.findData(userId, is_find, pos);
  if (is_find) {
    if(currentPassword == nullptr) {
      strcpy(tmp.password, newPassword);
      userIndex.updateData(tmp, pos);
      return true;
    }
    else if (strcmp(tmp.password, currentPassword) == 0) {
      strcpy(tmp.password, newPassword);
      userIndex.updateData(tmp, pos);
      return true;
    } else {
      return false;
    }
  }
  return false;
}


#endif