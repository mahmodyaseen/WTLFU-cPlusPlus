//
// Created by mahmo on 7/26/2020.
//

#include "ClimberResetCountMin4.h"

ClimberResetCountMin4::ClimberResetCountMin4(BasicSettings* settings) :
        countMin4(settings) ,
        doorkeeper(settings)
{
   additions =0;
   period = 0;
   prevMisses =0 ; // misses in previous interval
   misses = 0; // misses in this interval
   direction = 1; // are we increasing the 'step size' or decreasing it
   eventsToCount = 0;
  enabled = settings->tinyLfu().countMin4().periodic().doorkeeper().getEnabled();
}

int getLength(long * array){
  int i =0;
  while(array[i] !=  '\0'){
    i++;
  }
  return i;
}

void ClimberResetCountMin4::ensureCapacity(long maximumSize) {
  countMin4::ensureCapacity(maximumSize);
  period = (maximumSize == 0) ? 10 : (10 * getLength(table));
  if (period <= 0) {
    period = 2147483648;
  }
  eventsToCount = period;
}

int ClimberResetCountMin4::frequency(long e)  {
  int count =0;
  if(enabled){
    if (doorkeeper.mightContain(e)) {
      count++;
    }
  }
  count += countMin4::frequency(e);
  return MIN(count, 15);
}

void ClimberResetCountMin4::increment(long e) {
  eventsToCount--;
  if(enabled) {
    if (!doorkeeper.put(e)) {
      countMin4::increment(e);
    }
  }else{
    countMin4::increment(e);
  }
}


static unsigned int countSetBits(unsigned int n)
{
  unsigned int count = 0;
  while (n) {
    count += n & 1;
    n >>= 1;
  }
  return count;
}


void ClimberResetCountMin4::tryReset(bool added) {
  additions += step;
  if (!added) {
    return;
  }
  if (additions < period) {
    return;
  }
  int count = 0;
  for (int i = 0; i < getLength(table); i++) {
    count += countSetBits(table[i] & ONE_MASK);
    table[i] = (unsigned(table[i]) >> 1) & RESET_MASK;
  }
  additions = (unsigned(additions) >> 1) - (unsigned (count) >> 2);
  doorkeeper.clear();
}

void ClimberResetCountMin4::reportMiss() {
  // Each time there is a miss, TinyLFU invokes the reportMiss function and we can
  // make decisions
  misses++;
  if (eventsToCount <= 0) {
    eventsToCount = period;
    if (misses > prevMisses) {
      direction = -1 * direction;
    }
    step -= direction;
    prevMisses = misses;
    misses = 0;
    if (step < 1) {
      step = 1;
    } else if (step > 15) {
      step = 15;
    }
  }
}