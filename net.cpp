#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <numeric>
#include <iomanip>
#include <thread>
using namespace std;


#include "net.hpp"
#include "base_layer.hpp"

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#define DUMP_VAR(x) {BOOST_LOG_TRIVIAL(debug) << __LINE__ << ":" #x "=<" << x << ">" << endl;}



WatorNet::WatorNet(WatorBaseLPtr entry)
  :entry_(entry) {
}
void WatorNet::build(void) {
  entry_->build();
}

void WatorNet::train(void) {
  //
  entry_->wait();
}
void WatorNet::test(void) {
  //
  entry_->wait();
}



void WatorNet::snapshot(void) {
  entry_->snapshot();
}


