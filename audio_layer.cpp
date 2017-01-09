#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <numeric>
#include <iomanip>
#include <thread>
using namespace std;

#include <opencv/highgui.h>
//#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include "audio_layer.hpp"
#include "base_layer.hpp"
#include "wave.hpp"



#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#define DUMP_VAR(x) {BOOST_LOG_TRIVIAL(debug) << typeid(*this).name() \
<< "::" << __func__ << ":" << __LINE__ << " " #x "=<" << x << ">" << endl;}


const unsigned long iConstWaveGraphWidth = 3600*5;

const unsigned long iConstDeActiveRateUp = 80;
const unsigned long iConstDeActiveRateDown = 70;
const double dConstDeActiveStep = 0.1;

WatorAudioWaveL::WatorAudioWaveL()
:WatorInputL() {
    DUMP_VAR(this);
}
void WatorAudioWaveL::execBody(void) {
    DUMP_VAR(this);
}

void WatorAudioWaveL::forwardOneWave(const string &path){
    auto waves = readWave(path);
    DUMP_VAR(waves.size());
    if(waves.empty()) {
        return ;
    }
    
    DUMP_VAR(waves.begin()->size());
    for(int i = 0;i < waves.begin()->size() -1;i++) {
	deque<int16_t> blob;
	for(auto &wave:waves) {
		blob.push_back(wave.at(i));
		if(std::abs(wave.at(i)) > maxHeight_) {
		    maxHeight_ = std::abs(wave.at(i));
		}
	}
	blobs_.push_back(blob);
	if(blobs_.size() > iMaxWaveWidth_) {
	    blobs_.pop_front();
	}
        for(auto top:top_) {
            top->forward();
        }
    }
}

void WatorAudioWaveL::forward(void){
    for(int i = 0;i < 1;i++) {
        this->forwardOneWave("./waveform/myRecording06.wav");
    }
}


deque<int16_t> WatorAudioWaveL::active(void) {
    if(blobs_.size()>1) {
	deque<int16_t> value;
	for(auto blob :blobs_){
        	auto it = blob.rbegin();
        	value.push_back( (*it)*2/3 + *(++it)/3);
	}
	return value;
    }
    return {};
}
deque<bool> WatorAudioWaveL::diactive(void) {
#if 0
    allNumber_++;
    if(blob_.size()>1) {
        auto it = blob_.rbegin();
        double diff = *it;
        //DUMP_VAR(*it);
        diff -= *(it+1);
        //DUMP_VAR(*it);
        double diffABS = std::abs(diff);
        double sum = std::abs(*it);
        sum += std::abs(*(it+1));
        sum += 1.0;
        //DUMP_VAR(diff);
        //DUMP_VAR(diffABS);
#if 0
        double diffAdj = (double)diffABS/(sum);
#else
        double diffAdj = (double)diffABS*sum;
#endif
        double diffAve = diffAdj/(double)iMaxWaveWidth_;
        diffs_.push_back(diffAve);
        //DUMP_VAR(diffs_.size());
        //DUMP_VAR(iMaxWaveWidth_);
        dThreshold_ += diffAve;
        if(diffs_.size() >iMaxWaveWidth_) {
            dThreshold_ -= diffs_.front();
            diffs_.pop_front();
        }
        if(dThreshold_ < 0) {
            //DUMP_VAR(diffABS);
            //DUMP_VAR(diffAdj);
            //DUMP_VAR(dThreshold_);
        }
        //DUMP_VAR(diffAdj);
        //DUMP_VAR(dThreshold_);
        if(diffAdj > dThreshold_ *  dDeativeFactor_) {
            //DUMP_VAR(diffAdj);
            //DUMP_VAR(dThreshold_);
            intermediate_.push_back(false);
            if(intermediate_.size() >iMaxWaveWidth_) {
                intermediate_.pop_front();
            }
            deActiveNumber_++;
            this->adjustRate();
            return false;
        }
    }
    intermediate_.push_back(true);
    if(intermediate_.size() >iMaxWaveWidth_) {
        intermediate_.pop_front();
    }
    this->adjustRate();
    return true;
#endif
    return {};
}

deque<int16_t> WatorAudioWaveL::value(void) {
    return {};
}

void WatorAudioWaveL::adjustRate(void) {
    /*
     if(allNumber_ < iMaxWaveWidth_) {
     return;
     }
     int rate = 100*deActiveNumber_ /allNumber_;
     DUMP_VAR(rate);
     if( rate > iConstDeActiveRateUp) {
     dDeativeFactor_ += dConstDeActiveStep;
     } else if( rate < iConstDeActiveRateDown){
     dDeativeFactor_ -= dConstDeActiveStep;
     }
     */
}


