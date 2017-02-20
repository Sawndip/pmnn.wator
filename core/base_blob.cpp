#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <numeric>
#include <iomanip>
#include <thread>
using namespace std;

#include "base_blob.hpp"

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#define DUMP_VAR(x) {BOOST_LOG_TRIVIAL(debug) << typeid(*this).name() \
<< "::" << __func__ << ":" << __LINE__ << " " #x "=<" << x << ">" << endl;}



using namespace WatorVapor;

template < typename T > Blob<T>::Blob(condition_variable &cond_var)
:cond_var_(cond_var){
}

template < typename T > void Blob<T>::push(T val) {
    unique_lock<mutex> lock(mtx_);
    memory_.push_back(val);
    cond_var_.notify_all();
}

template < typename T > void Blob<T>::wait() {
    unique_lock<mutex> lock(mtx_);
    cond_var_.wait(lock);
}


template  Blob<int16_t>::Blob(condition_variable &cond_var);
template  void Blob<int16_t>::push(int16_t val);
