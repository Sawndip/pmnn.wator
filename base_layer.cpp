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


#include "base_layer.hpp"


#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#define DUMP_VAR(x) {BOOST_LOG_TRIVIAL(debug) << typeid(*this).name() \
<< "::" << __func__ << ":" << __LINE__ << " " #x "=<" << x << ">" << endl;}


const unsigned long iConstWaveGraphWidth = 3600*5;

const unsigned long iConstDeActiveRateUp = 80;
const unsigned long iConstDeActiveRateDown = 70;
const double dConstDeActiveStep = 0.1;


WatorBaseL::WatorBaseL(){
    DUMP_VAR(this);
}
void WatorBaseL::operator()() {
    this->execBody();
}
void WatorBaseL::execBody(void) {
    DUMP_VAR(this);
}

void WatorBaseL::build(void) {
  auto bindOperation = std::bind(&WatorBaseL::operator(),shared_from_this());
  std::thread t(bindOperation);
  t_ = std::move(t);
  DUMP_VAR(t.get_id());
}

void WatorBaseL::wait(void) {
  DUMP_VAR(t_.get_id());
  if(t_.joinable()) {
    t_.join();
  }
  DUMP_VAR(t_.get_id());
}

void WatorBaseL::name(const string &name) 
{
  name_ = name;
}
int WatorBaseL::depth(void){
  return depth_;
}
void WatorBaseL::forward(void){
}

deque<int16_t> WatorBaseL::active(void) {
  return {};
}
deque<bool> WatorBaseL::diactive(void) {
  return {};
}
deque<int16_t> WatorBaseL::value(void) {
  return {};
}
int WatorBaseL::width(void) {
  return 0;
}
void WatorBaseL::snapshot(void){
    
}

atomic_bool WatorBaseL::isRunning(true);


WatorInputL::WatorInputL()
:WatorBaseL(){
    DUMP_VAR(this);
}
void WatorInputL::execBody(void) {
    DUMP_VAR(this);
}

void WatorInputL::addTop (WatorBaseLPtr top) {
  top_.push_back(top);
}
void WatorInputL::build(void) {
  cout << name_ << endl;
  for(auto top:top_) {
    top->build();
  }
  WatorBaseL::build();
}
void WatorInputL::wait(void) {
  for(auto top:top_) {
    top->wait();
  }
  WatorBaseL::wait();
}

void WatorInputL::forward(void){
}
deque<int16_t> WatorInputL::active(void) {
    return {};
}
deque<bool> WatorInputL::diactive(void) {
    return {};
}
deque<int16_t> WatorInputL::value(void) {
    return {};
}

int WatorInputL::width(void) {
  return 0;
}
void WatorInputL::snapshot(void){
    
}






WatorOutputL::WatorOutputL()
  :WatorBaseL(){
      DUMP_VAR(this);
}
void WatorOutputL::execBody(void) {
    DUMP_VAR(this);
}

void WatorOutputL::addButtom(WatorBaseLPtr buttom) {
  buttom_.push_back(buttom);
  depth_ = buttom->depth() +1;
}
void WatorOutputL::build(void)
{
  for(int i = 0 ;i < depth_ ;i++) {
    cout << "  ";
  }
  cout << name_ << endl;
  WatorBaseL::build();
}

void WatorOutputL::wait(void) {
  WatorBaseL::wait();
}



WatorHiddenL::WatorHiddenL()
  :WatorBaseL(){
      DUMP_VAR(this);
}
void WatorHiddenL::execBody(void) {
    DUMP_VAR(this);
}

void WatorHiddenL::addTop(WatorBaseLPtr top)
{
  top_.push_back(top);
}
void WatorHiddenL::addButtom(WatorBaseLPtr buttom)
{
  buttom_.push_back(buttom);
  depth_ = buttom->depth() +1;
}
void WatorHiddenL::build(void)
{
    auto buttom = buttom_.at(0);
    auto width = buttom->width();
    //DUMP_VAR(width);
    //DUMP_VAR(step_);
    iMaxWaveWidth_ = width/step_;
    //DUMP_VAR(iMaxWaveWidth_);
    interNumber_ = iMaxWaveWidth_/iInterActiveRateReciprocal;

  for(int i = 0 ;i < depth_ ;i++) {
    cout << "  ";
  }
  cout << name_ << ":m[" << iMaxWaveWidth_ << "]:i[" << interNumber_ << "]" << endl;
  for(auto top:top_) {
    top->build();
  }
  WatorBaseL::build();
}
void WatorHiddenL::wait(void) {
  for(auto top:top_) {
    top->wait();
  }
  WatorBaseL::wait();
}

void WatorHiddenL::forward(void) {
  //DUMP_VAR(name_);
  //DUMP_VAR(ave);
  for(auto top:top_) {
    top->forward();
  }
}

deque<int16_t> WatorHiddenL::active(void) {
    return {};
}
deque<bool> WatorHiddenL::diactive(void) {
    return {};
}
deque<int16_t> WatorHiddenL::value(void) {
    return {};
}


void WatorHiddenL::setDAF(double factor){
    dDeativeFactor_ = factor;
}


int WatorHiddenL::width(void) {
    return iMaxWaveWidth_;
}
void WatorHiddenL::snapshot(void){
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
        
        
        int16_t yDiff = heightLow + heightFull + heightDiff/2;
        if(i < intermediate_.at(ch).size() && intermediate_.at(ch).at(i)) {
            mat.at<cv::Vec3b>(yDiff, x) = cv::Vec3b(0,0,0);
        }
    }
    }
    for(auto top:top_) {
        top->snapshot();
    }
}


