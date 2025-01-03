//
// Created by Freewings on 2024/12/15.
//

#ifndef LOGS_HPP
#define LOGS_HPP
#include<iostream>
#include<ctime>
#include "file.cpp"

class logs {
public:
  void save();

  void read();
};

//TODO: 把财务报表加入到其中
class Profit : public logs {
private:
  //Info
  // Info 1: 写入指针位置 Info 2: 写入总数
  int count; //记录交易次数
  int now_cur; //当前操作的数据

  //enum OP {IMPORT = 1,BUY = 2};
  struct financeLog {
    double profit;
    int op_;
  };

  LogFile<financeLog, 2> logProgift;

public:
  Profit(std::string profit_path_);

  ~Profit();

  void save(double profit);

  bool read(int k = -1);

  void print();
};


class Operator : public logs {
private:
  //Info
  // Info 1: 写入指针位置 Info 2: 写入总数
  int count; //当前总数据量
  int now_cur; //当前读写的位置

  struct OpLog {
    char userId[32];
    std::tm opTime;
    int op_;

    OpLog() {
      userId[0] = '\0';
      op_ = -1;
    }

    OpLog(const char *userId_, int op__) {
      strcpy(userId, userId_);
      std::time_t now = std::time(nullptr);
      opTime = *std::localtime(&now);
      this->op_ = op__;
    }
  };

  //以数字代替操作 用数字来表示对应的操作
  LogFile<OpLog> logOperator;

public:
  Operator(std::string operator_path_);

  ~Operator();

  void save(const char *userId_, int op);

  //TODO 目前只写入不进行读入
  void read(int k = -1);
};


class SystemLog : public logs {
private:
  struct SysLog {
    std::tm opTime;
    int priviledge;
    char userId[32];
    char op[18];

    SysLog() {
      priviledge = 0;
      userId[0] = '\0';
      op[0] = '\0';
    }

    SysLog(const char *userId_, int priviledege_, int op_) {
      if (userId_ == nullptr) {
        userId[0] = '\0';
      } else {
        strcpy(userId, userId_);
      }
      priviledge = priviledege_;
      strcpy(op, opEnum[op_].c_str());
      std::time_t now = std::time(nullptr);
      opTime = *std::localtime(&now);
    }
  };

  int count;
  int now_cur;
  LogFile<SysLog> logSystem;

public:
  static std::string opEnum[14];

  SystemLog(std::string syslog_path_);

  ~SystemLog();

  //
  void save(const char *, int pri, int op);

  void read(int k = -1);
};


#endif //LOGS_HPP
