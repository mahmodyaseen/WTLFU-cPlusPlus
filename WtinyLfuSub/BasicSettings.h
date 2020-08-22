//
// Created by mahmo on 8/6/2020.
//

#ifndef WTINYLFU_BASICSETTINGS_H
#define WTINYLFU_BASICSETTINGS_H

#include <cstdio>
#include <cstdlib>
#include <set>


class BasicSettings {
public:
    BasicSettings()= default;

    // The maximum number of entries in the cache
    static const int maximumSize = 512;
    static int getMaximumSize(){
      return maximumSize;
    }

  //The seed for randomized operations
    static const int randomSeed = 1033096058;
    static int getRandomSeed(){
      return randomSeed;
    }


    class membershipSettings {
    // caffeine, guava, fast-filter
    //        filter = caffeine
    public:
        membershipSettings() = default;
    //The false positive probability
        double fpp = 0.01;

//        FilterType filter() {
//                String type = config().getString("membership.filter");
//                return FilterType.valueOf(CaseFormat.LOWER_HYPHEN.to(CaseFormat.UPPER_UNDERSCORE, type));
//            }

        double multiplier = 3.0;

        long expectedInsertions(){
          return (BasicSettings::getMaximumSize() * (int) multiplier);
        }

        double getfpp() {
          return this->fpp;
        }
    };

    membershipSettings memberShipSet() {
      return* new membershipSettings();
    }

    class  windowTinyLfuSettings {
    public:
        windowTinyLfuSettings()= default;

    // The percentage for the MAIN space (PROBATION + PROTECTED)
        double percentMain = 0.99;
    // The percentage for the PROTECTED MAIN queue
        double percentMainProtected = 0.80;
    };

    windowTinyLfuSettings windowTinyLfu(){
      return *new windowTinyLfuSettings();
    }

    class tinyLfuSettings {
    public:
        tinyLfuSettings() = default;

        //count-min-4: periodic:- 55.74% - 55.76% = +0.02%
        //count-min-4: incremental:- 55.54% - 53.52% = -2.0%
        //count-min-4: climber:-  55.88% - 53.27% = -2.61%
        //count-min-4: indicator:- 55.93% - ---
        //count-min-64:-  56.25% - 56.27% = 0.00%
        //random-table:-  55.9% - 55.6% = -0.3%
        //perfect-table:-  56.29% - 55.89% = -0.4%
        // tiny-table:-   55.59 % - ---

        // CountMinSketch: count-min-4 (4-bit), count-min-64 (64-bit)
        // Table: random-table, tiny-table, perfect-table
        const char *sketch = "count-min-4";

        const char *getSketch() {
          return sketch;
        }

        // If increments are conservative by only updating the minimum counters for CountMin sketches
        bool countminconservative = true;

        bool conservative() {
          return countminconservative;
        }


        class countMin64Settings {
        public:
            countMin64Settings() = default;

            double eps = 0.0001;
            double confidence = 0.99;

            double getEps() {
              return eps;
            }

            double getConfidence() {
              return confidence;
            }
        };


        class countMin4Settings {
        public:
            countMin4Settings() = default;

            // periodic: Resets by periodically halving all counters
            // adaptive: Resets periodically at an adaptive interval
            // incremental: Resets by halving counters in an incremental sweep
            const char *reset = "periodic";
            // The multiple of the maximum size determining the number of counters
            double countersMultiplier = 1.0;

            const char *getReset() {
              return reset;
            }

        double getCountersMultiplier() {
          return countersMultiplier;
        }

        class IncrementalSettings {
        public:
            int interval = 16;

            int getInterval() {
              return interval;
            }
        };

        IncrementalSettings incremental() {
          return *new IncrementalSettings();
        }


        class PeriodicSettings {
        public:
             PeriodicSettings() = default;

            class DoorKeeperSettings {
            public:
                DoorKeeperSettings() = default;

                // When enabled the "counters-multiplier" should be reduced to determine the space savings
                bool enabled = true;

                bool getEnabled() {
                  return enabled;
                }
            };

            DoorKeeperSettings doorkeeper() {
              return *new DoorKeeperSettings();
            }
         };

