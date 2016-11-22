#include <iostream>
#include <vector>
#include <memory>
#include <string>
using namespace std;
#include "pulse.hpp"


#define NAME_(x) {x->name(#x);}

int main() {
  shared_ptr<WatorAudioWaveL> audio = make_shared<WatorAudioWaveL>();
  NAME_(audio);
  
  shared_ptr<WatorHiddenL> hide1 = make_shared<WatorHiddenL>();
  NAME_(hide1);
  shared_ptr<WatorHiddenL> hide2 = make_shared<WatorHiddenL>();
  NAME_(hide2);
  shared_ptr<WatorHiddenL> hide3 = make_shared<WatorHiddenL>();
  NAME_(hide3);
    shared_ptr<WatorHiddenL> hide4 = make_shared<WatorHiddenL>();
    NAME_(hide4);
    shared_ptr<WatorHiddenL> hide5 = make_shared<WatorHiddenL>();
    NAME_(hide5);
    shared_ptr<WatorHiddenL> hide6 = make_shared<WatorHiddenL>();
    NAME_(hide6);

    shared_ptr<WatorHiddenL> hide7 = make_shared<WatorHiddenL>();
    NAME_(hide7);

    shared_ptr<WatorHiddenL> hide8 = make_shared<WatorHiddenL>();
    NAME_(hide8);

    shared_ptr<WatorHiddenL> hide9 = make_shared<WatorHiddenL>();
    NAME_(hide9);

    shared_ptr<WatorHiddenL> hide10 = make_shared<WatorHiddenL>();
    NAME_(hide10);
 
  shared_ptr<WatorOutputL> out = make_shared<WatorOutputL>();
  NAME_(out);
  
  audio->addTop(hide1);
  hide1->addTop(hide2);
  hide2->addTop(hide3);
    hide3->addTop(hide4);
    hide4->addTop(hide5);
    hide5->addTop(hide6);
    hide6->addTop(hide7);
    hide7->addTop(hide8);
    hide8->addTop(hide9);
    hide9->addTop(hide10);
    hide10->addTop(out);

  hide1->addButtom(audio);
  hide2->addButtom(hide1);
    hide3->addButtom(hide2);
    hide4->addButtom(hide3);
    hide5->addButtom(hide4);
    hide6->addButtom(hide5);
    hide7->addButtom(hide6);
    hide8->addButtom(hide7);
    hide9->addButtom(hide8);
    hide10->addButtom(hide9);
  out->addButtom(hide10);
    
    
    audio->setDAF(0.5);
    hide1->setDAF(0.6);
    hide2->setDAF(0.8);
    hide3->setDAF(1.0);
    hide4->setDAF(1.0);
    hide5->setDAF(1.0);
    hide6->setDAF(1.0);
    hide7->setDAF(1.0);
    hide8->setDAF(1.0);
    hide9->setDAF(1.0);
    hide10->setDAF(1.0);
  
  WatorNet net(audio);
  net.layout();
  net.train();
  net.snapshot();
  return 0;
}
