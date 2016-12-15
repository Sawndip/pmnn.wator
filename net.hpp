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


class WatorNet {
public:
  WatorNet(WatorBaseLPtr entry);
  void train(void);
  void build(void);
  void snapshot(void);
private:
  WatorBaseLPtr entry_;
};

