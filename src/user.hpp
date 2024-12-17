//
// Created by Freewings on 2024/12/15.
//

#ifndef USER_HPP
#define USER_HPP
#include "file.hpp"

class user {
private:
  //Info 1: 写入总数
  //Info 0:不可读 是Blockcount
  int total_user = 0;
  char userId[32];
  struct User {
    char password[32];
    char username[32];
    int privilege;
    User &operator=(const User &a) {
      strcmp(password, a.password);
      strcmp(username, a.username);
      privilege = a.privilege;
    }
    User() {
      password[0]= '\0';
      username[0] = '\0';
      privilege = 0;
    }
    User (char * password_,char * username_ ,int privilege_) {
      strcpy(password,password_);
      strcpy(username,username_);
      privilege = privilege_;
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
      addUser("root","root",7,"sjtu");
    }
  }
  ~user() {
    userIndex.updateInfo(1,total_user);
  }

  ///
  /// @param password 可以为空
  /// @return 登陆成功 or 失败
  bool login(const char * , int & privilege,const char * password = nullptr);

  ///退出登录
  ///这个感觉在user里面没有什么用，先当一个空函数用吧 或者在main()里面实现
  bool logout();

  /// 权限 数量检测在外部函数进行
  /// @param newPassword 可以为空 nullptr 要特判
  /// @param currentPassword  可以传入
  /// @return 操作成功与失败
  bool modifyPasswd(const char * userId,const char * newPassword,const char * currentPassword = nullptr);

  /// 权限 参数数据量在外部函数进行
  /// @param userId 传入用户ID 重复性检查在本函数进行
  /// @param userName 传入userName 无需检查
  /// @param privilege 传入privilege 合法性检查在外部函数进行
  /// @param password 传入password 有无在外部函数进行
  /// @return 操作成功或失败
  bool addUser(const char * userId , const char * userName,int privilege,const char * password= nullptr );

  /// 权限检测在外部进行
  /// @param userId
  /// @return 删除成功与否
  bool daleteUser(const char * userId );
};




#endif //USER_HPP
