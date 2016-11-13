#include <iostream>
#include <vector>
#include <memory>
#include <string>
using namespace std;
#include "pulse.hpp"
#include "wave.hpp"
#define DUMP_VAR(x) {cout << #x "=<" << x << ">" << endl;}


WatorBaseL::WatorBaseL(){
}
void WatorBaseL::layout() {
}
void WatorBaseL::name(const string &name) 
{
  name_ = name;
}
int WatorBaseL::depth(void){
  return depth_;
}
void WatorBaseL::forward(){
}


WatorInputL::WatorInputL()
:WatorBaseL(){
}
void WatorInputL::addTop (WatorBaseLPtr top){
  top_.push_back(top);
}
void WatorInputL::layout()
{
  cout << name_ << endl;
  for(auto top:top_) {
    top->layout();
  }
}

void WatorInputL::forward(){
  auto waves = readWave("./waveform/myRecording03.wav");
  DUMP_VAR(waves.size());
  if(waves.empty()) {
    return ;
  }

  auto wave = waves.begin();
  DUMP_VAR(wave->size());
  for(int i = 0;i < wave->size() -1;i++) {
    blob_.push_back(wave->at(i));
    if(blob_.size() > iMaxWaveLength_) {
      blob_.pop_front();
    }
    for(auto top:top_) {
      top->forward();
    }
  }
}




WatorOutputL::WatorOutputL()
  :WatorBaseL(){
}
void WatorOutputL::addButtom(WatorBaseLPtr buttom) {
  buttom_.push_back(buttom);
  depth_ = buttom->depth() +1;
}
void WatorOutputL::layout()
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
void WatorHiddenL::layout()
{
  for(int i = 0 ;i < depth_ ;i++) {
    cout << "  ";
  }
  cout << name_ << endl;
  for(auto top:top_) {
    top->layout();
  }
}


WatorNet::WatorNet(WatorBaseLPtr entry)
  :entry_(entry) {
}
void WatorNet::train() {
    entry_->forward();
}
void WatorNet::layout() {
  entry_->layout();
}



