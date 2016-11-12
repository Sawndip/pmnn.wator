#include <iostream>
#include <vector>
#include <memory>
#include <string>
using namespace std;
#include "pulse.hpp"


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
