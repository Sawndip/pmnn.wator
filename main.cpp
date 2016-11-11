#include <iostream>
#include <vector>
using namespace std;


class WatorBaseL {
public:
protected:
  WatorBaseL();
};

class WatorInputL :public WatorBaseL {
public:
  WatorInputL();
protected:
  vector<WatorBaseL> top_;
};

class WatorOutputL :public WatorBaseL {
public:
  WatorOutputL();
protected:
  vector<WatorBaseL> buttom_;
};

class WatorHiddenL :public WatorBaseL {
public:
  WatorHiddenL();
protected:
  vector<WatorBaseL> top_;
  vector<WatorBaseL> buttom_;
};


int main() {
  return 0;
}
