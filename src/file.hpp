//
// Created by Freewings on 2024/12/15.
//

#ifndef FILE_HPP
#define FILE_HPP
#include<cmath>
#include <cstring>
#include<fstream>
#include<algorithm>

#include"user.hpp"





template<typename  T,int len,int info = 3>
class Index {
  struct  Block {
    long long cur;
    int now_size;
    char max[len];
    char min[len];
    Block *next;
  };
  struct Data {
    char key[len];
    T value;
    friend bool operator< (const Data & a,const Data & b) {
      return strcmp(a.key, b.key) < 0;
    }
    // friend bool operator<= (const Data & a,const Data & b) {
    //
    //   return strcmp(a.key, b.key) < 0;
    //
    // }
    friend bool operator== (const Data & a,const Data & b) {
      return strcmp(a.key,b.key) == 0;
    }
    Data & operator = (const Data & a) {
      strcpy(this->key,a.key);
      this->value = a.value;
      return *this;
    }
  };
private:
  const std::string Indexnmame;
  const std::string Blockname;
  const int Blocksize;
  int offset;
  int Blockcount;
  int sqrBlocksize;

  int info_data[info];
  //索引文件
  std::fstream index_file;
  //块状文件的存储
  std::fstream block_file;
  //头指针
  Block *head = nullptr;
  Block *tail = nullptr;


public:

  Index(int blocksize, const std::string &indexname, const std::string &blockname): Blocksize(blocksize),
    Indexnmame(indexname), Blockname(blockname) {
    sqrBlocksize = sqrt(Blocksize);
    index_file.open(indexname, std::ios::out | std::ios::binary | std::ios::in);
    if (!index_file.is_open()) {
      index_file.open(indexname, std::ios::out);
      index_file.close();
      index_file.open(indexname, std::ios::out | std::ios::binary | std::ios::in);
    }
    index_file.close();
    block_file.open(blockname, std::ios::out | std::ios::binary | std::ios::in);
    if (!block_file.is_open()) {
      block_file.open(blockname, std::ios::out);
      block_file.close();
      block_file.open(blockname, std::ios::out | std::ios::binary | std::ios::in);
    }
    block_file.close();
    offset = info * sizeof(int);
  }

  ~Index() {
    //析构 首先是要把所有链表存储到Index里面 然后进行析构
    block_file.close();
    Block *cur = head;
    Block *delete_p;
    //把Index文件覆写
    index_file.open(Indexnmame, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
    index_file.seekp(0, std::ios::beg);
    index_file.write(reinterpret_cast<char *>(&Blockcount), sizeof(int));
    for(int i = 1;i<info;i++) {//写入info
      index_file.write(reinterpret_cast<char *>(&info_data[i]), sizeof(int));
    }
    index_file.seekp(offset,std::ios::beg);
    for (int i = 0;i<Blockcount;i++)  {
      index_file.write(reinterpret_cast<char *>(cur), sizeof(Block));
      delete_p = cur;
      cur = cur->next;
      delete delete_p;
    }
    index_file.close();
  }

  //获得第n个info
  int getInfo(int n) {
    return info_data[n];
  }

  //修改对应位置的info
  void updateInfo(int n,int val) {
    info_data[n] = val;
  }

  //初始化 读入所有Index
  void Initialise();

  //检测是否需要合并
  void Merge(Block *, Block *);

  //检测是否需要裂块
  void Split(Block *);

  //在当前块中插入数据
  bool insertData(const char *, T value);

  //在当前块中删除块
  bool deleteData(const char *, T value);

  bool updateData(const char *,T value);
  //在当前块中寻找数据
  T findData(const char *,bool &);
};




template<class T, int info_len = 2>
class LogFile {
private:
  /* your code here */
  std::fstream file;
  std::string file_name;
  int sizeofT = sizeof(T);
  int max_size = 0;
public:
  LogFile() = default;

  LogFile(const std::string& file_name) : file_name(file_name) {}

  void initialise(const std::string &FN = "") {
    if (FN != "") file_name = FN;
    file.open(file_name, std::ios::out|std::ios::binary);
    int tmp = 0;
    for (int i = 0; i < info_len; ++i)
      file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
    file.close();
  }

  //读出第n个int的值赋给tmp，1_base
  void get_info(int &tmp, int n) {
    if (n > info_len) return;
    file.open(file_name,std::ios::binary|std::ios::in|std::ios::out);
    /* your code here */
    int t;
    file.seekg((n - 1) * sizeof(int),std::ios::beg);
    file.read(reinterpret_cast<char*>(&tmp),sizeof(int));
    file.close();
  }

  //将tmp写入第n个int的位置，1_base
  void write_info(int tmp, int n) {
    if (n > info_len) return;
    /* your code here */
    file.open(file_name,std::ios::binary|std::ios::in|std::ios::out);
    file.seekp((n - 1)*sizeof(int));
    file.write(reinterpret_cast<char *>(&tmp),sizeof(int));
    file.close();

  }

  //在文件合适位置写入类对象t，并返回写入的位置索引index
  //位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
  //位置索引index可以取为对象写入的起始位置
  int write(T &t) {
    /* your code here */
    file.open(file_name,std::ios::binary|std::ios::in|std::ios::out);
    file.seekp(0,file.end);
    int index = file.tellp();
    file.write(reinterpret_cast<char*>(&t),sizeofT);
    file.close();
    max_size++;
    return index;

  }

  //用t的值更新位置索引index对应的对象，保证调用的index都是由write函数产生
  void update(T &t, const int index) {
    /* your code here */
    file.open(file_name,std::ios::in|std::ios::binary|std::ios::out);
    file.seekp(index);
    file.write(reinterpret_cast<char*>(&t),sizeofT);
    file.close();
  }

  //读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
  void read(T &t, const int index) {

    file.open(file_name,std::ios::binary|std::ios::in);
    file.seekg(index);
    file.read(reinterpret_cast<char*>(&t),sizeofT);
    file.close();
  }

  //删除位置索引index对应的对象(不涉及空间回收时，可忽略此函数)，保证调用的index都是由write函数产生
  void Delete(int index) {
    /* your code here */
    file.open(file_name,std::ios::binary|std::ios::in|std::ios::out);
    file.seekg(0);
    int info[info_len + 2];
    for(int i = 0;i<info_len;i++) {
      file.read(reinterpret_cast<char *>(&info[i]),sizeof(int));
    }
    T buffer[max_size+2];
    T temp;
    int cur = 0;
    for(int i = 0;i<max_size;i++) {
      if(file.tellg() == index) {
        file.read(reinterpret_cast<char *>(&temp),sizeofT);
        continue;
      }
      file.read(reinterpret_cast<char *>(&buffer[cur++]),sizeofT);
    }
    file.close();
    file.open(file_name,std::ios::in|std::ios::out|std::ios::trunc);
    file.seekp(0);
    for(int i= 0;i<info_len;i++) {
      file.write(reinterpret_cast<char *>(&info[i]),sizeof(int));
    }
    for(int i = 0;i<cur;i++) {
      file.write(reinterpret_cast<char *>(&buffer[i]),sizeofT);
    }
    max_size = cur;
    file.close();
  }
};

#endif //FILE_HPP
