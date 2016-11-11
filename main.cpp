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

class WatorInputL :public WatorBaseL {
public:
  WatorInputL();
protected:
  vector<WatorBaseL> top_;
};
WatorInputL::WatorInputL()
  :WatorBaseL()
{
}

class WatorOutputL :public WatorBaseL {
public:
  WatorOutputL();
protected:
  vector<WatorBaseL> buttom_;
};

WatorOutputL::WatorOutputL()
  :WatorBaseL()
{
}

class WatorHiddenL :public WatorBaseL {
public:
  WatorHiddenL();
protected:
  vector<WatorBaseL> top_;
  vector<WatorBaseL> buttom_;
};

WatorHiddenL::WatorHiddenL()
  :WatorBaseL()
{
}


int main() {
  return 0;
}
