//
// Created by Freewings on 2024/12/15.
//

#ifndef LOGS_HPP
#define LOGS_HPP
#include<iostream>
#include "file.cpp"

class logs {
public:
  void save();

  void read();

  void print();
};

class Profit : public logs {
private:
  //Info
  // Info 1: 写入指针位置 Info 2: 写入总数
  int count; //记录交易次数
  int now_cur; //当前操作的数据
  LogFile<double, 2> logProgift;

public:
  Profit(std::string profit_path_);
  ~Profit();
  void save(int n, double profit);

  bool read(int k = -1);
};


class Operator : public logs {
private:
  //Info
  // Info 1: 写入指针位置 Info 2: 写入总数
  char name;
  int count; //当前总数据量
  int now_cur; //当前读写的位置
  struct OpLog {
    char userId[32];
    char userName[32];
    int op;

    OpLog(char *userId_, char *userName_, int op_) {
      op = op_;
      strcpy(userId, userId_);
      strcpy(userName, userName_);
    }
  };
  //以数字代替操作 用数字来表示对应的操作
  std::string Op[6] = {
    "", "BUY", "MODIFY", "IMPORT", "CREAT_USER", "MODIFY_USER"
  };
  LogFile<OpLog> logOperator;

public:
  Operator(std::string operator_path_);
  ~Operator();
  void save(char *userId_, char *userName_, int op);

  //TODO 目前只写入不进行读入
  void read() {

  }
};

//TODO system log 有待完善
class SystemLog : public logs {
private:
  struct SysLog {
    long long time;
    char statement[60];
  };


  int count;
  int now_cur;
  LogFile<SysLog> logSystem;
public:
  SystemLog(std::string syslog_path_);
  ~SystemLog();
  void save();

  void read();
};


#endif //LOGS_HPP
