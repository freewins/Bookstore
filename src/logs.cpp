//
// Created by Freewings on 2024/12/15.
//
#include <memory_resource>
#include<iomanip>
#ifndef LOGS_CPP
#define LOGS_CPP
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
  logProgift.write_info(count, 2);
  logProgift.write_info(now_cur, 1);
}

void Profit::save(double profit) {
  count++;
  financeLog fo;
  fo.profit = profit;
  if (profit < 0) {
    fo.op_ = 0;
  } else {
    fo.op_ = 1;
  }
  now_cur = logProgift.write(fo);
}

bool Profit::read(int k) {
  double income = 0;
  double outcome = 0;
  if (k == -1) {
    k = count;
    if (k == 0) {
      printf("+ %.2lf - %.2lf\n", income, outcome);
      return true;
    }
  }
  if (k == 0) {
    std::cout << "\n";
    return true;
  }
  if (k > count) {
    return false;
  } else {
    //要读入全部的数据log
    //TODO 写一个读特定长度数据的函数
    financeLog *pool = new financeLog[k + 2];
    logProgift.read_block(pool, k, count);
    for (int i = 0; i < k; i++) {
      if (pool[i].profit >= 0) {
        income += pool[i].profit;
      } else {
        outcome += pool[i].profit;
      }
    }
    if (outcome < 0) {
      outcome = -outcome;
    }
    printf("+ %.2lf - %.2lf\n", income, outcome);
    delete[] pool;
    return true;
  }
}

//打印财务报表
void Profit::print() {
  int k = count;
  int i = 1, j = 32;
  double income = 0;
  double outcome = 0;
  financeLog *pool = new financeLog[34];
  std::cout << "---------Finance---------\n";
  std::cout << std::setfill(' ');
  std::cout << std::setw(15) << "Operator" << std::setw(15) << " Total" << "\n";
  for (; i * j <= k; i++) {
    if (i * j <= k) {
      logProgift.read_block(pool, j, count - (i - 1) * j);
    }
    for (int t = 0; t < j; t++) {
      if (pool[t].op_ == 1) {
        std::cout << std::setw(15) << "BUY" << std::setw(15) << pool[t].profit << "\n";
        income += pool[t].profit;
      } else if (pool[t].op_ == 0) {
        std::cout << std::setw(15) << "IMPORT" << std::setw(15) << pool[t].profit << "\n";
        outcome += pool[t].profit;
      }
    }
  }
  if (i * j != k) {
    int f = k - (i - 1) * j;
    logProgift.read_block(pool, f, count - (i - 1) * j);
    for (int t = 0; t < f; t++) {
      if (pool[t].op_ == 1) {
        std::cout << std::setw(15) << "BUY" << std::setw(15) << pool[t].profit << "\n";
        income += pool[t].profit;
      } else if (pool[t].op_ == 0) {
        std::cout << std::setw(15) << "IMPORT" << std::setw(15) << pool[t].profit << "\n";
        outcome += pool[t].profit;
      }
    }
  }
  std::cout << std::setfill('-') << std::setw(25) << "-" << "\n";
  std::cout << " Income: " << income << " Outcome: " << std::abs(outcome) << "\n";
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
  logOperator.write_info(count, 2);
  logOperator.write_info(now_cur, 1);
}

//保留员工操作
// op = 1 buy op = 2 Modify op = 3 create_user op = 4 register op = 5 passwd
void Operator::save(const char *userId_, int op) {
  OpLog newOp(userId_, op);
  count++;
  logOperator.write(newOp);
}

