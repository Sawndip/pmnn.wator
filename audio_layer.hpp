#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cstdint>
#include <list>
#include <tuple>
#include <deque>
#include <mutex>
#include <condition_variable>
using namespace std;

#include "base_layer.hpp"
#pragma once


namespace WatorVapor {
    class AudioWaveLayer :public InputLayer<int16_t> {
    public:
        AudioWaveLayer();
        
        virtual int width(void);
        virtual void snapshot(void);
        virtual void fetch(T &value,int16_t &channel) {}
        
    protected:
        virtual void execBody(void);
    protected:
    private:
        void forwardOneWave(const string &path);
    private:
        const uint32_t memoryWidth_ = 48*1024 *5;
    };
    
    class Peak2PeakLayer :public HiddenLayer<int16_t> {
    public:
        Peak2PeakLayer();
    protected:
        virtual void execBody(void);
    private:
    private:
    };
}


class WatorAudioWaveL :public WatorInputL {
public:
    WatorAudioWaveL();
    
    virtual void build(void);
    virtual int width(void);
    virtual void snapshot(void);
    void setDAF(double factor);
    
    virtual void forward(void);
    virtual deque<int16_t> active(void);
    virtual deque<bool> diactive(void);
    virtual deque<int16_t> value(void);
protected:
    virtual void execBody(void);
protected:
    deque<deque<int16_t>> blobs_;

    deque<deque<int16_t>> forwordBlob_;
    mutex mtxForwordBlob_;
    condition_variable cvForwordBlob_;
    
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
    virtual deque<int16_t> value(void);
protected:
    virtual void execBody(void);
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

  virtual size_t size(void);

  uint16_t accumulate(void);
  
  int16_t at(size_t index);

protected:
  deque<int16_t> blob_;
  uint16_t max_;
  uint32_t accumulate_;
  int8_t order_;
};

class EmptySinBlob:public SinBlob {
public:
  EmptySinBlob(size_t);
  virtual size_t size(void);
private:
  EmptySinBlob();
private:
  size_t holdspace_;
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
    virtual void build(void);
    virtual void forward(void);
    virtual deque<int16_t> value(void);
    virtual void snapshot(void);

    deque<SinBlob> & valueSin();

protected:
    virtual void execBody(void);
private:
    void fillArch(int channel);
    void fillSinArch(int channel);
    void fillOrigArch(int channel);
    void fillEmptyArch(int channel);

    bool isEarArch(int channel);
    bool isPowerArch(int channel);

    void process(void);

private:
    /// 48k samplate.
    int sampleRate_ = 48 *1024;
    
    /// hearable wave max 20khz => 16k
    const int earRateMax_ = 16 * 1024;
    /// hearable wave min 20hz => 16hz
    const int earRateMin_ = 16;
    
    bool archUp_ = false;
    deque<SinBlob> archProcess_;
    deque<deque<SinBlob>> archs_;

    int dropCounter_ = 0;
    int keepCounter_ = 1;
   
    int archWidthCutMin_ = sampleRate_ / (2*earRateMax_);
    int archWidthCutMax_ = sampleRate_ / (2*earRateMin_);
};

class FullSinCurveL :public WatorHiddenL {
public:
    FullSinCurveL();
    virtual ~FullSinCurveL();
    
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
    virtual void build(void);
    virtual void forward(void);
    virtual deque<int16_t> value(void);
    virtual void snapshot(void);
protected:
    virtual void execBody(void);
private:
    void fillArch(void);
    void fillSinArch(void);
    void fillOrigArch(void);
    void fillEmptyArch(void);
    
    void process(void);

private:
    deque<deque<SinBlob>> archs_;
    int dropCounter_ = 0;
    int keepCounter_ = 1;
};

