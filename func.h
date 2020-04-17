#pragma once

struct Data {
  int sum = 0;
};

void Plus(Data* d);

struct Data2 {
  int sum = 0;
  void Plus();
};

struct Data3Base {
  int sum = 0;
  virtual void Plus();
};

struct Data3Base2 : public Data3Base {
  virtual void Plus();
};

struct Data3 : public Data3Base2 {
  virtual void Plus();
};

struct Data3_2 : public Data3 {
  virtual void Plus();
};

extern int sum;
void Global();

