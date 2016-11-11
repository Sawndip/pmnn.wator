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
protected:
  vector<WatorBaseLPtr> top_;
};
WatorInputL::WatorInputL()
  :WatorBaseL()
{
}

class WatorOutputL :public WatorBaseL {
public:
  WatorOutputL();
protected:
  vector<WatorBaseLPtr> buttom_;
};

WatorOutputL::WatorOutputL()
  :WatorBaseL()
{
}

class WatorHiddenL :public WatorBaseL {
public:
  WatorHiddenL();
protected:
  vector<WatorBaseLPtr> top_;
  vector<WatorBaseLPtr> buttom_;
};

WatorHiddenL::WatorHiddenL()
  :WatorBaseL()
{
}

class WatorNet {
public:
  WatorNet(WatorBaseLPtr entry);
  void train();
private:
  WatorBaseLPtr entry_;
}
WatorNet::WatorNet(WatorBaseLPtr entry) {
  :entry_(entry)
}
void WatorNet::train() {
}

int main() {
  shared_ptr<WatorInputL> input(new(WatorInputL));
  WatorNet net(input);
  net.train();
  return 0;
}
