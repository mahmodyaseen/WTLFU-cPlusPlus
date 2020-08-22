//
// Created by mahmo on 8/16/2020.
//

#ifndef WTINYLFU_NADAM_H
#define WTINYLFU_NADAM_H



#include "../../../../BasicSettings.h"
#include "../../../../policy/sketch/climbing/AbstractClimber.h"
#include <list>

class Nadam : public AbstractClimber {
private:
    int stepSize;
    float beta1;
    float beta2;
    float epsilon;
    int t;
    float moment;
    float velocity;

public:

    Nadam(BasicSettings* settings) {
      sampleSize = (int) (settings->hillClimberWindowTinyLfu().nadam().percentSample * settings->getMaximumSize());
      stepSize = (int) (settings->hillClimberWindowTinyLfu().nadam().percentPivot * settings->getMaximumSize());
      epsilon = settings->hillClimberWindowTinyLfu().nadam().epsilon;
      beta1 = settings->hillClimberWindowTinyLfu().nadam().beta1;
      beta2 = settings->hillClimberWindowTinyLfu().nadam().beta2;
      t = 1;
    }


    void resetSample(double hitRate) override {
      AbstractClimber::resetSample(hitRate);
      t++;
    }

    double adjust(double hitRate) override {
      double currentMissRate = (1 - hitRate);
      double previousMissRate = (1 - previousHitRate);
      double gradient = currentMissRate - previousMissRate;

      moment = (beta1 * moment) + ((1 - beta1) * gradient);
      velocity = (beta2 * velocity) + ((1 - beta2) * (gradient * gradient));

      double momentBias = moment / (1 - pow(beta1, t));
      double velocityBias = velocity / (1 - pow(beta2, t));
      return (stepSize / (sqrt(velocityBias) + epsilon))
             * ((beta1 * momentBias) + (((1 - beta1) / (1 - pow(beta1, t))) * gradient));
    }
};

#endif //WTINYLFU_NADAM_H
