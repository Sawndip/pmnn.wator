#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cstdint>
#include <list>
#include <tuple>
#include <deque>
using namespace std;

#include "base_layer.hpp"
#pragma once




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

class SinBlob {
public:
  SinBlob();
  void push(int16_t val);
  void clear();

  uint16_t max(void);
  void max(uint16_t val);

  size_t size(void);

  uint16_t accumulate(void);
  
  int16_t at(size_t index);

private:
  deque<int16_t> blob_;
  uint16_t max_;
  uint32_t accumulate_;
  int8_t order_;
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
    virtual void snapshot(void);
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
    SinBlob arch_;
    deque<SinBlob> archs_;
   
    int archWidthCutMin_ = sampleRate_ / (2*earRateMax_);
    int archWidthCutMax_ = sampleRate_ / (2*earRateMin_);
};