void WatorAudioWaveL::setDAF(double factor){
    dDeativeFactor_ = factor;
}

int WatorAudioWaveL::width(void) {
    //DUMP_VAR(iMaxWaveWidth_);
    return iMaxWaveWidth_;
}

void WatorAudioWaveL::build(void) {
    cout << name_ << ":m[" << iMaxWaveWidth_ << "]:i[" << interNumber_ << "]" << endl;
    DUMP_VAR(name_);
    DUMP_VAR(name_);
    for(auto top:top_) {
        top->build();
    }
    WatorBaseL::build();
}
void WatorAudioWaveL::snapshot(void){
    if(maxHeight_ <= 0) {
        return;
    }
    DUMP_VAR(name_);
    DUMP_VAR(maxHeight_);
    DUMP_VAR(blobs_.size());
    int heightLow = 256;
    int heightFull = 256 *8;
    int heightDiff = 100;
    int width = iConstWaveGraphWidth;
    for(const auto & blob :blobs_){
        if(width > blob.size()){
            width = blob.size();
        }
    }
    int onechannelHeight = heightLow + heightFull + heightDiff;
    cv::Mat mat( blobs_.size() *onechannelHeight ,width,CV_8UC3,cv::Scalar(255,255,255));
    for(int ch = 0;ch < blobs_.size();ch++){
      auto blob = blobs_.at(ch);
      for(int i = 0;i < blob.size();i++) {
        auto slip = i / iConstWaveGraphWidth;
        if(i % iConstWaveGraphWidth == 0){
          //mat = white;
          mat = cv::Scalar(255,255,255);
        }
        if(i % iConstWaveGraphWidth == iConstWaveGraphWidth-1 || i == blob.size() -1) {
          string path = "dump/wave.out.";
          std::stringstream ss;
          ss << name_;
          ss << ".";
          ss << std::setfill('0') << std::setw(8) << slip;
          path += ss.str();
          path += ".png";
          cv::imwrite( path,mat);
        }
        int16_t baseLineWavFull = heightFull/2;
        int16_t baseLineWavLow =  heightFull + heightLow/2;
        int16_t val = blob.at(i);
        int16_t yWaveFull = (int16_t)( val*(heightFull) / (2*maxHeight_)) + baseLineWavFull;;

        int16_t yWaveLow = heightFull;
        if(std::abs(val) < heightLow/2) {
          yWaveLow = val + baseLineWavLow;
        }

        int x = i % iConstWaveGraphWidth;
        cv::line(mat,cv::Point(x,yWaveFull),cv::Point(x,baseLineWavFull), cv::Scalar(0,0,255));

        cv::line(mat,cv::Point(x,yWaveLow),cv::Point(x,baseLineWavLow), cv::Scalar(0,0,255));

        mat.at<cv::Vec3b>(baseLineWavFull, x) = cv::Vec3b(0,0,0);

        mat.at<cv::Vec3b>(baseLineWavLow, x) = cv::Vec3b(0,0,0);
      }
    }
    for(auto top:top_) {
        top->snapshot();
    }
}

WatorAudioWave2L::WatorAudioWave2L()
:WatorAudioWaveL(){
    DUMP_VAR(this);
}
WatorAudioWave2L::~WatorAudioWave2L() {
}
void WatorAudioWave2L::execBody(void) {
    DUMP_VAR(this);
    DUMP_VAR(std::this_thread::get_id());
    for(int i = 0;i < 1;i++) {
        this->forwardOneWave("./waveform/myRecording06.wav");
    }
    // wait for buffer out.
    while(true) {
        {
            lock_guard<std::mutex> scopLock(mtxForwordBlob_);
            if(forwordBlob_.empty()) {
                break;
            }
        }
        //DUMP_VAR(forwordBlob_.size());
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        cvForwordBlob_.notify_one();
    }
    DUMP_VAR(isRunning);
    isRunning = false;
    DUMP_VAR(isRunning);
}

void WatorAudioWave2L::forward(void) {
    for(auto top:top_) {
        top->forward();
    }
}

