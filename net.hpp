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
#include "base_layer.hpp"

namespace WatorVapor {
    class Net {
    public:
        Net(BaseLayerPtr entry);
        void build(void);
        
        void train(void);
        void test(void);
        
        void snapshot(void);
    private:
        BaseLayerPtr entry_;
    };
}

class WatorNet {
public:
  WatorNet(WatorBaseLPtr entry);
  void build(void);

  void train(void);
  void test(void);

  void snapshot(void);
private:
  WatorBaseLPtr entry_;
};

