//
// Created by mahmo on 8/16/2020.
//

#ifndef WTINYLFU_ADAM_H
#define WTINYLFU_ADAM_H

#include "../../../../BasicSettings.h"
#include "../../../../policy/sketch/climbing/AbstractClimber.h"
#include <list>

class Adam : public AbstractClimber {
private:
    int stepSize;
    float beta1;
    float beta2;
    float epsilon;
    int t;
    float moment;
    float velocity;
public:
    Adam(BasicSettings * sett) {
      AdamSettings* settings = new AdamSettings(sett);
      sampleSize = (int) (settings->percentSample() * sett->getMaximumSize());
      stepSize = (int) (settings->percentPivot() * sett->getMaximumSize());
      epsilon = settings->epsilon();
      beta1 = settings->beta1G();
      beta2 = settings->beta2G();
      t = 1;
    }


    double adjust(double hitRate) override {
      double currentMissRate = (1 - hitRate);
      double previousMissRate = (1 - previousHitRate);
      double gradient = currentMissRate - previousMissRate;

      moment = (beta1 * moment) + ((1 - beta1) * gradient);
      velocity = (beta2 * velocity) + ((1 - beta2) * (gradient * gradient));

      double momentBias = moment / (1 - pow(beta1, t));
      double velocityBias = velocity / (1 - pow(beta2, t));
      return (stepSize * momentBias) / (sqrt(velocityBias) + epsilon);
    }


    void resetSample(double hitRate) override {
      AbstractClimber::resetSample(hitRate);
      t++;
    }


    class AdamSettings {
    public:
        BasicSettings *settings;

        AdamSettings(BasicSettings *sett) {
          settings = sett;
        }

        ~AdamSettings()=default;

        float percentMain() {
          return settings->hillClimberWindowTinyLfu().percentMain;
        }
        float percentPivot() {
          return settings->hillClimberWindowTinyLfu().adam().percentPivot;
        }
        float percentSample() {
          return settings->hillClimberWindowTinyLfu().adam().percentSample;
        }
        float beta1G() {
          return settings->hillClimberWindowTinyLfu().adam().beta1;
        }
        float beta2G() {
          return settings->hillClimberWindowTinyLfu().adam().beta2;
        }
        float epsilon() {
          return settings->hillClimberWindowTinyLfu().adam().epsilon;
        }
    };
};

#endif //WTINYLFU_ADAM_H
