#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <numeric>
using namespace std;
#include "pulse.hpp"
#include "wave.hpp"
#define DUMP_VAR(x) {cout << __LINE__ << ":" #x "=<" << x << ">" << endl;}


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


WatorAudioWaveL::WatorAudioWaveL()
:WatorInputL() {
}
void WatorAudioWaveL::forward(void){
  auto waves = readWave("./waveform/myRecording03.wav");
  DUMP_VAR(waves.size());
  if(waves.empty()) {
    return ;
  }

  auto wave = waves.begin();
  DUMP_VAR(wave->size());
  for(int i = 0;i < wave->size() -1;i++) {
    blob_.push_back(wave->at(i));
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
    return (*it + *it++)/2;
  }
  return 0;
}
bool WatorAudioWaveL::diactive(void) {
  if(blob_.size()>1) {
    auto it = blob_.rbegin();
    int diff = *it;
    //DUMP_VAR(*it);
    diff -= *(++it);
    //DUMP_VAR(*it);
    uint16_t diffABS = ::abs(diff);
    //DUMP_VAR(diff);
    //DUMP_VAR(diffABS);
    diffs_.push_back(diffABS);
      //DUMP_VAR(diffs_.size());
      //DUMP_VAR(iMaxWaveWidth_);
      if(diffs_.size() >iMaxWaveWidth_) {
          diffs_.pop_front();
      }
      std::sort(diffs_.begin(),diffs_.end(),std::greater<uint16_t>());
      //DUMP_VAR(interNumber_);
      if(diffs_.size() > interNumber_) {
          auto last = diffs_[interNumber_];
          //DUMP_VAR(last);
          //DUMP_VAR(diffABS);
          if(diffABS > last) {
              return true;
          }
      }
      return false;
  }
  return false;
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
  int16_t a = buttom->active();
  bool _a = buttom->diactive();
    if(_a) {
  DUMP_VAR(a);
  DUMP_VAR(_a);
    }
  int16_t blob = a * active_ + disactive_ * _a;
  //DUMP_VAR(blob);
  stepBuff_.push_back(blob);
  if(stepBuff_.size() < step_) {
    return;
  }
  
  //intermediate_.push();

  int16_t ave = accumulate(stepBuff_.begin(),stepBuff_.end(),0);
  ave /= stepBuff_.size();
  stepBuff_.clear();
  blob_.push_back(ave);
  if( blob_.size()> iMaxWaveWidth_) {
    blob_.pop_front();
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
    return (*it + *it++)/2;
  }
  return 0;
}
bool WatorHiddenL::diactive(void) {
  if(blob_.size()>1) {
    auto it = blob_.rbegin();
    return *it - *it++;
  }
  return false;
}

int WatorHiddenL::width(void) {
    return iMaxWaveWidth_;
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



