#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <numeric>
#include <iomanip>
using namespace std;


#include "net.hpp"
#include "base_layer.hpp"


#define DUMP_VAR(x) {cout << __LINE__ << ":" #x "=<" << x << ">" << endl;}


WatorNet::WatorNet(WatorBaseLPtr entry)
  :entry_(entry) {
}
void WatorNet::train(void) {
    entry_->forward();
}
void WatorNet::layout(void) {
  entry_->layout();
}

void WatorNet::snapshot(void) {
  entry_->snapshot();
}


