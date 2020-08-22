
#ifndef WTINYLFU_PERIODICRESETCOUNTMIN4_H
#define WTINYLFU_PERIODICRESETCOUNTMIN4_H


#include "countMin4.h"
#include "../../BasicSettings.h"
#include "../../membership/bloom/bloomFilter.h"


#define MIN(a,b)    (a > b ? b : a)
#define MAX(a ,b)   (a > b ? a : b)

class PeriodicResetCountMin4 : public countMin4 {
private:
    long long int ONE_MASK = 0x1111111111111111L;

    bloomFilter doorkeeper;
    int additions;
    int period;
    bool enabled;
public:
    PeriodicResetCountMin4(BasicSettings* settings):
    countMin4(settings),
    doorkeeper(settings){
      ensureCapacity(settings->getMaximumSize());
      this->enabled= settings->tinyLfu().countMin4().periodic().doorkeeper().enabled;
    }

    void ensureCapacity(long maximumSize) override {
      tableSize = maximumSize;
      countMin4::ensureCapacity(maximumSize);
      period = (maximumSize == 0) ? 10 : (10 * (tableSize));
      if (period <= 0) {
        period = 2147483647;
      }
    }

    int frequency(long e) override {
      int count =0;
      if(enabled){
        if (doorkeeper.mightContain(e)) {
          count++;
        }
      }
      count += countMin4::frequency(e);
      return MIN(count, 15);
    }

    void increment(long e) override {
      if(enabled) {
        if (!doorkeeper.put(e)) {
          countMin4::increment(e);
        }
      }else{
        countMin4::increment(e);
      }
    }

    static unsigned int countSetBits(unsigned int n) {
      unsigned int count = 0;
      while (n) {
        count += n & 1;
        n >>= 1;
      }
      return count;
    }

    void tryReset(bool added) override {
      if (!added) {
        return;
      }
      additions++;
      if (additions != period) {
        return;
      }
      int count = 0;
      for (int i = 0; i < tableSize; i++) {
        count += countSetBits(table[i] & ONE_MASK);
        table[i] = (unsigned(table[i]) >> 1) & RESET_MASK;
      }
      additions = (unsigned(additions) >> 1) - (unsigned(count) >> 2);
      if(enabled)
          doorkeeper.clear();
    }

};
#endif //WTINYLFU_PERIODICRESETCOUNTMIN4_H
