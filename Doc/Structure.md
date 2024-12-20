## Bookstore 总体设计文档

---

  **wrriten by freewings**


### 程序功能概述

---

本程序主要采取模块化设计，将主要分成 ```4```个主模块，并对各个模块细分成若干个小模块
```plain
- 文件模块
  - 图书信息读写模块
  - 用户信息读写模块
  - 日志信息读写模块
- 用户模块              ==> 用户信息读写模块
  - 用户登陆\注销模块
  - 用户管理模块      
- 图书模块              ==> 图书信息读写模块
  - 图书查询模块
  - 图书编辑模块
  - 图书采购模块
  - 图书购买模块
- 日志模块              ==> 日志信息读写模块
  - 营销记录储存模块
  - 员工操作储存模块
  - 系统信息储存模块
```
### 主体逻辑说明

---

- 接受指令 - 指令切片 - 分模块检测指令格式 - 执行指令 - 接受指令 - ... 
                           |- 抛出异常  -  处理异常 - |


### 代码文件结构

---

```plain
├── Bookstore
│   ├── include
│   │   ├── 
│   │   ├── 
│   │   ├── 
│   │   └── 
│   ├── src
│   │   ├── Bookstore.cpp
│   │   ├── file.cpp
│   │   ├── file.hpp
│   │   ├── user.hpp
│   │   ├── user.cpp
│   │   ├── book.cpp
│   │   ├── book.hpp
│   │   ├── logs.cpp
│   │   └── logs.hpp
│   ├── Data
│   │   ├── Index
│   │   │   ├── bookInfoIndex.index
│   │   │   ├── userInfoIndex.index
│   │   │   ├── syslogInfoIndex.index
│   │   │   ├── profitInfoIndex.index
│   │   │   └── operatorInfoIndex.index
│   │   ├── basicInfo.data
│   │   ├── bookInfo.data
│   │   ├── userInfo.data
│   │   ├── syslogInfo.data
│   │   ├── operatorInfo.data
│   │   └── profitInfo.data
│   ├── doc
│   ├── lib
│   ├── Cmakelist.txt
│   ├── .gitnore
│   ├── code
│   ├── README.md

```
#### 文件说明

- `Bookstore.cpp` 主程序 包含`main`函数和异常处理部分
- `Data`数据库文件，包含程序运行的全部信息
  - `Index`块状链表的索引表
  - `bookInfo.data`图书信息
  - `userInfo.data`用户信息
  - `syslogInfo.data`系统日志信息
  - `profitInfo.data`盈利信息
  - `operatorInfo.data` 操作信息
  - `basicInfo.data` 基本信息 如存储路径
- `bin` 各个模块的程序
  - `file.xxx`文件模块
  - `user.xxx`用户模块
  - `book.xxx`图书模块
  - `logs.xxx`日志模块
- `doc`项目文档
- `lib` 项目库
- `code` 可执行文件
### 功能设计

---


### 数据库设计

---

#### 用户数据库设计

- 数据内容 
  - `struct User`
    - `UserId` **唯一**标识
      - 存储类型 `char`
      - 存储大小 `32` 
    - `Privilege` 用户权限 
      - 存储类型 `int`
      - 存储大小 `1`
    - `UserName` 用户名
      - 存储类型 `char`
      - 存储大小 `32`
    - `Password` 密码
      - 存储类型 `char`
      - 存储大小 `32`
- 存储方式 
  - `userInfo.index`存储对应的下标文件
  - `userInfo.data` 存储对应的文件信息

#### 图书数据库设计

- 数据内容
  - `struct Book`
    - `ISBN` **唯一**标识
      - 存储类型 `char`
      - 存储大小 `20`
    - `BookName` 书名
      - 存储类型 `char`
      - 存储大小 `60`
    - `AuthorName` 作者名
      - 存储类型 `char`
      - 存储大小 `60`
    - `Keyword` 图书关键词
      - 存储类型 `char`
      - 存储大小 `60`
    - `Price` 图书单价
      - 存储类型 `double`
      - 存储大小 `1`
    - `Amount` 总库存
      - 存储类型 `int`
      - 存储大小 `1`
- 存储方式
  - `bookInfo.index`存储对应的下标文件
  - `bookInfo.data` 存储对应的文件信息

#### 日志数据库设计

- 日志数据 --> 基类 LogBase
  -  **Profit**
    - 数据内容
      - `Id` **唯一标识** 表示第`Id`次交易 
        - 存储类型 `int`
        - 存储大小 `1`
      - `Profit` 交易利润 可正可负
        - 存储类型 `double`
        - 存储大小 `1`
    - 存储方式
        - `profitInfoIndex.index`存储对应的下标文件
        - `profitInfo.data` 存储对应的文件信息
  - **Operator**
    - 数据内容
      - `UserId` **唯一标识** 表示对应员工ID
        - 存储类型 `char`
        - 存储大小 `32`
      - `OpType` 操作名称
        - 存储类型 `Op` 这里想写成几个子类，进行集成，分别输出不同的信息
        - 存储大小 `1`
    - 存储方式
      - `operatorInfoIndex.index`存储对应的下标文件
      - `operatorInfo.data` 存储对应的文件信息
  - **SystemLog**
     - 数据内容
       - `Time` **唯一标识** 表示记录的时间
         - 存储类型 `long long`
         - 存储大小 `1`
       - `LogStatement` 语句的形式输出一行系统记录
         - 存储类型 `char`
         - 存储大小 `100`
     - 存储方式
       - `syslogInfoIndex.index`存储对应的下标文件
       - `syslogInfo.data` 存储对应的文件信息

### 类结构体设计

---
#### 文件类 file

