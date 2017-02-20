#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <numeric>
#include <iomanip>
#include <thread>
using namespace std;

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#define DUMP_VAR(x) {BOOST_LOG_TRIVIAL(debug) << typeid(*this).name() \
<< "::" << __func__ << ":" << __LINE__ << " " #x "=<" << x << ">" << endl;}


#include "audio_layer.hpp"
#include "base_layer.hpp"
#include "wave.hpp"

using namespace WatorVapor;
AudioWaveLayer::AudioWaveLayer() {
}

int AudioWaveLayer::width(void) {
    return 0;
}

void AudioWaveLayer::snapshot(void) {
}

void AudioWaveLayer::forward(void) {
}

void AudioWaveLayer::execBody(void) {
    DUMP_VAR(name_);
    DUMP_VAR(t_.get_id());
}
