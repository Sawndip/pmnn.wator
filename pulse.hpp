#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cstdint>
#include <list>
#include <tuple>
using namespace std;

#pragma once

class WatorBaseL {
public:
  
  virtual void layout(void);
  virtual int16_t width(void);
  void name(const string &name);
  int depth(void);
  
  virtual void forward(void);
  virtual int16_t active(void);
  virtual int16_t diactive(void);

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
  virtual void layout(void);
  virtual int16_t width(void);

  virtual void forward(void);
  virtual int16_t active(void);
  virtual int16_t diactive(void);
protected:
  vector<WatorBaseLPtr> top_;
  int16_t iThreshold_ = 0;
private:
};


class WatorAudioWaveL :public WatorInputL {
public:
  WatorAudioWaveL();

  virtual int16_t width(void);

  virtual void forward(void);
  virtual int16_t active(void);
  virtual int16_t diactive(void);
protected:
  list<int16_t> blob_;
  int iMaxWaveWidth_ = 1024*1024;
private:
};


class WatorOutputL :public WatorBaseL {
public:
  WatorOutputL();
  void addButtom(WatorBaseLPtr buttom);
  virtual void layout(void);
protected:
  vector<WatorBaseLPtr> buttom_;
};


class WatorHiddenL :public WatorBaseL {
public:
  WatorHiddenL();
  void addTop(WatorBaseLPtr top);
  void addButtom(WatorBaseLPtr buttom);
  virtual void layout(void);
  virtual void forward(void);

  virtual int16_t active(void);
  virtual int16_t diactive(void);

protected:
  vector<WatorBaseLPtr> top_;
  vector<WatorBaseLPtr> buttom_;
  
  list<int16_t> blob_;
  int iMaxWaveWidth_;
  float active_ = 1.0;
  float disactive_ = - 1.0;
  int step_ = 2;
  vector<int16_t> stepBuff_;

  int16_t iThreshold_ = 0;
};


class WatorNet {
public:
  WatorNet(WatorBaseLPtr entry);
  void train(void);
  void layout(void);
private:
  WatorBaseLPtr entry_;
};

