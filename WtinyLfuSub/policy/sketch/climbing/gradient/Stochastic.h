//
// Created by mahmo on 8/16/2020.
//

#ifndef WTINYLFU_STOCHASTIC_H
#define WTINYLFU_STOCHASTIC_H


#include "../../../../BasicSettings.h"
#include "../../../../policy/sketch/climbing/AbstractClimber.h"
#include <list>
#include <cctype>
#include <cstring>
#include <cstdio>
#include <string>


enum Acceleration { NON, MOMENTUM, NESTEROV };

class Stochastic : public AbstractClimber{
private:
    Acceleration acceleration;
    int stepSize;
    float beta;
    float velocity;
public:
    Stochastic(BasicSettings* sett){
      StochasticSettings* settings = new StochasticSettings(sett);
      sampleSize = (int) (settings->percentSample() * sett->getMaximumSize());
      stepSize = (int) (settings->percentPivot() * sett->getMaximumSize());
      Acceleration tmp = settings->acceleration();
      beta = settings->betaG();
    }

    double adjust(double hitRate) override {
      double currentMissRate = (1 - hitRate);
      double previousMissRate = (1 - previousHitRate);
      double gradient = currentMissRate - previousMissRate;

      switch (acceleration) {
        case NONE:
          return stepSize * gradient;
        case MOMENTUM:
          velocity = (beta * velocity) + (1 - beta) * gradient;
          return stepSize * velocity;
        case NESTEROV:
          double previousVelocity = velocity;
          velocity = (beta * velocity) + stepSize * gradient;
          return -(beta * previousVelocity) + ((1 + beta) * velocity);
      }
      throw (&"Unknown acceleration type: " [ acceleration]);
    }

    class StochasticSettings  {
    public:
        BasicSettings* settings;
        StochasticSettings(BasicSettings* sett) {
          settings = sett;
        }
        ~StochasticSettings() =default;
        float percentMain() {
          return settings->hillClimberWindowTinyLfu().percentMain;
        }
        float percentPivot() {
          return settings->hillClimberWindowTinyLfu().stochasticGradientDescent().percentPivot;
        }
        float percentSample() {
          return settings->hillClimberWindowTinyLfu().stochasticGradientDescent().percentSample;
        }
        Acceleration acceleration() {
          return Acceleration ::MOMENTUM;
        }
        float betaG() {
          return settings->hillClimberWindowTinyLfu().stochasticGradientDescent().beta;
        }
    };
};


#endif //WTINYLFU_STOCHASTIC_H
