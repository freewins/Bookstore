#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<exception>
#include "user.cpp"
#include"book.cpp"
#include "file.cpp"
#include "logs.cpp"
#include"utils.hpp"
//保存系统状态

struct aBook {
  char ISBN[20];
  book::Book _book;

  aBook &operator=(const aBook &a) {
    strcpy(ISBN, a.ISBN);
    this->_book = a._book;
    return *this;
  }
};

struct Statement {
  int nowPrivilige;
  int book_Pos;
  aBook _abook;
  std::string userId;
};

enum INFO { ISBN, NAME, AUTHOR, KETWORD, PRICE, dFualt };

const std::string Path[10] = {
  "../Data/Index/userInfoIndex.index",
  "../Data/Index/bookInfoIndex.index",
  "../Data/basicInfo.data",
  "../Data/bookInfo.data",
  "../Data/userInfo.data",
  "../Data/syslogInfo.data",
  "../Data/operatorInfo.data",
  "../Data/profitInfo.data"
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

int getInt(const std::string &number);

double getDouble(const std::string &number);

INFO getInfoType(const std::string &op, std::string &data);

void Run(user &user_, book &book_, Profit &_log_profit,
         SystemLog &_log_sys, Operator &_log_operator);

void splitOrder(std::string &input, std::vector<std::string> &orders);


int getInt(const std::string &number) {
  int sum = 0;
  int flag = 1;
  int i = 0;
  if (number[0] == '-') {
    flag = -1;
    i = 1;
  }
  for (; i < number.size(); i++) {
    if (number[i] <= '9' && number[i] >= '0') {
      sum = sum * 10 + (number[i] - '0');
    } else return -1;
  }

  return sum * flag;
}

double getDouble(const std::string &number) {
  size_t pos = number.find('.');
  if (pos != std::string::npos) {
    //说明有小数点 分两部分读入
    double ans = 0;
    int flag = 1;
    int i = 0;
    if (number[i] == '-') {
      flag = -1;
      i = 1;
    }
    for (; i < pos; i++) {
      if (number[i] <= '9' && number[i] >= '0') {
        ans = ans * 10 + (number[i] - '0');
      } else {
        return -1;
      }
    }
    i = pos + 1;
    int base = 10;
    for (; i < number.size(); i++) {
      if (number[i] >= '0' && number[i] <= '9') {
        ans += (double) (number[i] - '0') / base;
        base *= 10;
        if (base >= 10000) {
          //最多四位小数
          break;
        }
      } else {
        return -1;
      }
    }
    return flag * ans;
  }
  else {
    double ans = getInt(number);
    return ans;
  }
  return -1;
}

INFO getInfoType(const std::string &op, std::string &data) {
  int l = op.size();
  if (op[0] != '-') {
    return INFO::dFualt;
  } else {
    size_t p = op.find('=');
    size_t l = op.size();
    if (p != std::string::npos) {
      std::string get_op = op.substr(1, p - 1);

      if (get_op == "ISBN") {
        data = op.substr(p + 1);
        if(check_no_blank(data.c_str())) {
          return INFO::ISBN;
        }
        return INFO::dFualt;
      } else if (get_op == "name") {
        //去掉冒号
        if (p + 1 < l && op[p + 1] == '\"') {
          int next_pos = op.find('\"',p + 2 );
          if (next_pos == l - 1) {
            int i = p+2;
            int j = next_pos - p - 2;
            data = op.substr(p + 2, next_pos - p - 2 );
            if(check_no_blank_quation(data.c_str())) {
              return INFO::NAME;
            }
            return INFO::dFualt;
          }
        }
        return INFO::dFualt;
      } else if (get_op == "author") {
        if (p + 1 < l && op[p + 1] == '\"') {
          int next_pos = op.find('\"',p + 2 );
          if (next_pos == l - 1) {
            data = op.substr(p + 2, next_pos - p - 2 );
            if(check_no_blank_quation(data.c_str())) {
              return INFO::AUTHOR;
            }
            return INFO::AUTHOR;
          }
        }
        return INFO::dFualt;
      } else if (get_op == "keyword") {
        if (p + 1 < l && op[p + 1] == '\"') {
          int next_pos = op.find('\"',p + 2);
          if (next_pos == l - 1) {
            data = op.substr(p + 2, next_pos - p - 2 );
            if(check_no_blank_quation(data.c_str())) {
              return INFO::KETWORD;
            }
            return INFO::dFualt;
          }
        }
        return INFO::dFualt;
      } else if (get_op == "price") {
        data = op.substr(p + 1);
        return INFO::PRICE;
      } else {
        return INFO::dFualt;
      }
    } else {
      return INFO::dFualt;
    }
  }
}

void splitOrder(std::string &input, std::vector<std::string> &orders) {
  //用于分割指令 后面将这个函数放到lib中
  int i = 0;
  int l = input.size();
  std::string tmp;
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
    tmp = input.substr(i, j - i);
    tmp += "\0";
    orders.push_back(tmp);
    i = j+1;
  }
}


