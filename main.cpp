#include <iostream>
#include <vector>
#include <memory>
#include <string>
using namespace std;
#include "pulse.hpp"

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#define NAME_(x) {x->name(#x);}

#define NAME__(x) {x.name(#x);}

int main() {
    boost::log::core::get()->set_filter (
                                         boost::log::trivial::severity >= boost::log::trivial::warning
                                         );
/*
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
    
    
    audio->setDAF(0.01);
    hide1->setDAF(0.01);
    hide2->setDAF(0.02);
    hide3->setDAF(0.02);
    hide4->setDAF(0.02);
    hide5->setDAF(0.2);
    hide6->setDAF(0.5);
    hide7->setDAF(0.5);
    hide8->setDAF(0.5);
    hide9->setDAF(0.5);
    hide10->setDAF(0.5);
*/
    shared_ptr<WatorAudioWave2L> audio = make_shared<WatorAudioWave2L>();
    NAME_(audio);
    shared_ptr<HalfSinCurveL> halfSin = make_shared<HalfSinCurveL>();
    NAME_(halfSin);
    shared_ptr<FullSinCurveL> fullSin = make_shared<FullSinCurveL>();
    NAME_(fullSin);
    shared_ptr<WatorOutputL> out = make_shared<WatorOutputL>();
    NAME_(out);
  
    audio->addTop(halfSin);
    halfSin->addTop(fullSin);
    fullSin->addTop(out);

    halfSin->addButtom(audio);
    fullSin->addButtom(halfSin);
    out->addButtom(fullSin);

    
  WatorNet net(audio);
  net.layout();
  net.train();
  net.snapshot();
  return 0;
}
