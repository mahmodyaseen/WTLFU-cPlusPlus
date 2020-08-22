#include <iostream>
#include "membership/bloom/bloomFilter.h"
#include <iostream>
#include "Admission/countmin4/countMin4.h"
#include "Admission/countmin4/PeriodicResetCountMin4.h"
#include "BasicSettings.h"
#include "Admission/TinyLfu.h"
#include "policy/PolicyStats.h"
#include "policy/sketch/WindowTinyLfuPolicy.h"

int main() {
  BasicSettings* settings= new BasicSettings();
//  countMin4 sketch=countMin4(settings);
//  sketch.increment(2);
//  sketch.increment(5);
//  sketch.increment(5);
//  sketch.increment(5);
//  sketch.increment(5);
//  sketch.increment(3);
//  sketch.increment(9);
//  sketch.increment(2);
//  sketch.increment(3);
//  sketch.increment(3);
//  std::cout << sketch.frequency(5)<< std::endl;

//  sketch. ~countMin4();

//  PeriodicResetCountMin4 periodicResetCountMin4(settings);
////  periodicResetCountMin4.increment(2);
////  periodicResetCountMin4.increment(2);
////  periodicResetCountMin4.increment(5);
////  periodicResetCountMin4.increment(5);
////  periodicResetCountMin4.increment(5);
////  periodicResetCountMin4.increment(5);
////  std::cout << periodicResetCountMin4.frequency(5)<< std::endl;
////  std::cout << periodicResetCountMin4.frequency(2)<< std::endl;
////
////  std::cout << periodicResetCountMin4.frequency(3)<< std::endl;

policyStats stats=  policyStats();
TinyLfu tiny = *new TinyLfu(&stats,settings);
tiny.record(5);
  std::cout << "Hello, World!"<<" XXX" <<std::endl;
  return 0;
}
