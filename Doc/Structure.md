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


