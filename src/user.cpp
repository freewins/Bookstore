//
// Created by Freewings on 2024/12/15.
//

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
      } else return false;
    }
    else {
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
  return userIndex.deleteData(userId, tmp);
}

bool user::addUser(const char *userId, const char *userName, int privilege, const char *password) {
  User tmp(userName, password, privilege);
  return userIndex.insertData(userId, tmp);
}

bool user::modifyPasswd(const char *userId, const char *newPassword, const char *currentPassword) {
  bool is_find;
  int pos;
  User tmp = userIndex.findData(userId, is_find, pos);
  if (is_find) {
    if (strcpy(tmp.password, currentPassword) == 0) {
      strcpy(tmp.password, newPassword);
      userIndex.updateData(tmp, pos);
      return true;
    } else {
      return false;
    }
  }
  return false;
}
