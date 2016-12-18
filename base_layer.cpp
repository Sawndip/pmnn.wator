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
#define DUMP_VAR(x) {BOOST_LOG_TRIVIAL(debug) << __func__ << ":" << __LINE__ << ":" #x "=<" << x << ">" << endl;}


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
  t_.join();
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

int16_t WatorBaseL::active(void) {
  return 0;
}
bool WatorBaseL::diactive(void) {
  return true;
}
int16_t WatorBaseL::value(void) {
  return 0;
}
int WatorBaseL::width(void) {
    return 0;
}
void WatorBaseL::snapshot(void){
    
}



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
int16_t WatorInputL::active(void) {
  return 0;
}
bool WatorInputL::diactive(void) {
  return true;
}
int16_t WatorInputL::value(void) {
    return 0;
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
  //cout << name_ << endl;
  auto buttom = buttom_.at(0);
  int16_t blob = buttom->active();
  static int16_t prev_activ = 0;
  bool _a = buttom->diactive();
  //DUMP_VAR(_a);
  if(_a) {
      //blob = prev_activ;
      blob = 0;
  } else {
      prev_activ = blob;
  }
  //DUMP_VAR(blob);
  stepBuff_.push_back(blob);
  if(stepBuff_.size() < step_ ) {
    return;
  }
  
  int16_t ave = accumulate(stepBuff_.begin(),stepBuff_.end(),0);
  ave /= stepBuff_.size();
  
  uint16_t max = 0;
  int16_t vMax = stepBuff_[0];
  
  for(auto &val: stepBuff_) {
      if(std::abs(val) > max) {
          max = std::abs(val);
          vMax = val;
      }
  }
  stepBuff_.clear();
//  blob_.push_back(ave);
    blob_.push_back(vMax);
  if( blob_.size()> iMaxWaveWidth_) {
    blob_.pop_front();
  }
//    if(::abs(ave) > maxHeight_) {
    if(::abs(vMax) > maxHeight_) {
        maxHeight_ = max;
    }
    
  //DUMP_VAR(name_);
  //DUMP_VAR(ave);
  for(auto top:top_) {
    top->forward();
  }
}

int16_t WatorHiddenL::active(void) {
  if(blob_.size()>1) {
    auto it = blob_.rbegin();
    return (*it)*2/3 + *(++it)/3;
  }
  return 0;
}
bool WatorHiddenL::diactive(void) {
    if(blob_.size()>1) {
        auto it = blob_.rbegin();
        double diff = *it;
        //DUMP_VAR(*it);
        diff -= *(it+1);
        double sum = std::abs(*it);
        sum += std::abs(*(it+1));
        sum += 1.0;
        //DUMP_VAR(*it);
        double diffABS = std::abs(diff);
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
        if(diffAdj > dThreshold_ * dDeativeFactor_) {
            //DUMP_VAR(name_);
            //DUMP_VAR(dThreshold_);
            //DUMP_VAR(diffABS);
            //DUMP_VAR(diffAdj);
            intermediate_.push_back(false);
            if(intermediate_.size() >iMaxWaveWidth_) {
              intermediate_.pop_front();
            }
           return false;
        }
    }
    intermediate_.push_back(true);
    if(intermediate_.size() >iMaxWaveWidth_) {
      intermediate_.pop_front();
    }
    return true;
}
int16_t WatorHiddenL::value(void) {
    return 0;
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
    DUMP_VAR(blob_.size());
    int heightLow = 256;
    int heightFull = 256 *8;
    int heightDiff = 100;
    int width = std::min(iConstWaveGraphWidth,blob_.size());;
    cv::Mat mat( heightLow + heightFull + heightDiff ,width,CV_8UC3,cv::Scalar(255,255,255));
    for(int i = 0;i < blob_.size();i++) {
        
        auto slip = i / iConstWaveGraphWidth;
        if(i % iConstWaveGraphWidth == 0){
            //mat = white;
            mat = cv::Scalar(255,255,255);
        }
        if(i % iConstWaveGraphWidth == iConstWaveGraphWidth-1 || i == blob_.size() -1) {
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
        int16_t val = blob_.at(i);
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
        if(i < intermediate_.size() && intermediate_.at(i)) {
            mat.at<cv::Vec3b>(yDiff, x) = cv::Vec3b(0,0,0);
        }
    }
    for(auto top:top_) {
        top->snapshot();
    }
}


