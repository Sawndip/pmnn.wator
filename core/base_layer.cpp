#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <numeric>
#include <iomanip>
#include <thread>
using namespace std;

#include "base_layer.hpp"

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#define DUMP_VAR(x) {BOOST_LOG_TRIVIAL(debug) << typeid(*this).name() \
<< "::" << __func__ << ":" << __LINE__ << " " #x "=<" << x << ">" << endl;}



using namespace WatorVapor;

atomic_bool BaseLayer::isRunning(true);
BaseLayer::BaseLayer() {
    DUMP_VAR(this);
}
void BaseLayer::operator()() {
    this->execBody();
}
void BaseLayer::build(void) {
    auto bindOperation = std::bind(&BaseLayer::operator(),shared_from_this());
    std::thread t(bindOperation);
    t_ = std::move(t);
    DUMP_VAR(t.get_id());
}
void BaseLayer::wait(void) {
    DUMP_VAR(t_.get_id());
    if(t_.joinable()) {
        t_.join();
    }
    DUMP_VAR(t_.get_id());
}
int BaseLayer::width(void) {
    return 0;
}
void BaseLayer::snapshot(void) {
}
void BaseLayer::name(const string &name) {
    name_ = name;
}
int BaseLayer::depth(void) {
    return depth_;
}

void BaseLayer::execBody(void) {
    DUMP_VAR(t_.get_id());
}
