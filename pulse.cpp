#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <numeric>
#include <iomanip>
using namespace std;

#include <opencv/highgui.h>
//#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#include "pulse.hpp"
#include "wave.hpp"


#define DUMP_VAR(x) {cout << __LINE__ << ":" #x "=<" << x << ">" << endl;}


const unsigned long iConstWaveGraphWidth = 3600*5;


WatorBaseL::WatorBaseL(){
}
void WatorBaseL::layout(void) {
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
int WatorBaseL::width(void) {
  return 0;
}
void WatorBaseL::snapshot(void){
    
}



WatorInputL::WatorInputL()
:WatorBaseL(){
}
void WatorInputL::addTop (WatorBaseLPtr top) {
  top_.push_back(top);
}
void WatorInputL::layout(void) {
  cout << name_ << endl;
  for(auto top:top_) {
    top->layout();
  }
}

void WatorInputL::forward(void){
}
int16_t WatorInputL::active(void) {
  return 0;
}
bool WatorInputL::diactive(void) {
  return true;
}
int WatorInputL::width(void) {
  return 0;
}
void WatorInputL::snapshot(void){
    
}


WatorAudioWaveL::WatorAudioWaveL()
:WatorInputL() {
}
void WatorAudioWaveL::forward(void){
  auto waves = readWave("./waveform/myRecording09.wav");
  DUMP_VAR(waves.size());
  if(waves.empty()) {
    return ;
  }

  auto wave = waves.begin();
  DUMP_VAR(wave->size());
  for(int i = 0;i < wave->size() -1;i++) {
    blob_.push_back(wave->at(i));
    if(::abs(wave->at(i)) > maxHeight_) {
        maxHeight_ = ::abs(wave->at(i));
    }
    if(blob_.size() > iMaxWaveWidth_) {
      blob_.pop_front();
    }
    for(auto top:top_) {
      top->forward();
    }
  }
}
int16_t WatorAudioWaveL::active(void) {
  if(blob_.size()>1) {
    auto it = blob_.rbegin();
    return (*it)*2/3 + *(++it)/3;
  }
  return 0;
}
bool WatorAudioWaveL::diactive(void) {
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
    diffs_.push_back(diffABS);
      //DUMP_VAR(diffs_.size());
      //DUMP_VAR(iMaxWaveWidth_);
      dThreshold_ += (double)diffABS/(sum *(double)diffs_.size());
      if(diffs_.size() >iMaxWaveWidth_) {
          dThreshold_ -= (double)diffs_.front()/(double)diffs_.size();
          diffs_.pop_front();
      }
      if(diffABS > dThreshold_ *  dDeativeFactor_) {
          //DUMP_VAR(diffABS);
          //DUMP_VAR(dThreshold_);
          intermediate_.push_back(true);
          if(intermediate_.size() >iMaxWaveWidth_) {
            intermediate_.pop_front();
          }
          return true;
      }
      intermediate_.push_back(false);
      if(intermediate_.size() >iMaxWaveWidth_) {
        intermediate_.pop_front();
      }
      return false;
  }
  intermediate_.push_back(false);
  if(intermediate_.size() >iMaxWaveWidth_) {
    intermediate_.pop_front();
  }
  return false;
}

void WatorAudioWaveL::setDAF(double factor){
    dDeativeFactor_ = factor;
}

int WatorAudioWaveL::width(void) {
  //DUMP_VAR(iMaxWaveWidth_);
  return iMaxWaveWidth_;
}

void WatorAudioWaveL::layout(void) {
    cout << name_ << ":m[" << iMaxWaveWidth_ << "]:i[" << interNumber_ << "]" << endl;
    for(auto top:top_) {
        top->layout();
    }
}
void WatorAudioWaveL::snapshot(void){
    DUMP_VAR(name_);
    DUMP_VAR(maxHeight_);
    DUMP_VAR(blob_.size());
    int height = 200;
    int heightDiff = 10;
    int width = std::min(iConstWaveGraphWidth,blob_.size());;
    cv::Mat mat( height + heightDiff ,width,CV_8UC3,cv::Scalar(255,255,255));
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
        int16_t baseLineWav =  height/2;
        int16_t yWave = (int16_t)( blob_.at(i)*height/ (2*maxHeight_))  + baseLineWav;
        int x = i % iConstWaveGraphWidth;
        cv::line(mat,cv::Point(x,yWave),cv::Point(x,baseLineWav), cv::Scalar(0,0,255));
        mat.at<cv::Vec3b>(baseLineWav, x) = cv::Vec3b(0,0,0);

        int16_t yDiff = height + heightDiff/2;
        if(i < intermediate_.size() && intermediate_.at(i)) {
           mat.at<cv::Vec3b>(yDiff, x) = cv::Vec3b(0,255,0);
        }
    }
    for(auto top:top_) {
        top->snapshot();
    }
}




