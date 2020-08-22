//
// Created by mahmo on 8/17/2020.
//

#ifndef WTINYLFU_COUNTMIN64TLFU_H
#define WTINYLFU_COUNTMIN64TLFU_H

#include "../Frequency.h"
#include "countMin64.h"
#include "../../BasicSettings.h"
#define MAX_COUNT 15



class cm64TinyLFU: public Frequency {
private:
    bool conservative;
    countMin64 sketch ;
    int sampleSize;
    int size;
public:
    cm64TinyLFU(BasicSettings* settings):
            sketch(settings->tinyLfu().countMin64().getEps(),settings->tinyLfu().countMin64().getConfidence())
    {
      conservative = settings->tinyLfu().conservative();
      sampleSize = 10 * settings->getMaximumSize();
    }

    ~cm64TinyLFU(); //probably wrong needs change mahmod

    virtual int frequency(long o)override {
      return (int) sketch.estimate(o);
    }

    virtual void increment(long o) override {
      if (sketch.estimate(o) < MAX_COUNT) {
        sketch.update(conservative, o, 1);
      }
      size += 1;
      resetIfNeeded();
    }

    void resetIfNeeded() {
      if (size > sampleSize) {
        size /= 2;
        for (int i = 0; i < sketch.depth ; i++) {
          for (int j = 0; j < sketch.width; j++) {
            size -= ((int) sketch.table[i][j]) & 1;
            sketch.table[i][j] >>= 1;
          }

        }
      }
    }

};

#endif //WTINYLFU_COUNTMIN64TLFU_H
