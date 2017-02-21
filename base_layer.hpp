#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cstdint>
#include <list>
#include <tuple>
#include <deque>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <mutex>
using namespace std;

#include "base_blob.hpp"
#pragma once

static const int iInterActiveRateReciprocal = 4;

namespace WatorVapor {
    class BaseLayer:public std::enable_shared_from_this<BaseLayer> {
    public:
        void operator()();
        virtual void build(void);
        virtual void wait(void);
        virtual int width(void);
        virtual void snapshot(void);
        void name(const string &name);
        int depth(void);
    protected:
        BaseLayer();
        virtual void execBody(void);
    protected:
        string name_ = "";
        int16_t depth_ = 0;
        std::thread t_;
        condition_variable cond_var_;
        mutex mtx_;
    protected:
        static atomic_bool isRunning;
        
    };
    typedef shared_ptr<BaseLayer> BaseLayerPtr;

    template < typename T > class InputLayer :public BaseLayer {
    public:
        InputLayer() {}
        void addTop(BaseLayerPtr top) {
            top_.push_back(top);
        }
        virtual void build(void) {
            BaseLayer::build();
            for(auto & top : top_) {
                top->build();
            }
        }
    protected:
        virtual void execBody(void) {}
    protected:
        vector<BaseLayerPtr> top_;
        deque<shared_ptr<Blob<T>>> blobs_;
    private:
    };
    class OutputLayer :public BaseLayer {
    public:
        OutputLayer() {}
        virtual void build(void) {}
        virtual void wait(void) {}
        
        void addButtom(BaseLayerPtr buttom) {
            buttom_.push_back(buttom);
        }
        
    protected:
        virtual void execBody(void) {}
    protected:
        vector<BaseLayerPtr> buttom_;
    };
    
    template < typename T > class HiddenLayer :public BaseLayer {
    public:
        HiddenLayer() {}
        virtual void build(void) {
            BaseLayer::build();
            for(auto & top : top_) {
                top->build();
            }
        }
        virtual void wait(void) {}
        
        virtual void addTop(BaseLayerPtr top) {
            top_.push_back(top);
        }
        virtual void addButtom(BaseLayerPtr buttom) {
            buttom_.push_back(buttom);
        }
    protected:
        virtual void execBody(void) {}
    protected:
        vector<BaseLayerPtr> top_;
        vector<BaseLayerPtr> buttom_;
        
        deque<shared_ptr<Blob<T>>> blobs_;
    };
}





class WatorBaseL:public std::enable_shared_from_this<WatorBaseL> {
public:
  void operator()();
  
  virtual void build(void);
  virtual void wait(void);

  virtual int width(void);
  virtual void snapshot(void);
  
  void name(const string &name);
  int depth(void);
  
  virtual void forward(void);

  virtual deque<int16_t> active(void);
  virtual deque<bool> diactive(void);
  virtual deque<int16_t> value(void);
    

protected:
  WatorBaseL();
  virtual void execBody(void);


protected:
  string name_;
  int16_t depth_ = 0;
  std::thread t_;


protected:
  static atomic_bool isRunning;
  
};

typedef shared_ptr<WatorBaseL> WatorBaseLPtr;


class WatorInputL :public WatorBaseL {
public:
  WatorInputL();

  virtual void build(void);
  virtual void wait(void);

  void addTop(WatorBaseLPtr top);

  virtual int width(void);
  virtual void snapshot(void);

  virtual void forward(void);
  virtual deque<int16_t> active(void);
  virtual deque<bool> diactive(void);
  virtual deque<int16_t> value(void);
    
protected:
    virtual void execBody(void);
protected:
  vector<WatorBaseLPtr> top_;
  double dThreshold_ = 0.0;
private:
};


class WatorOutputL :public WatorBaseL {
public:
  WatorOutputL();

  virtual void build(void);
  virtual void wait(void);

  void addButtom(WatorBaseLPtr buttom);
    
    
protected:
    virtual void execBody(void);
protected:
  vector<WatorBaseLPtr> buttom_;
};


class WatorHiddenL :public WatorBaseL {
public:
  WatorHiddenL();
  virtual void build(void);
  virtual void wait(void);

  virtual void addTop(WatorBaseLPtr top);
  virtual void addButtom(WatorBaseLPtr buttom);

  virtual void forward(void);
  virtual int width(void);
  virtual void snapshot(void);

  virtual deque<int16_t> active(void);
  virtual deque<bool> diactive(void);
  virtual deque<int16_t> value(void);
  
  virtual void setDAF(double factor);

    
protected:
    virtual void execBody(void);
protected:
  vector<WatorBaseLPtr> top_;
  vector<WatorBaseLPtr> buttom_;
  
  deque<deque<int16_t>> blobs_;
  int iMaxWaveWidth_;
  int interNumber_;
  
  float active_ = 1.0;
  float disactive_ = - 1.0;
  int step_ = 2;
  vector<int16_t> stepBuff_;
  int stepBuffInsertNumber_ = 0;

  deque<deque<bool>> intermediate_;
  deque<deque<uint16_t>> diffs_;
  double dThreshold_ = 0.0;
  double dDeativeFactor_ = 1.0;
    
  // dump
  uint16_t maxHeight_ = 0;
};