WatorOutputL::WatorOutputL()
  :WatorBaseL(){
}
void WatorOutputL::addButtom(WatorBaseLPtr buttom) {
  buttom_.push_back(buttom);
  depth_ = buttom->depth() +1;
}
void WatorOutputL::layout(void)
{
  for(int i = 0 ;i < depth_ ;i++) {
    cout << "  ";
  }
  cout << name_ << endl;
}


WatorHiddenL::WatorHiddenL()
  :WatorBaseL(){
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
void WatorHiddenL::layout(void)
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
    top->layout();
  }
}

void WatorHiddenL::forward(void) {
  //cout << name_ << endl;
  auto buttom = buttom_.at(0);
  int16_t blob = buttom->active();
  static int16_t prev_activ = blob;
  bool _a = buttom->diactive();
  //DUMP_VAR(_a);
  if(_a) {
    blob = prev_activ;
  }
  prev_activ = blob;
  //DUMP_VAR(blob);
  stepBuff_.push_back(blob);
  if(stepBuff_.size() < step_) {
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
        maxHeight_ = ::abs(ave);
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
        diffs_.push_back(diffABS);
        //DUMP_VAR(diffs_.size());
        //DUMP_VAR(iMaxWaveWidth_);
        dThreshold_ += (double)diffABS/(sum * (double)diffs_.size());
        if(diffs_.size() >iMaxWaveWidth_) {
            dThreshold_ -= (double)diffs_.front()/(double)diffs_.size();
            diffs_.pop_front();
        }
        if(diffABS > dThreshold_ * dDeativeFactor_) {
            //DUMP_VAR(name_);
            //DUMP_VAR(dThreshold_);
            intermediate_.push_back(true);
            if(intermediate_.size() >iMaxWaveWidth_) {
              intermediate_.pop_front();
            }
           return true;
        }
        intermediate_.push_back(false);
        if(intermediate_.size() >iMaxWaveWidth_) {
          intermediate_.pop_front();
        }
        return false;
    }
    intermediate_.push_back(false);
    if(intermediate_.size() >iMaxWaveWidth_) {
      intermediate_.pop_front();
    }
    return false;
}
void WatorHiddenL::setDAF(double factor){
    dDeativeFactor_ = factor;
}


int WatorHiddenL::width(void) {
    return iMaxWaveWidth_;
}
void WatorHiddenL::snapshot(void){
    DUMP_VAR(name_);
    DUMP_VAR(maxHeight_);
    DUMP_VAR(blob_.size());
    int height = 200;
    int heightDiff = 10;
    int width = std::min(iConstWaveGraphWidth,blob_.size());;
    cv::Mat mat( height+heightDiff ,width,CV_8UC3,cv::Scalar(255,255,255));
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
        int16_t baseLineWav =  height/2;
        int16_t yWave = (int16_t)( blob_.at(i)*height/ (2*maxHeight_))  + baseLineWav;
        int x = i % iConstWaveGraphWidth;
        cv::line(mat,cv::Point(x,yWave),cv::Point(x,baseLineWav), cv::Scalar(0,0,255));
        mat.at<cv::Vec3b>(baseLineWav, x) = cv::Vec3b(0,0,0);

        int16_t yDiff = height + heightDiff/2;
        if(i < intermediate_.size() && intermediate_.at(i)) {
           mat.at<cv::Vec3b>(yDiff, x) = cv::Vec3b(0,255,0);
        }
    }
    for(auto top:top_) {
        top->snapshot();
    }
}





WatorNet::WatorNet(WatorBaseLPtr entry)
  :entry_(entry) {
}
void WatorNet::train(void) {
    entry_->forward();
}
void WatorNet::layout(void) {
  entry_->layout();
}

void WatorNet::snapshot(void) {
  entry_->snapshot();
}