void WatorAudioWave2L::forwardOneWave(const string &path){
    auto waves = readWave(path);
    DUMP_VAR(waves.size());
    if(waves.empty()) {
        return ;
    }
    DUMP_VAR(waves.begin()->size());
    for(int i = 0;i < waves.begin()->size() -1;i++) {
        // new data
	deque<int16_t> blob;
	for(auto &wave :waves) {
		blob.push_back(wave.at(i));
		if(std::abs(wave.at(i)) > maxHeight_) {
		    maxHeight_ = std::abs(wave.at(i));
		}
	}
        if(blobs_.size() > iMaxWaveWidth_) {
            blobs_.pop_front();
        }
        blobs_.push_back(blob);

        lock_guard<std::mutex> scopLock(mtxForwordBlob_);
        forwordBlob_.push_back(blob);
        cvForwordBlob_.notify_one();
        this->forward();
    }
}
deque<int16_t> WatorAudioWave2L::value(void) {
//    DUMP_VAR(forwordBlob_.size());
    unique_lock<std::mutex> ulock(mtxForwordBlob_);
    if(forwordBlob_.empty()) {
        cvForwordBlob_.wait(ulock);
    }
    auto value = forwordBlob_.front();
    forwordBlob_.pop_front();
    return value;
}


SinBlob::SinBlob()
:blob_({})
,max_(0)
,accumulate_(0)
,order_(-1) {
}

void SinBlob::push(int16_t val) {
  blob_.push_back(val);
  uint16_t absVal = std::abs(val);
  if( absVal> max_) {
    max_ = absVal;
  }
  accumulate_ += absVal;
  if(order_ == -1) {
    if(val < 0) {
      order_ = 0;
    } else if(val > 0) {
      order_ = 1;
    } else {
      // continue;
    }
  }
}
void SinBlob::clear() {
  blob_.clear();
  max_ = 0;
  accumulate_ = 0;
  order_ = -1;
}

uint16_t SinBlob::max(void) {
  return max_;
}


size_t SinBlob::size(void) {
  return blob_.size();
}

uint16_t SinBlob::accumulate(void) {
  return accumulate_;
}
  
int16_t SinBlob::at(size_t index) {
  return blob_.at(index);
}
EmptySinBlob::EmptySinBlob(size_t size)
: SinBlob()
, holdspace_(size){
}
size_t EmptySinBlob::size(void) {
  return holdspace_;
}


HalfSinCurveL::HalfSinCurveL()
:WatorHiddenL(){
    DUMP_VAR(name_);
    DUMP_VAR(archWidthCutMin_);
    DUMP_VAR(archWidthCutMax_);
    DUMP_VAR(this);
}
HalfSinCurveL::~HalfSinCurveL() {
}
void HalfSinCurveL::execBody(void) {
    DUMP_VAR(name_);
    DUMP_VAR(std::this_thread::get_id());
    while(isRunning) {
      this->process();
    }
    DUMP_VAR(name_);
    DUMP_VAR(std::this_thread::get_id());
}

void HalfSinCurveL::build(void)
{
    auto buttom = buttom_.at(0);
    auto width = buttom->width();
    //DUMP_VAR(width);
    //DUMP_VAR(step_);
    iMaxWaveWidth_ = width;
    //DUMP_VAR(iMaxWaveWidth_);
    interNumber_ = iMaxWaveWidth_;
    
    for(int i = 0 ;i < depth_ ;i++) {
        cout << "  ";
    }
    cout << name_ << ":m[" << iMaxWaveWidth_ << "]:i[" << interNumber_ << "]" << endl;
    for(auto top:top_) {
        top->build();
    }
    WatorBaseL::build();
}

void HalfSinCurveL::forward(void) {
    for(auto top:top_) {
        top->forward();
    }
}

void HalfSinCurveL::process(void) {
    //cout << name_ << endl;
    auto buttom = buttom_.at(0);
    auto values = buttom->value();
    for(auto value:values) {
    if(value > 0) {
        if(archUp_ == false) {
            this->fillArch();
        }
        archUp_ = true;
    } else if(value < 0){
        if(archUp_ == true) {
            this->fillArch();
        }
        archUp_ = false;
    } else {
        /// zero continue;
    }
    arch_.push(value);

    uint16_t absVal = std::abs(value);
    if(absVal > maxHeight_) {
        maxHeight_ = absVal;
    }
    }
    for(auto top:top_) {
        top->forward();
    }
}

const double dConstPI = std::acos(-1.0);
const int iConstArchPowerThrelod = 128;