          PeriodicSettings periodic() {
            return *new PeriodicSettings();
          }
        };

      countMin4Settings countMin4() {
          return *new countMin4Settings;
        }

        countMin64Settings countMin64() {
          return *new countMin64Settings();
        }

    };

    tinyLfuSettings tinyLfu() {
      return *new tinyLfuSettings();
    }

    class hillClimberWindowTinyLfuSettings {
    public:
        //adam - 54.67%/56.05% - 54.47% = +0.2%
        //Nadam - 54.616%/56.0% - 54.58% = +0.03%
        //simple-climber - 54.83% - 55.58%/55.85% = +0.02%
        //stochastic 54.29%/56.11% - 55.49% = ++0.2%
        //ams-grad 55.40%/55.68% - 54.41% = +1%
        const char *climberSketch = "ams-grad";
        const char * getClimberSketch(){
          return  climberSketch;
        }

        hillClimberWindowTinyLfuSettings() = default;
        // The initial percentage for the MAIN space (PROBATION + PROTECTED)
        double percentMain = 0.99;
        // The initial percentage for the PROTECTED MAIN queue
        double percentMainProtected = 0.80;

        class adamSettings {
        public:
            //The size of the sample period (1.0 = 100%)
            double percentSample = 0.05;
            // The decay rate of the momentum
            double beta1 = 0.9;
            // The decay rate of the velocity
            double   beta2 = 0.999;
            // The fuzz factor for stability
            double  epsilon = 1e-8;
            adamSettings() = default;

            // The percent of the total size to adapt the window by
            double percentPivot = 0.005;
        };


        class stochasticGradientDescentSettings {
        public:
            stochasticGradientDescentSettings()= default;
            // The percent of the total size to adapt the window by
            double percentPivot = 0.005;
            // The size of the sample period (1.0 = 100%)
            double percentSample = 0.05;
            //none, momentum, nesterov
            // double acceleration = momentum; /* watch out*/
            // The force of acceleration
            double beta = 0.9;
        };


        class nadamSettings {
        public:
            nadamSettings() = default;
            // The percent of the total size to adapt the window by
            double percentPivot = 0.005;
            //The size of the sample period (1.0 = 100%)
            double percentSample = 0.05;
            // The decay rate of the momentum
            double beta1 = 0.9;
            // The decay rate of the velocity
            double   beta2 = 0.999;
            // The fuzz factor for stability
            double  epsilon = 1e-8;
        };

        class amsgradSettings {
        public:
            amsgradSettings() = default;
            // The percent of the total size to adapt the window by
            double percentPivot = 0.005;
            // The size of the sample period (1.0 = 100%)
            double percentSample = 0.05;
            // The decay rate of the momentum
            double beta1 = 0.9;
            // The decay rate of the velocity
            double  beta2 = 0.999;
            // The fuzz factor for stability
            double epsilon = 1e-8;
        };

        class simpleSettings {
        public:
            simpleSettings() = default;

            // The percent of the total size to adapt the window by
            double percentPivot = 0.0625;
            // The size of the sample period (1.0 = 100%)
            double percentSample = 10.0;
            // The difference in hit rate percentage to tolerate before changing directions
            double tolerance = 0.0;
            // The rate to decrease the step size to adapt by
            double stepDecayRate = 0.98;
            // The rate to decrease the sampling period
            double sampleDecayRate = 1.0;
            // The difference in hit rate percentage to tolerate before restarting the adaption
            double restartThreshold = 0.05;
        };


        adamSettings adam(){
          return adamSettings();
        }
        nadamSettings nadam(){
          return nadamSettings();
        }
        amsgradSettings amsgrad(){
          return amsgradSettings();
        }
        stochasticGradientDescentSettings stochasticGradientDescent(){
          return stochasticGradientDescentSettings();
        }
        simpleSettings simple(){
          return simpleSettings();
        }
    };

    hillClimberWindowTinyLfuSettings hillClimberWindowTinyLfu(){
      return {};
    }
};

#endif //WTINYLFU_BASICSETTINGS_H
