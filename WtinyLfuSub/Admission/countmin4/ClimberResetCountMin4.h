//
// Created by mahmo on 8/17/2020.
//

#ifndef WTINYLFU_CLIMBERRESETCOUNTMIN4_H
#define WTINYLFU_CLIMBERRESETCOUNTMIN4_H



#include "countMin4.h"
#include "../../membership/bloom/bloomFilter.h"


#define MIN(a,b)    (a > b ? b : a)
#define MAX(a ,b)   (a > b ? a : b)

#define MAX_VALUE  0x7fffffff

class ClimberResetCountMin4: public countMin4 {

    long long ONE_MASK = 0x1111111111111111L;

    bloomFilter doorkeeper;

    int additions;
    int period;
    int prevMisses; // misses in previous interval
    int misses; // misses in this interval
    int direction = 1; // are we increasing the 'step size' or decreasing it
    int eventsToCount;
    bool enabled;
public:
//    ClimberResetCountMin4();

    ClimberResetCountMin4(BasicSettings *settings);

    void ensureCapacity(long maximumSize) override ;

    int frequency(long e) override ;

    void increment(long e) override ;

    void tryReset(bool added) override  ;

    void reportMiss() override ;

    int getStep() {
      return step;
    }

    void setStep(int x) {
      this->step = MAX(1, MIN(15, step));
    }

    int getEventsToCount() {
      return eventsToCount;
    }

    void resetEventsToCount() {
      eventsToCount = period;
    }

    int getPeriod() {
      return period;
    }
};

#endif //WTINYLFU_CLIMBERRESETCOUNTMIN4_H
