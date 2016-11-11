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
  void operator >> (WatorBaseLPtr buttom);
protected:
  vector<WatorBaseLPtr> buttom_;
};

WatorOutputL::WatorOutputL()
  :WatorBaseL(){
}

class WatorHiddenL :public WatorBaseL {
public:
  WatorHiddenL();
  void operator << (WatorBaseLPtr top);
  void operator >> (WatorBaseLPtr buttom);
protected:
  vector<WatorBaseLPtr> top_;
  vector<WatorBaseLPtr> buttom_;
};

WatorHiddenL::WatorHiddenL()
  :WatorBaseL(){
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

  WatorNet net(input);
  net.layout();
  net.train();
  return 0;
}
