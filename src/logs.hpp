//
// Created by Freewings on 2024/12/15.
//

#ifndef LOGS_HPP
#define LOGS_HPP
#include "file.hpp"
class logs {
public:
  virtual void save() ;
  virtual void read() ;
  virtual void print() ;
};

class  Profit :public logs{
private:

public:
  void save() override ;
  void read() override;
  void print() override;
};


class  Operator :public logs{
private:
public:
  void save() override ;
  void read() override;
  void print() override;

};

class  SystemLog :public logs{
private:

public:
  void save() override ;
  void read() override;
  void print() override;
};


#endif //LOGS_HPP