void Run(user &user_, book &book_, Profit &_log_profit,
         SystemLog &_log_sys, Operator &_log_operator) {
  std::string input;
  std::vector<std::string> orders;
  std::vector<Statement> state;
  //保存当前选择的图书
  aBook book_tmp;
  //保存当前登录的用户
  std::string userId;
  //当前权限
  int now_privilege = 0;
  int now_bookPos = -1;
  int l; //指令参数计数器
  while (true) {
    try {
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
              success_login = user_.login(orders[1].c_str(), privilege_);
          } else if (l == 3) {
            success_login = user_.login(orders[1].c_str(), privilege_, orders[2].c_str());
          } else {
            throw defualtError("Invalid\n");
          }
          if (success_login) {
            //登陆成功 就压栈 栈顶为当前登陆用户
            userId = orders[1];
            now_bookPos = -1;
            now_privilege = privilege_;
            //有账户压栈
              state.push_back({now_privilege, now_bookPos, book_tmp, userId});
            //change state
          } else {
            throw defualtError("Invalid\n");
          }
        }
        else if (orders[0] == "logout") {
          if (l > 1 | now_privilege == 0) {
            throw defualtError("Invalid\n");
          } else {
            state.pop_back();
            if (state.empty()) {
              now_privilege = 0;
              userId[0] = '\0';
              now_bookPos = -1;
            } else {
              userId = state.back().userId;
              now_privilege = state.back().nowPrivilige;
              book_tmp = state.back()._abook;
            }
          }
        }
        else if (orders[0] == "register") {
          if (l != 4) {
            throw defualtError("Invalid\n");
          } else {
            if(check_only_(orders[1].c_str())&&check_only_(orders[2].c_str())&&check_no_blank(orders[3].c_str())) {
              if (!user_.addUser(orders[1].c_str(), orders[3].c_str(), 1, orders[2].c_str())) {
                throw defualtError("Invalid\n");
              }
            }else {
              throw defualtError("Invalid\n");
            }

          }
        }
        else if (orders[0] == "passwd") {
          if (l > 4 || now_privilege == 0) {
            throw defualtError("Invalid\n");
          } else {
            if (l == 3) {//有没有修改密码为空的情况
              if(now_privilege  == 7) {
                if(check_only_(orders[2].c_str())) {
                  if (!user_.modifyPasswd(orders[1].c_str(), orders[2].c_str())) {
                    throw defualtError("Invalid\n");
                  }
                }else {
                  throw defualtError("Invalid\n");
                }

              }
              else {
                throw defualtError("Invalid\n");
              }
            }
            else if(l == 4){
              if(check_only_(orders[3].c_str())) {
                if (!user_.modifyPasswd(orders[1].c_str(), orders[3].c_str(), orders[2].c_str())) {
                  throw defualtError("Invalid\n");
                }
              }
              else {
                throw defualtError("Invalid\n");
              }
            }
            else {
              throw defualtError("Invalid\n");
            }
          }
        }
        else if (orders[0] == "useradd") {
          if (l != 5 ) {
            throw defualtError("Invalid\n");
          } else {
            int tmp = getInt(orders[3]);
            if (tmp == -1) {
              throw defualtError("Invalid\n");
            }
            if(tmp >= now_privilege) {
              throw defualtError("Invalid\n");
            }
            if(check_only_(orders[1].c_str())&&check_no_blank(orders[4].c_str())&&check_only_(orders[2].c_str())) {
              if (!user_.addUser(orders[1].c_str(), orders[4].c_str(), tmp, orders[2].c_str())) {
                throw defualtError("Invalid\n");
              }
            }
            else {
              throw defualtError("Invalid\n");
            }

          }
        }
        else if (orders[0] == "delete") {
          if (l != 2 || orders[1] == userId || now_privilege < 7) {
            throw defualtError("Invalid\n");
          } else {
            for(auto t = state.begin();t != state.end();++t) {//检测是否已经在登陆栈中
              if(orders[1] == t->userId) {
                throw defualtError("Invalid\n");
              }
            }
            if (!user_.daleteUser(orders[1].c_str())) {
              throw defualtError("Invalid\n");
            }
          }
        }


        //Book
        else if (orders[0] == "buy") {
          if (l != 3 || now_privilege == 0) {
            throw defualtError("Invalid\n");
          } else {
            int tmp = getInt(orders[2]);
            if (tmp <= 0) {
              throw defualtError("Invalid\n");
            }
            double total;
            if (!book_.buy(orders[1].c_str(), tmp, total)) {
              throw defualtError("Invalid\n");
            } else {
              _log_profit.save(total);
              printf("%.2lf\n", total);
            }
          }
        }
        else if (orders[0] == "select") {
          if (l != 2 || now_privilege < 3) {
            throw defualtError("Invalid\n");
          } else {
            book_.selete(book_tmp._book, orders[1].c_str(), now_bookPos);
            strcpy(book_tmp.ISBN, orders[1].c_str());
          }
        }
        else if (orders[0] == "modify") {

          if (now_privilege < 2 || now_bookPos == -1) {
            throw defualtError("Invalid\n");
          } else {
            std::vector<std::string> datas;
            double _price;//price 仅出现一次 保存避免二次求值
            std::string tmp;
            INFO tmp_info;
            std::vector<INFO> infos;
            for (int i = 1; i < l; i++) {
              tmp_info = getInfoType(orders[i], tmp);
              if (tmp_info == INFO::dFualt) {
                throw defualtError("Invalid\n");
              } else {
                if(tmp_info == PRICE) {
                  _price = getDouble(tmp);
                  if(_price <  0) {
                    throw defualtError("Invalid\n");
                  }
                }
                infos.push_back(tmp_info);
                datas.push_back(tmp);
              }
            }
            for (int i = 1; i < l; i++) {
              tmp_info = infos[i - 1];

              switch (tmp_info) {
                case INFO::dFualt: {
                  throw defualtError("Invalid\n");
                  break;
                }
                case INFO::ISBN: {
                  if (book_.modify(book_tmp._book, book_tmp.ISBN, datas[i - 1].c_str(), now_bookPos, 1)) {
                    strcpy(book_tmp.ISBN, datas[i - 1].c_str());
                  } else {
                    throw defualtError("Invalid\n");
                  }
                  break;
                }
                case INFO::NAME: {
                  if (!book_.modify(book_tmp._book, book_tmp.ISBN, datas[i - 1].c_str(), now_bookPos, 2)) {
                    throw defualtError("Invalid\n");
                  }
                  break;
                }
                case INFO::AUTHOR: {
                  if (!book_.modify(book_tmp._book, book_tmp.ISBN, datas[i - 1].c_str(), now_bookPos, 3)) {
                    throw defualtError("Invalid\n");
                  }
                  break;
                }
                case INFO::KETWORD: {
                  if (!book_.modify(book_tmp._book, book_tmp.ISBN, datas[i - 1].c_str(), now_bookPos, 4)) {
                    throw defualtError("Invalid\n");
                  }
                  break;
                }
                case INFO::PRICE: {
                    if (!book_.modify(book_tmp._book, _price, now_bookPos)) {
                      throw defualtError("Invalid\n");
                    }
                    break;
                }
              }
            }
          }
        }
        else if (orders[0] == "import") {
          if (l != 3 || now_privilege < 3 || now_bookPos == -1) {
            throw defualtError("Invalid\n");
          } else {
            int tmp = getInt(orders[1]);
            double cost = getDouble(orders[2]);
            if (tmp < 0 || cost < 0) {
              throw defualtError("Invalid\n");
            } else {
              book_.import(book_tmp._book, tmp, cost, now_bookPos);
              _log_profit.save(cost * (-1) );
            }
          }
        }
        //log
        else if (orders[0] == "show") {
          if (now_privilege < 1) {
            throw defualtError("Invalid\n");
          }
          //打印总支出
          if (l == 3) {
            if (orders[1] == "finance") {
              if (now_privilege < 7) {
                throw defualtError("Invalid\n");
              }
              int tmp = getInt(orders[2]);
              if (tmp == -1) {
                throw defualtError("Invalid\n");
              } else {
                _log_profit.read(tmp);
              }
            } else {
              throw defualtError("Invalid\n");
            }
          } //
          else if (l == 2) {
            //按条件打印对应图书
            //还有可能是finance
            if(orders[1] == "finance") {
              _log_profit.read();
            }
            else{
            //按条件打印对应图书
              std::string data;
              switch (getInfoType(orders[1], data)) {
              case INFO::dFualt: {
                throw defualtError("Invalid\n");
                break;
              }
              case INFO::ISBN: {
                book_.bookIndex.showAll(data.c_str(), 1);
                break;
              }
              case INFO::NAME: {
                book_.bookIndex.showAll(data.c_str(), 2);
                break;
              }
              case INFO::AUTHOR: {
                book_.bookIndex.showAll(data.c_str(), 3);
                break;
              }
              case INFO::KETWORD: {
                book_.bookIndex.showAll(data.c_str(), 4);
                break;
              }
            }
            }
          } else if (l == 1) {
            book_.bookIndex.showAll(book_tmp.ISBN, 0);
          } else {
            throw defualtError("Invalid\n");
          }
        }

        //exit
        else if (orders[0] == "quit" || orders[0] == "exit") {
          return;
        }
        else {
          throw defualtError("Invalid\n");
        }
      }

    } catch (const defualtError &e) {
      std::cout << e.what();
    }
  }
}


 int main() {
    std::fstream file ;
    for(int i = 0;i<8;i++) {
      file.open(Path[i],std::ios::out|std::ios::trunc);
      file.close();
    }
  freopen("../testcases/basic/testcase5.in","r",stdin);
  freopen("../testcases/basic/testcase5-2.out","w",stdout);
  // //存储文件保存路径
  user _user(Path[0], Path[4]);
  book _book(Path[1], Path[3]);
  Profit _log_profit(Path[7]);
  SystemLog _log_sys(Path[5]);
  Operator _log_operator(Path[6]);

  try {
    Run(_user, _book, _log_profit, _log_sys, _log_operator);
  } catch (...) {
  }
  return 0;
}
