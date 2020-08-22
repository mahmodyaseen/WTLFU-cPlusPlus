//
// Created by mahmo on 8/16/2020.
//

#ifndef WTINYLFU_SIMPLECLIMBER_H
#define WTINYLFU_SIMPLECLIMBER_H

#define  MAX_VALUE 0x7fffffff

#include "../../../../BasicSettings.h"
#include <string>
#include "../AbstractClimber.h"

//#define MAX_VALUE 2147483647;

class SimpleClimber : public AbstractClimber{
private:
    double restartThreshold;
    double initialStepSize;
    double sampleDecayRate;
    int initialSampleSize;
    double stepDecayRate;
    double tolerance;
    bool increaseWindow;
    double stepSize;

public:
    SimpleClimber(BasicSettings* settings) {
      SimpleClimberSettings* climbersettings = new SimpleClimberSettings(settings);
      this->initialSampleSize = (int) (climbersettings->percentSample() * settings->getMaximumSize());
      this->initialStepSize = climbersettings->percentPivot() * settings->getMaximumSize();
      this->restartThreshold = climbersettings->restartThreshold();
      this->sampleDecayRate = climbersettings->sampleDecayRate();
      this->stepDecayRate = climbersettings->stepDecayRate();
      this->tolerance = 100.00 * climbersettings->tolerance();
      this->sampleSize = initialSampleSize;
      this->stepSize = initialStepSize;
    }

    double adjust(double hitRate) override {
      if (hitRate < (previousHitRate + tolerance)) {
        increaseWindow = !increaseWindow;
      }
      if (abs(hitRate - previousHitRate) >= restartThreshold) {
        sampleSize = initialSampleSize;
        stepSize = initialStepSize;
      }
      return increaseWindow ? stepSize : -stepSize;
    }

    void resetSample(double hitRate) override {
      AbstractClimber::resetSample(hitRate);
      stepSize *= stepDecayRate;
      sampleSize = (int) (sampleSize * sampleDecayRate);
      if ((stepSize <= 0.01) || (sampleSize <= 1)) {
        sampleSize = MAX_VALUE;
      }
    }

    class SimpleClimberSettings {
    public:
        BasicSettings* basicSettings;
        double percentPivot() {
          return basicSettings->hillClimberWindowTinyLfu().simple().percentPivot;
        }
        double percentSample() {
          return basicSettings->hillClimberWindowTinyLfu().simple().percentSample;
        }
        double tolerance() {
          return basicSettings->hillClimberWindowTinyLfu().simple().tolerance;
        }
        double stepDecayRate() {
          return basicSettings->hillClimberWindowTinyLfu().simple().stepDecayRate;
        }
        double sampleDecayRate() {
          return basicSettings->hillClimberWindowTinyLfu().simple().sampleDecayRate;
        }
        double restartThreshold() {
          return basicSettings->hillClimberWindowTinyLfu().simple().restartThreshold;
        }

        SimpleClimberSettings(BasicSettings* settings){
          this->basicSettings= settings;
        }
    };
};

#endif //WTINYLFU_SIMPLECLIMBER_H
