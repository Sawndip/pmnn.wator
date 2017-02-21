#include <deque>
#include <thread>
#include <cinttypes>
#include <condition_variable>
#include <mutex>
using namespace std;

#pragma once

namespace WatorVapor {
    template < typename T > class Blob {
    public:
        explicit Blob(condition_variable &cond_var,mutex &mtx,uint32_t width);
        void push(T val);
        T wait(void);
    protected:
    private:
        Blob();
    protected:
        deque<T> memory_;
        deque<T> bufferNext_;
        uint32_t width_;
        condition_variable &cond_var_;
        mutex &mtx_;
        condition_variable empty_cond_var_;
        mutex empty_mtx_;
    };
}