user 和 book 的文件类
```c++

template<typename T, int len, int info = 3>
class Index {
  struct Block {
    long long cur;
    int now_size;
    char max[len];
    char min[len];
    Block *next;
  };
  //用一个结构体来封装数据
  struct Data {
    char key[len];
    T value;

    friend bool operator<(const Data &a, const Data &b);
    friend bool operator==(const Data &a, const Data &b);
    Data &operator =(const Data &a) ;
  };
  private:
  const std::string Indexnmame;
  const std::string Blockname;
  const int Blocksize;
  int offset;
  int Blockcount;
  int sqrBlocksize;
  
  //前置信息
  int info_data[info];
  //索引文件
  std::fstream index_file;
  //块状文件的存储
  std::fstream block_file;
  //头指针
  Block *head = nullptr;
  //写入尾指针
  Block *tail = nullptr;

public:
  //构造函数
  Index(int blocksize, const std::string &indexname, const std::string &blockname);
  
  //析构函数
  ~Index();

  //获得第n个info
  int getInfo(int n);

  //修改对应位置的info
  void updateInfo(int n, int val);

  //初始化 读入所有Index
  void Initialise();

  //检测是否需要合并
  void Merge(Block *, Block *);

  //检测是否需要裂块
  void Split(Block *);

  //在当前块中插入数据
  bool insertData(const char *, T &value);

  //在当前块中删除块
  bool deleteData(const char *, T &value);
  
  //更新数据
  bool updateData(T &value, int pos);

  //在当前块中寻找数据
  T findData(const char *, bool &);

  /// 在当前块中寻找数据
  T findData(const char *, bool &, int &);

  //用于Book的特化模版展示遍历所有匹配数据
  void showAll(const char *, int);
};
  
```
log 的文件类

```c++
template<class T, int info_len = 2>
class LogFile {
private:
  /* your code here */
  std::fstream file;
  std::string file_name;
  int sizeofT = sizeof(T);
  int max_size = 0;
  int offset = sizeof(int) * info_len;
  bool isFirst;

public:
  LogFile() = default;

  LogFile(const std::string &file_name);

  ~LogFile();

  bool firstOpen();
  //除去info的offset
  int getOffset() ;

  void initialise(const std::string &FN = "");

  //读出第n个int的值赋给tmp，1_base
  void get_info(int &tmp, int n);

  //将tmp写入第n个int的位置，1_base
  void write_info(int tmp, int n);

  //在文件合适位置写入类对象t，并返回写入的位置索引index
  //位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
  //位置索引index可以取为对象写入的起始位置
  int write(T &t);

  //用t的值更新位置索引index对应的对象，保证调用的index都是由write函数产生
  void update(T &t, const int index);

  //读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
  void read(T &t, const int index);

  //注意这里传入的n读的块数
  void read_block(T *a, int n, int total);

  //删除位置索引index对应的对象(不涉及空间回收时，可忽略此函数)，保证调用的index都是由write函数产生
  void Delete(int index);
};

```

#### 用户类 user

```c++

class user{
private:
  int total_user = 0; //总用户数
  char userId[32]; //userId 
  Index<User, 32> userIndex; // 存储数据的文件流对象
  
  struct User {
    char password[32];
    char username[32];
    int privilege;

    User &operator=(const User &a) ;

    User() ;
    User(const char *password_, const char *username_, int privilege_);
  };
  
public:
  // 构造函数
   user(std::string index_path_, std::string block_path_): userIndex(1024, index_path_, block_path_) {
    userIndex.Initialise(); //首先初始化文件
    total_user = userIndex.getInfo(1); //数量统一放在文件的第一个位置处
    if (total_user == 0) {
      addUser("root", "root", 7, "sjtu");
    }
  }
  //析构函数
  ~user() {
    userIndex.updateInfo(1, total_user);
  }
  //登陆函数
  bool login(const char *, int &privilege, const char *password = nullptr);
  ///退出登录
  bool logout();
  //更改密码
  bool modifyPasswd(const char *userId, const char *newPassword, const char *currentPassword = nullptr);
  //添加用户
  bool addUser(const char *userId, const char *userName, int privilege, const char *password = nullptr);
  //删除用户
  bool daleteUser(const char *userId);
}

```
#### 图书类 book
```c++


class book {
  //Info 1: 书籍总数
  //Info 0: 不可访问 Blockcount
private:
  int total_book; // 书籍总数 
  char ISBN[20];//ISBN

public:
  struct Book {
    char bookName[62];
    char authorName[62];
    char keyWord[70];
    double price;
    int amount;

    Book() ;
     Book &operator=(const Book&) ;
     bool findKey(const char *) ;
     bool insertKey(const char *);
     void eraseSpace(std::string & ) ;
  }
  book(std::string index_path_, std::string block_path_);
  ~book();
  //购买图书
  bool buy(const char *ISBN, int &quantity, double &total);
  //展示图书
  bool show_book(const char *find_key, int mode = 0);
  //选择图书
  bool selete(Book &tmp, const char *ISBN, int &pos);
  //修改图书
  bool modify(Book &, double &price, int &);
  //修改图书的重载函数
  bool modify(Book &, const char *ISBN, const char *new_data, int &pos, int mode);
  //采购图书
  bool import(Book &, int quantity, double total_cost, int &pos);
};

```

#### 日志类

```c++
//日志不涉及非线性查找和删除 故直接使用常规文件读写
//log基类
class logs {
public:
  void save();

  void read();

  void print();
};
//利润的类Profit
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
  void save(double profit);

  bool read(int k = -1);
};
//员工操作类
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

    OpLog(char *userId_, char *userName_, int op_);
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
  void read();
};
//系统日志读写类
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
```

