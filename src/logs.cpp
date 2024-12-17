//
// Created by Freewings on 2024/12/15.
//

#include "logs.hpp"


Profit::Profit(std::string profit_path_) : logProgift(profit_path_) {
  if (logProgift.firstOpen()) {
    logProgift.initialise();
    count = 0;
    now_cur = logProgift.getOffset();
  } else {
    logProgift.get_info(now_cur, 1);
    logProgift.get_info(count, 2);
  }
}

Profit::~Profit() {
  logProgift.write_info(count,2);
  logProgift.write_info(now_cur,1);
}

void Profit::save(int n, double profit) {
  count++;
  now_cur = logProgift.write(profit);
}

bool Profit::read(int k) {
  double income = 0;
  double outcome = 0;
  if (k == -1) {
    if (count == 0) {
      printf("+ %.2lf - %.2lf\n", income, outcome);
      return true;
    }
    k = count;
  }
  if (k > count) {
    return false;
  } else {
    if (count == 0) {
      std::cout << "\n";
      return true;
    }
    //要读入全部的数据log
    //TODO 写一个读特定长度数据的函数
    double *pool = new double[k + 2];
    logProgift.read_block(pool, k, count);
    for (int i = 0; i < k; i++) {
      if (pool[i] >= 0) {
        income += pool[i];
      } else {
        outcome += pool[i];
      }
    }
    printf("+ %.2lf - %.2lf\n", income, outcome);
    delete[] pool;
    return true;
  }
}


Operator::Operator(std::string operator_path_): logOperator(operator_path_) {
  if (logOperator.firstOpen()) {
    logOperator.initialise();
    count = 0;
    now_cur = logOperator.getOffset();
  } else {
    logOperator.get_info(now_cur, 1);
    logOperator.get_info(count, 2);
  }
}

Operator::~Operator() {
  logOperator.write_info(count,2);
  logOperator.write_info(now_cur,1);
}


void Operator::save(char *userId_, char *userName_, int op) {
  OpLog newOp(userId_, userName_, op);
  count++;
  logOperator.write(newOp);
}

SystemLog::SystemLog(std::string syslog_path_) :logSystem(syslog_path_){
  if (logSystem.firstOpen()) {
    logSystem.initialise();
    count = 0;
    now_cur = logSystem.getOffset();
  } else {
    logSystem.get_info(now_cur, 1);
    logSystem.get_info(count, 2);
  }
}

SystemLog::~SystemLog() {
  logSystem.write_info(count,2);
  logSystem.write_info(now_cur,1);
}

