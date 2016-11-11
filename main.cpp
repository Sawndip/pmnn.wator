#include <iostream>
#include <vector>
#include <memory>
#include <string>
using namespace std;


class WatorBaseL {
public:
  virtual void layout();
  void name(const string &name);
protected:
  WatorBaseL();
  string name_;
};
WatorBaseL::WatorBaseL(){
}
void WatorBaseL::layout() {
}
void WatorBaseL::name(const string &name) 
{
  name_ = name;
}


typedef shared_ptr<WatorBaseL> WatorBaseLPtr;


class WatorInputL :public WatorBaseL {
public:
  WatorInputL();
  void addTop(WatorBaseLPtr top);
  virtual void layout();
protected:
  vector<WatorBaseLPtr> top_;
};
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



class WatorOutputL :public WatorBaseL {
public:
  WatorOutputL();
  void addButtom(WatorBaseLPtr buttom);
  virtual void layout();
protected:
  vector<WatorBaseLPtr> buttom_;
};

WatorOutputL::WatorOutputL()
  :WatorBaseL(){
}
void WatorOutputL::addButtom(WatorBaseLPtr buttom) {
  buttom_.push_back(buttom);
}
void WatorOutputL::layout()
{
  cout << name_ << endl;
/*  
  for(auto top:top_) {
    top->layout();
  }
*/
}

class WatorHiddenL :public WatorBaseL {
public:
  WatorHiddenL();
  void addTop(WatorBaseLPtr top);
  void addButtom(WatorBaseLPtr buttom);
  virtual void layout();
protected:
  vector<WatorBaseLPtr> top_;
  vector<WatorBaseLPtr> buttom_;
};

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
}
void WatorHiddenL::layout()
{
  cout << name_ << endl;
  for(auto top:top_) {
    top->layout();
  }
}


class WatorNet {
public:
  WatorNet(WatorBaseLPtr entry);
  void train();
  void layout();
private:
  WatorBaseLPtr entry_;
};

WatorNet::WatorNet(WatorBaseLPtr entry)
  :entry_(entry) {
}
void WatorNet::train() {
}
void WatorNet::layout() {
  entry_->layout();
}

#define NAME_(x) {x->name(#x);}

int main() {
  shared_ptr<WatorInputL> input = make_shared<WatorInputL>();
  NAME_(input);
  shared_ptr<WatorHiddenL> hide1 = make_shared<WatorHiddenL>();
  NAME_(hide1);
  shared_ptr<WatorHiddenL> hide2 = make_shared<WatorHiddenL>();
  NAME_(hide2);
  shared_ptr<WatorOutputL> out = make_shared<WatorOutputL>();
  NAME_(out);
  
  input->addTop(hide1);
  hide1->addTop(hide2);
  hide2->addTop(out);

  hide1->addButtom(input);
  hide2->addButtom(hide1);
  out->addButtom(hide2);
  
  WatorNet net(input);
  net.layout();
  net.train();
  return 0;
}
