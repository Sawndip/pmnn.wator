#include <iostream>
#include <vector>
#include <memory>
#include <string>
using namespace std;

#pragma once

class WatorBaseL {
public:
  virtual void layout();
  void name(const string &name);
  int depth(void);
protected:
  WatorBaseL();
protected:
  string name_;
  int depth_ = 0;
};

typedef shared_ptr<WatorBaseL> WatorBaseLPtr;


class WatorInputL :public WatorBaseL {
public:
  WatorInputL();
  void addTop(WatorBaseLPtr top);
  virtual void layout();
protected:
  vector<WatorBaseLPtr> top_;
};



class WatorOutputL :public WatorBaseL {
public:
  WatorOutputL();
  void addButtom(WatorBaseLPtr buttom);
  virtual void layout();
protected:
  vector<WatorBaseLPtr> buttom_;
};


class WatorHiddenL :public WatorBaseL {
public:
  WatorHiddenL();
  void addTop(WatorBaseLPtr top);
  void addButtom(WatorBaseLPtr buttom);
  virtual void layout();
protected:
  vector<WatorBaseLPtr> top_;
  vector<WatorBaseLPtr> buttom_;
};


class WatorNet {
public:
  WatorNet(WatorBaseLPtr entry);
  void train();
  void layout();
private:
  WatorBaseLPtr entry_;
};

