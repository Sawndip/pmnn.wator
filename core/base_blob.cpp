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

#define TRACE_VAR(x)



using namespace WatorVapor;

template < typename T > Blob<T>::Blob(condition_variable &cond_var,mutex &mtx,uint32_t width)
:cond_var_(cond_var)
,mtx_(mtx)
,width_(width) {
    
}

template < typename T > void Blob<T>::push(T val) {
    unique_lock<mutex> lock(mtx_);
    memory_.push_back(val);
    if(memory_.size() > width_) {
        memory_.pop_front();
    }
    bufferNext_.push_back(val);
    if(bufferNext_.size() > width_) {
        DUMP_VAR(bufferNext_.size());
        DUMP_VAR(width_);
        DUMP_VAR(val);
        unique_lock<mutex> empty(empty_mtx_);
        empty_cond_var_.wait(empty);
    } else {
        TRACE_VAR(bufferNext_.size());
    }
    cond_var_.notify_all();
}

template < typename T > T Blob<T>::wait(void) {
    unique_lock<mutex> lock(mtx_);
    if(bufferNext_.empty()) {
        cond_var_.wait(lock);
    }
    T ret = bufferNext_.front();
    bufferNext_.pop_front();
    empty_cond_var_.notify_all();
    return ret;
}


template  Blob<int16_t>::Blob(condition_variable &cond_var,mutex &mtx,uint32_t width);
template  void Blob<int16_t>::push(int16_t val);
template  int16_t Blob<int16_t>::wait(void);
