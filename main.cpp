#include <iostream>
#include <vector>
#include <memory>
using namespace std;


class WatorBaseL {
public:
protected:
  WatorBaseL();
};
WatorBaseL::WatorBaseL(){
}

typedef shared_ptr<WatorBaseL> WatorBaseLPtr;


class WatorInputL :public WatorBaseL {
public:
  WatorInputL();
  void addTop(WatorBaseLPtr top);
protected:
  vector<WatorBaseLPtr> top_;
};
WatorInputL::WatorInputL()
  :WatorBaseL(){
}
void WatorInputL::addTop (WatorBaseLPtr top){
  top_.push_back(top);
}


class WatorOutputL :public WatorBaseL {
public:
  WatorOutputL();
  void addButtom(WatorBaseLPtr buttom);
protected:
  vector<WatorBaseLPtr> buttom_;
};

WatorOutputL::WatorOutputL()
  :WatorBaseL(){
}
void WatorOutputL::addButtom(WatorBaseLPtr buttom) {
  buttom_.push_back(buttom);
}


class WatorHiddenL :public WatorBaseL {
public:
  WatorHiddenL();
  void addTop(WatorBaseLPtr top);
  void addButtom(WatorBaseLPtr buttom);
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
}

int main() {
  shared_ptr<WatorInputL> input = make_shared<WatorInputL>();
  shared_ptr<WatorHiddenL> hide1 = make_shared<WatorHiddenL>();
  shared_ptr<WatorHiddenL> hide2 = make_shared<WatorHiddenL>();
  shared_ptr<WatorOutputL> out = make_shared<WatorOutputL>();
  
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
