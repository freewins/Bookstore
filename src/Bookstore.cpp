

#include<iostream>
#include<string>
#include<stack>
#include<map>
#include<time.h>
#include "user.hpp"
#include "file.hpp"

//保存系统状态
struct Statement {
  int nowPrivilige;
  std::stack<std::pair<user,char *>> LogStack; // 存储用户和当前选择的图书
}state;

const std::string Path[10] ={
  ".\\Data\\"
};
enum error {defualt}; //保存错误信息 在没有细化之前都用defualt

void Run();
void Initialisze();//初始化函数
int main() {
  //存储文件保存路径

  try {
    Run();
  }catch (...) {

  }
}

void Initialisze() {
  state.nowPrivilige = 0;
  //初始化文件输入输出流
  //读入图书信息，用户信息的index文件
  //检测是否为第一次登陆 创建超级管理员账户

}
void splitOrder(std::string &input,std::string *order,int & ip) {//用于分割指令 后面将这个函数放到lib中

}
void Run() {
  std::string input;
  std::string order[10];//将指令进行拆分
  int ip;//指令参数计数器
  try {
    while(true) {
      std::cin>>input;
      //TODO 这里能不能集合成几个指令分发器 采取读一行的操作 对指令进行分割
      std::getline(std::cin,input);
      for(int i = 0;i<ip;i++) {
        if(order[i] == "") {//用户模块

        }
      }
    }
  }catch (...){
  }
}
