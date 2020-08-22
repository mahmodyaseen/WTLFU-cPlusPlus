//
// Created by mahmo on 8/17/2020.
//

#ifndef WTINYLFU_TINYCACHEADAPTER_H
#define WTINYLFU_TINYCACHEADAPTER_H

#include "../Frequency.h"
#include "../../BasicSettings.h"
#include "tinyCacheSketch.h"
#include <ctime>
#define MAX_COUNT 15

using namespace std;

class TinyCacheAdapter: public Frequency {
private:
    tinyCacheSketch sketch =tinyCacheSketch(0,0,0);
    int sampleFactor=10;
    int maxCount=10;
public:
    TinyCacheAdapter(BasicSettings* settings){
      srand (time(NULL));
      double rnd = (double)rand() / RAND_MAX;;
      int nrSets = sampleFactor * settings->getMaximumSize();
      sketch = tinyCacheSketch(nrSets, 64, rnd);
    }

    int frequency(long o) override {
      return (int) sketch.countItem(o);
    }

    void increment(long o) override {
      if (sketch.countItem(o) < maxCount) {
        sketch.addItem(o);
      }
    }

};

#endif //WTINYLFU_TINYCACHEADAPTER_H