void HalfSinCurveL::fillSinArch(void) {
    // forwod data to next layer.
    //DUMP_VAR(arch_.size());
    for(int i = 0 ;i < arch_.size();i++){
        int16_t value = arch_.max() * std::sin( (double) i * dConstPI/(double)(arch_.size() -1) );
        //DUMP_VAR(value);
        if(archUp_) {
            blob_.push_back(value);
        } else {
            blob_.push_back(0-value);
        }
        if(blob_.size() > iMaxWaveWidth_) {
            blob_.pop_front();
        }
    }
    //DUMP_VAR(blob_.size());
    archs_.push_back(arch_);
}

void HalfSinCurveL::fillOrigArch(void) {
    for(int i = 0 ;i < arch_.size();i++){
        int16_t value = arch_.at(i);
        //DUMP_VAR(value);
        blob_.push_back(value);
        if(blob_.size() > iMaxWaveWidth_) {
            blob_.pop_front();
        }
    }
    //DUMP_VAR(blob_.size());
    archs_.push_back(arch_);
}

void HalfSinCurveL::fillEmptyArch(void) {
    for(int i = 0 ;i < arch_.size();i++) {
        blob_.push_back(0);
        if(blob_.size() > iMaxWaveWidth_) {
            blob_.pop_front();
        }
    }
    EmptySinBlob empty(arch_.size());
    archs_.push_back(empty);
}

bool HalfSinCurveL::isEarArch(void) {
    if(arch_.size() > archWidthCutMin_ && arch_.size() < archWidthCutMax_) {
        return true;
    }
    return false;
}
bool HalfSinCurveL::isPowerArch(void) {
    if(arch_.accumulate() > iConstArchPowerThrelod) {
        return true;
    }
    return false;
}


void HalfSinCurveL::fillArch(void) {
    //DUMP_VAR(arch_.size());
    if(this->isEarArch() && this->isPowerArch()) {
        //this->fillSinArch();
        this->fillOrigArch();
        keepCounter_++;
    } else {
        this->fillEmptyArch();
        dropCounter_++;
    }
    arch_.clear();
}


void HalfSinCurveL::snapshot(void){
    WatorHiddenL::snapshot();
    writeWave("dump/"+name_ + ".wav",blob_);
    DUMP_VAR(dropCounter_);
    DUMP_VAR(keepCounter_);
    DUMP_VAR(dropCounter_+keepCounter_);
    DUMP_VAR(100*dropCounter_/(dropCounter_+keepCounter_));
}

int16_t HalfSinCurveL::value(void) {
    return 0;
}

SinBlob & HalfSinCurveL::valueSin() {
    return archs_.front();
}


FullSinCurveL::FullSinCurveL()
:WatorHiddenL(){
    DUMP_VAR(name_);
    DUMP_VAR(this);
}
FullSinCurveL::~FullSinCurveL() {
}

void FullSinCurveL::execBody(void) {
    DUMP_VAR(name_);
    DUMP_VAR(std::this_thread::get_id());
    while(isRunning) {
      this->process();
    }
}

void FullSinCurveL::build(void)
{
    auto buttom = buttom_.at(0);
    auto width = buttom->width();
    //DUMP_VAR(width);
    //DUMP_VAR(step_);
    iMaxWaveWidth_ = width;
    //DUMP_VAR(iMaxWaveWidth_);
    interNumber_ = iMaxWaveWidth_;
    
    for(int i = 0 ;i < depth_ ;i++) {
        cout << "  ";
    }
    cout << name_ << ":m[" << iMaxWaveWidth_ << "]:i[" << interNumber_ << "]" << endl;
    for(auto top:top_) {
        top->build();
    }
    WatorBaseL::build();
}

void FullSinCurveL::forward(void) {
    for(auto top:top_) {
        top->forward();
    }
}

void FullSinCurveL::process(void) {
    //cout << name_ << endl;
/*
    auto buttom = buttom_.at(0);
    auto halfSin = dynamic_pointer_cast<HalfSinCurveL>(buttom);
    if(halfSin) {
	auto value = halfSin->valueSin();
    }
*/
    for(auto top:top_) {
        top->forward();
    }
}

int16_t FullSinCurveL::value(void) {
    return 0;
}



void FullSinCurveL::snapshot(void){
    WatorHiddenL::snapshot();
    writeWave("dump/"+name_ + ".wav",blob_);
    DUMP_VAR(dropCounter_);
    DUMP_VAR(keepCounter_);
    DUMP_VAR(dropCounter_+keepCounter_);
    DUMP_VAR(100*dropCounter_/(dropCounter_+keepCounter_));
}

