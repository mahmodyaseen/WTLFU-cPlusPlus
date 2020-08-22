//
// Created by mahmo on 8/16/2020.
//

#ifndef WTINYLFU_AMSGRAD_H
#define WTINYLFU_AMSGRAD_H


#include "../../../../BasicSettings.h"
#include "../AbstractClimber.h"
#include <list>


class AmsGrad : public AbstractClimber {
private:
    int stepSize;
    float beta1;
    float beta2;
    float epsilon;
    float moment;
    float velocity;
    float maxVelocity;
public:

    AmsGrad(BasicSettings* settings) {
      sampleSize = (int) (settings->hillClimberWindowTinyLfu().amsgrad().percentSample * settings->getMaximumSize());
      stepSize = (int) (settings->hillClimberWindowTinyLfu().amsgrad().percentPivot * settings->getMaximumSize());
      epsilon = settings->hillClimberWindowTinyLfu().amsgrad().epsilon;
      beta1 = settings->hillClimberWindowTinyLfu().amsgrad().beta1;
      beta2 = settings->hillClimberWindowTinyLfu().amsgrad().beta2;
    }


    double adjust(double hitRate) override {
      double currentMissRate = (1 - hitRate);
      double previousMissRate = (1 - previousHitRate);
      double gradient = currentMissRate - previousMissRate;
      moment = (beta1 * moment) + ((1 - beta1) * gradient);
      velocity = (beta2 * velocity) + ((1 - beta2) * (gradient * gradient));
      maxVelocity = max(velocity, maxVelocity);
      return (stepSize * moment) / (sqrt(maxVelocity) + epsilon);
    }

};


#endif //WTINYLFU_AMSGRAD_H
