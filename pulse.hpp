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
  virtual int16_t value(void);
    
    

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
    virtual int16_t value(void);
protected:
  vector<WatorBaseLPtr> top_;
  double dThreshold_ = 0.0;
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
  virtual void addTop(WatorBaseLPtr top);
  virtual void addButtom(WatorBaseLPtr buttom);

  virtual void layout(void);
  virtual void forward(void);
    virtual int width(void);
    virtual void snapshot(void);

  virtual int16_t active(void);
  virtual bool diactive(void);
    virtual int16_t value(void);
  
    virtual void setDAF(double factor);

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
  int stepBuffInsertNumber_ = 0;

  deque<bool> intermediate_;
  deque<uint16_t> diffs_;
  double dThreshold_ = 0.0;
  double dDeativeFactor_ = 1.0;
    
  // dump
  uint16_t maxHeight_ = 0;
};

class WatorAudioWaveL :public WatorInputL {
public:
    WatorAudioWaveL();
    
    virtual void layout(void);
    virtual int width(void);
    virtual void snapshot(void);
    void setDAF(double factor);
    
    virtual void forward(void);
    virtual int16_t active(void);
    virtual bool diactive(void);
    virtual int16_t value(void);
protected:
    deque<int16_t> blob_;
    //int iMaxWaveWidth_ = 100*48*1024;
    int iMaxWaveWidth_ = 10*48*1024;
    int interNumber_ = iMaxWaveWidth_/iInterActiveRateReciprocal;
    deque<uint16_t> diffs_;
    deque<bool> intermediate_;
    double dThreshold_ = 0.0;
    double dDeativeFactor_ = 1.0;
    
    int deActiveNumber_ = 1;
    int allNumber_ = 1;
    
    // dump
    uint16_t maxHeight_ = 0;
private:
    void forwardOneWave(const string &path);
    void adjustRate(void);
private:
};

class WatorAudioWave2L :public WatorAudioWaveL {
public:
    WatorAudioWave2L();
    virtual ~WatorAudioWave2L();
    virtual void forward(void);
    virtual int16_t value(void);
private:
    void forwardOneWave(const string &path);
};


class HalfSinCurveL :public WatorHiddenL {
public:
    HalfSinCurveL();
    virtual ~HalfSinCurveL();

    /*
    
    virtual void addTop(WatorBaseLPtr top);
    virtual void addButtom(WatorBaseLPtr buttom);
    virtual void layout(void);
    virtual void forward(void);
    virtual int width(void);
    virtual void snapshot(void);
    
    virtual int16_t active(void);
    virtual bool diactive(void);
    
    virtual void setDAF(double factor);
*/
    virtual void layout(void);
    virtual void forward(void);
    virtual int16_t value(void);
private:
    void changeArch(void);
    void sinArch(void);
private:
    /// 48k samplate.
    int sampleRate_ = 48 *1024;
    
    /// hearable wave max 20khz => 16k
    const int earRateMax_ = 8 * 1024;
    /// hearable wave min 20hz => 32hz
    const int earRateMin_ = 32;
    
    bool archUp_ = false;
    deque<int16_t> arch_;
    uint16_t archMax_ = 0;
    
    int archWidthCutMin_ = sampleRate_ / (2*earRateMax_);
    int archWidthCutMax_ = sampleRate_ / (2*earRateMin_);
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

