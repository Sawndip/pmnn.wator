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


void AudioWaveLayer::execBody(void) {
    DUMP_VAR(name_);
    DUMP_VAR(t_.get_id());
    for(int i = 0;i < 1;i++) {
        this->forwardOneWave("./waveform/myRecording06.wav");
    }
    isRunning = false;
    DUMP_VAR(t_.get_id());
}

void AudioWaveLayer::forwardOneWave(const string &path){
    auto waves = readWave(path);
    DUMP_VAR(waves.size());
    if(waves.empty()) {
        return ;
    }
    int32_t minWave = waves.begin()->size();
    for(int channel = 0; channel <waves.size();channel++ ) {
        auto &wave = waves.at(channel);
        if(blobs_.size() <= channel) {
            auto blob = std::make_shared<Blob<int16_t>>(cond_var_,mtx_,memoryWidth_);
            blobs_.push_back(blob);
        }
        if(minWave > wave.size()) {
            minWave = wave.size();
        }
    }
    DUMP_VAR(minWave);
    for(int frame = 0 ; frame < minWave; frame++ ) {
        int channel = 0;
        for( auto &wave :waves) {
            blobs_.at(channel++)->push(wave.at(frame));
        }
    }
}





Peak2PeakLayer::Peak2PeakLayer() {
}

void Peak2PeakLayer::execBody(void) {
    DUMP_VAR(name_);
    DUMP_VAR(t_.get_id());
    while(isRunning) {
        for(auto &buttom:buttom_){
            //DUMP_VAR(isRunning);
            int16_t value;
            int16_t channel;
            //buttom->fetch(value,channel);
        }
    }
    DUMP_VAR(name_);
    DUMP_VAR(t_.get_id());
}
