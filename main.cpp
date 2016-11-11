#include <iostream>
#include <vector>
#include <memory>
using namespace std;

typedef shared_ptr<WatorBaseL> WatorBaseLPtr;



class WatorBaseL {
public:
protected:
  WatorBaseL();
};
WatorBaseL::WatorBaseL(){
}

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


int main() {
  return 0;
}
