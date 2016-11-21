#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cstdint>
#include <list>
#include <tuple>
#include <deque>
using namespace std;

#pragma once

static const int iInterActiveRateReciprocal = 4;


class WatorBaseL {
public:
  
  virtual void layout(void);
  virtual int width(void);
  virtual void snapshot(void);
  
  void name(const string &name);
  int depth(void);
  
  virtual void forward(void);
  virtual int16_t active(void);
  virtual bool diactive(void);

protected:
  WatorBaseL();
protected:
  string name_;
  int16_t depth_ = 0;

  
};

typedef shared_ptr<WatorBaseL> WatorBaseLPtr;


class WatorInputL :public WatorBaseL {
public:
  WatorInputL();
  void addTop(WatorBaseLPtr top);
  virtual void layout(void);
  virtual int width(void);
    virtual void snapshot(void);

  virtual void forward(void);
  virtual int16_t active(void);
  virtual bool diactive(void);
protected:
  vector<WatorBaseLPtr> top_;
  double dThreshold_ = 0.0;
private:
};


class WatorAudioWaveL :public WatorInputL {
public:
  WatorAudioWaveL();

  virtual void layout(void);
  virtual int width(void);
    virtual void snapshot(void);

    virtual void forward(void);
  virtual int16_t active(void);
  virtual bool diactive(void);
protected:
private:
    deque<int16_t> blob_;
    int iMaxWaveWidth_ = 100*48*1024;
    //int iMaxWaveWidth_ = 5*48*1024;
    int interNumber_ = iMaxWaveWidth_/iInterActiveRateReciprocal;
    deque<uint16_t> diffs_;
    deque<bool> intermediate_;
    double dThreshold_ = 0.0;
    const double iConstDeativeFactor = 0.5;
    
    // dump
    uint16_t maxHeight_ = 0;
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
    virtual int width(void);
    virtual void snapshot(void);

  virtual int16_t active(void);
  virtual bool diactive(void);

protected:
  vector<WatorBaseLPtr> top_;
  vector<WatorBaseLPtr> buttom_;
  
  deque<int16_t> blob_;
  int iMaxWaveWidth_;
  int interNumber_;
  
  float active_ = 1.0;
  float disactive_ = - 1.0;
  int step_ = 2;
  vector<int16_t> stepBuff_;

  deque<bool> intermediate_;
  deque<uint16_t> diffs_;
  double dThreshold_ = 0.0;
  const double iConstDeativeFactor = 1.0;
    
  // dump
  uint16_t maxHeight_ = 0;
};


class WatorNet {
public:
  WatorNet(WatorBaseLPtr entry);
  void train(void);
  void layout(void);
  void snapshot(void);
private:
  WatorBaseLPtr entry_;
};

