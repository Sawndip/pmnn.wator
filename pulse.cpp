#include <iostream>
#include <vector>
#include <memory>
#include <string>
using namespace std;
#include "pulse.hpp"


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
}
void WatorNet::layout() {
  entry_->layout();
}



