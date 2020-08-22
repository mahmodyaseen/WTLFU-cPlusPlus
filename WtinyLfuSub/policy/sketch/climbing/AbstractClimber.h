//
// Created by mahmo on 8/16/2020.
//

#ifndef WTINYLFU_ABSTRACTCLIMBER_H
#define WTINYLFU_ABSTRACTCLIMBER_H

#include "HillClimber.h"

class AbstractClimber : public HillClimber {
private:
    int hitsInMain;
    int hitsInWindow;
    int hitsInSample;
    int missesInSample;
protected:
    double previousHitRate;
    int sampleSize;
public:
    void onMiss(long key, bool isFull) override {
      if (isFull){
        missesInSample++;
      }
    }

    void onHit(long key, QueueType queueType, bool isFull) override {
      if(isFull){
        hitsInSample++;
        if (queueType == QueueType::WINDOW) {
          hitsInWindow++;
        }else{
          hitsInMain++;
        }
      }
    }

    virtual double adjust(double hitRate) =0;

    Adaptation adapt(double windowSize, double probationSize,
                     double protectedSize, bool isFull) override {
      if (!isFull) {
        return Adaptation::hold();
      }
      if(sampleSize <=0){
        throw("Sample size may not be zero");
      }
      int sampleCount = (hitsInSample + missesInSample);
      if(sampleCount < sampleSize) {
        return Adaptation::hold();
      }
      double hitRate = (double) hitsInSample / sampleCount;
      Adaptation adaption = Adaptation::adaptBy(adjust(hitRate));
      resetSample(hitRate);
      return adaption;
    }

    virtual void resetSample(double hitRate) {
      previousHitRate = hitRate;
      missesInSample = 0;
      hitsInSample = 0;
      hitsInWindow = 0;
      hitsInMain = 0;
    }
};

#endif //WTINYLFU_ABSTRACTCLIMBER_H
