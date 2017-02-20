#include <deque>
#include <thread>
#include <condition_variable>
#include <mutex>
using namespace std;

#pragma once

namespace WatorVapor {
    template < typename T > class Blob {
    public:
        explicit Blob(condition_variable &cond_var);
        void push(T val);
        void wait();
    protected:
    private:
        Blob();
    protected:
        deque<T> memory_;
        mutex mtx_;
        condition_variable &cond_var_;
    };
}