//输出k个员工操作
void Operator::read(int k) {
  if (k == -1) {
    //全部输出
    k = count;
  }
  if (k == 0) {
    std::cout << "\n";
    return;
  }
  int i = 1, j = 32;
  std::cout << std::setfill(' ');
  Operator::OpLog *pool = new Operator::OpLog[34]{};
  for (; i * j <= k; i++) {
    if (i * j <= k) {
      logOperator.read_block(pool, j, count - (i - 1) * j);
    }
    for (int t = 0; t < j; t++) {
      std::cout << std::setw(25) << pool[t].userId;
      switch (pool[t].op_) {
        case 1: {
          std::cout << std::setw(30) << "Orperation: BUY";
          break;
        }
        case 2: {
          std::cout << std::setw(30) << "Orperation: MODIFY";
          break;
        }
        case 3: {
          std::cout << std::setw(30) << "Orperation: IMPORT";
          break;
        }
        case 4: {
          std::cout << std::setw(30) << "Orperation: CREATE_USER";
          break;
        }
        case 5: {
          std::cout << std::setw(30) << "Orperation: REGISTER";
          break;
        }
        case 6: {
          std::cout << std::setw(30) << "Orperation: PASSWORD";
          break;
        }
      }
      std::cout << "\t" << pool[t].opTime.tm_year + 1900 << "/" <<
          pool[t].opTime.tm_mon + 1 << "/" <<
          pool[t].opTime.tm_mday << " - " <<
          pool[t].opTime.tm_hour << ":" << pool[t].opTime.tm_min << "\t\n";
    }
  }
  if (i * j != k) {
    int f = k - (i - 1) * j;
    logOperator.read_block(pool, f, count - (i - 1) * j);
    for (int t = 0; t < f; t++) {
      std::cout << std::setw(25) << pool[t].userId;
      switch (pool[t].op_) {
        case 1: {
          std::cout << std::setw(30) << "Orperation: BUY";
          break;
        }
        case 2: {
          std::cout << std::setw(30) << "Orperation: MODIFY";
          break;
        }
        case 3: {
          std::cout << std::setw(30) << "Orperation: IMPORT";
          break;
        }
        case 4: {
          std::cout << std::setw(30) << "Orperation: CREATE_USER";
          break;
        }
        case 5: {
          std::cout << std::setw(30) << "Orperation: REGISTER";
          break;
        }
        case 6: {
          std::cout << std::setw(30) << "Orperation: PASSWORD";
          break;
        }
      }
      std::cout << "\t" << pool[t].opTime.tm_year + 1900 << "/" <<
          pool[t].opTime.tm_mon + 1 << "/" <<
          pool[t].opTime.tm_mday << " - " <<
          pool[t].opTime.tm_hour << ":" << pool[t].opTime.tm_min << "\t\n";
    }
  }
}

SystemLog::SystemLog(std::string syslog_path_) : logSystem(syslog_path_) {
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
  logSystem.write_info(count, 2);
  logSystem.write_info(now_cur, 1);
}

//读取一定数量的系统日志
void SystemLog::read(int k) {
  if (k == -1) {
    //全部输出
    k = count;
  }
  if (k == 0) {
    std::cout << "\n";
    return;
  }
  std::cout << std::setfill(' ');
  int i = 1, j = 32;
  SystemLog::SysLog *pool = new SystemLog::SysLog[34];
  for (; i * j <= k; i++) {
    if (i * j <= k) {
      logSystem.read_block(pool, j, count - (i - 1) * j);
    }
    for (int t = 0; t < j; t++) {
      std::cout << std::setw(25) << pool[t].userId << std::setw(20) << pool[t].op;
      std::cout << "\t" << pool[t].opTime.tm_year + 1900 << "/" <<
          pool[t].opTime.tm_mon + 1 << "/" <<
          pool[t].opTime.tm_mday << " - " <<
          pool[t].opTime.tm_hour << ":" << pool[t].opTime.tm_min << "\t\n";
    }
  }
  if (i * j != k) {
    int f = k - (i - 1) * j;
    logSystem.read_block(pool, f, count - (i - 1) * j);
    for (int t = 0; t < f; t++) {
      std::cout << std::setw(25) << pool[t].userId << std::setw(20) << pool[t].op;
      std::cout << "\t" << pool[t].opTime.tm_year + 1900 << "/" <<
          pool[t].opTime.tm_mon + 1 << "/" <<
          pool[t].opTime.tm_mday << " - " <<
          pool[t].opTime.tm_hour << ":" << pool[t].opTime.tm_min << "\t\n";
    }
  }
}

//写入系统日志 op代表操作 0 - 13
void SystemLog::save(const char *userId_, int pri, int op) {
  SysLog sys(userId_, pri, op);
  logSystem.write(sys);
  count++;
}


#endif
