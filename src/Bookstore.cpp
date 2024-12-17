#include<iostream>
#include<string>
#include<vector>
#include<stack>
#include<exception>
#include<map>
#include<time.h>
#include "user.hpp"
#include"book.hpp"
#include "file.hpp"
#include "logs.hpp"
//保存系统状态

struct aBook {
  char ISBN[20];
  book::Book _book;
  aBook & operator=(const aBook & a) {
    strcpy(ISBN,a.ISBN);
    this->_book = a._book;
  }
};

struct Statement {
  int nowPrivilige;
  aBook _abook;
  std::string userId;
};

enum INFO{ISBN,NAME,AUTHOR,KETWORD,PRICE,dFualt};

const std::string Path[10] = {
  ".\\Data\\Index\\userInfoIndex.index",
  ".\\Data\\Index\\bookInfoIndex.index",
  ".\\Data\\basicInfo.data",
  ".\\Data\\bookInfo.data",
  ".\\Data\\userInfo.data",
  ".\\Data\\syslogInfo.data",
  ".\\Data\\operatorInfo.data",
  ".\\Data\\profitInfo.data"
};

class defualtError : public std::exception {
private:
  std::string message;

public:
  defualtError(const std::string &message_): message(message_) {
  }

  const char *what() const noexcept override {
    return message.c_str();
  }
};



void Run(user &user_, book &book_, Profit &_log_profit,
         SystemLog &_log_sys, Operator &_log_operator);

void splitOrder(std::string &input, std::vector<std::string> &orders);


int main() {
  //存储文件保存路径
  user _user(Path[0], Path[4]);
  book _book(Path[1], Path[3]);
  Profit _log_profit(Path[7]);
  SystemLog _log_sys(Path[5]);
  Operator _log_operator(Path[6]);
  try {
    Run(_user, _book, _log_profit, _log_sys, _log_operator);
  } catch (...) {
  }
}

int getInt(const std::string & number) {
  int sum;
  for(int i = number.size() - 1; i>= 0; i--) {
    if(number[i] <='9'&&number[i] >='0') {
      sum = sum*10 + (number[i] - '0');
    }
    else return -1;
  }
  return sum;
}

double getDouble() {

}

INFO getInfoType(const std::string & op,std::string data) {
  int l = op.size();
  if(op[0] != '-') {
    return INFO::dFualt;
  }
  else {
    size_t p = op.find('=');

    if(p != std::string::npos) {
      std::string get_op =  op.substr(1,p - 1);
      if(get_op == "ISBN") {
        return INFO::ISBN;
      }
      else if(get_op == "name") {
        return INFO::NAME;
      }
      else if(get_op == "author") {
        return INFO::AUTHOR;
      }
      else if(get_op == "keyword") {
        return INFO::KETWORD;
      }
      else if(get_op == "price") {
        return INFO::PRICE;
      }else {
        return INFO::dFualt;
      }
    }
    else {
      return INFO::dFualt;
    }
  }
}

void splitOrder(std::string &input, std::vector<std::string> &orders) {
  //用于分割指令 后面将这个函数放到lib中
  int i = 0;
  int l = input.size();
  orders.clear();
  while (i < l) {
    while (i < l && input[i] == ' ') {
      i++;
    }
    if (i == l)
      return;
    int j = i;
    while (j < l && input[j] != ' ') {
      j++;
    }
    orders.push_back(input.substr(i, j - i));
  }
}

void Run(user &user_, book &book_, Profit &_log_profit,
         SystemLog &_log_sys, Operator &_log_operator) {
  std::string input;
  std::vector<std::string> orders;
  std::stack<Statement> state;
  //保存当前选择的图书
  aBook book_tmp;
  //保存当前登录的用户
  std::string userId;
  //当前权限
  int now_privilege = 0;
  int l; //指令参数计数器
  while (true) {
    try {
      std::cin >> input;
      std::getline(std::cin, input);
      if (input == "") {
        continue;
      } else {
        splitOrder(input, orders);
        l = orders.size();

        //User
        if (orders[0] == "su") {
          int privilege_;
          bool success_login = false;
          if (l == 2) {
            success_login = user_.login(orders[1].c_str(),privilege_);
          } else if (l == 3) {
            success_login = user_.login(orders[1].c_str(),privilege_, orders[2].c_str());
          } else {
            throw defualtError("Invalid\n");
          }
          if (success_login) {
            //登陆成功
            //检测是否有账户
            if(!state.empty()) {//有账户压栈
              state.push({now_privilege,book_tmp,userId});
            }
            //change state
            userId = orders[1];
            book_tmp.ISBN[0] = '\0';
            now_privilege = privilege_;
          } else {
            throw defualtError("Invalid\n");
          }
        }
        else if(orders[0] == "logout") {
          if(l > 1 || state.empty()) {
            throw defualtError("Invalid\n");
          }
          else {
            state.pop();
            if(state.empty()) {
              now_privilege = 0;
            }
            else {
              userId = state.top().userId;
              now_privilege = state.top().nowPrivilige;
              book_tmp = state.top()._abook;
            }
          }
        }
        else if(orders[0] == "register") {
          if(l != 4) {
            throw defualtError("Invalid\n");
          }
          else {
            if(!user_.addUser(orders[1].c_str(),orders[3].c_str(),1,orders[2].c_str())) {
              throw defualtError("Invalid\n");
            }
          }
        }
        else if(orders[0] == "passws") {
          if(l >4 || l < 3||now_privilege == 0) {
            throw defualtError("Invalid\n");
          }
          else {
            if( l == 3) {
              if(!user_.modifyPasswd(orders[1].c_str(),orders[3].c_str())) {
                throw defualtError("Invalid\n");
              }
            }
            else {
              if(!user_.modifyPasswd(orders[1].c_str(),orders[3].c_str(),orders[2].c_str())) {
                throw defualtError("Invalid\n");
              }
            }
          }
        }
        else if(orders[0] == "useradd") {
          if(l != 5 || now_privilege < 3) {
            throw defualtError("Invalid\n");
          }
          else {
            int tmp = getInt(orders[3]);
            if(tmp == -1) {
              throw defualtError("Invalid\n");
            }
            if(!user_.addUser(orders[0].c_str(),orders[4].c_str(),tmp,orders[2].c_str())) {
              throw defualtError("Invalid\n");
            }
          }
        }
        else if(orders[0] == "delete") {
          if(l != 2 || orders[1] == userId || now_privilege < 7) {
            throw defualtError("Invalid\n");
          }else {
            if(!user_.daleteUser(orders[1].c_str())) {
              throw defualtError("Invalid\n");
            }
          }
        }


        //Book
        else if(orders[0] == "buy") {
          if(l != 3) {
            throw defualtError("Invalid\n");
          }
        }
        else if(orders[0] == "selete") {

        }
        else if(orders[0] == "modify") {

        }
        else if(orders[0] == "import") {

        }

        //log
        else if(orders[0] == "show") {
          //打印总支出
          if( l == 3) {
            if(orders[1] == "finance") {
              int tmp = getInt(orders[2]);
              if(tmp == -1) {
                throw defualtError("Invalid\n");
              }
              else {
                _log_profit.read(tmp);
              }
            }
          }//
          else if(l == 2) {

          }
          else {
            throw defualtError("Invalid\n");
          }
        }

        //exit
        else if(orders[0] == "quit"||orders[0] == "exit") {
          return;
        }

      }
    } catch (const defualtError &e) {
      std::cout << e.what();
    }
  }
}
