#include <deque>
#include <thread>
#include <atomic>
using namespace std;

#pragma once


namespace WatorVapor {
    template < typename T > class Blob {
    public:
        Blob();
        void push(T val);
    protected:
    protected:
        deque<T> memory_;
    };
}
