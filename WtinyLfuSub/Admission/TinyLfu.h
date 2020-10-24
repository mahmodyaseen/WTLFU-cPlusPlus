//
// Created by mahmo on 8/7/2020.
//

#ifndef WTINYLFU_TINYLFU_H
#define WTINYLFU_TINYLFU_H


#include "Admittor.h"
#include "../policy/PolicyStats.h"
#include "Frequency.h"
#include "../BasicSettings.h"
#include "countmin4/IncrementalResetCountMin4.h"
#include "countmin4/PeriodicResetCountMin4.h"
#include "countmin4/ClimberResetCountMin4.h"
#include "countmin64/countMin64TLfu.h"
#include "table/RandomRemovalFrequencyTable.h"
#include "perfect/PerfectFrequency.h"
#include "tinycache/TinyCacheAdapter.h"


#include <string>
#include <memory>

/**
 * Admits new entries based on the estimated frequency of its historic use.
 *
 * this the main sketch in the Main cache area, offering multi-sketches.
 * to add a sketch of your own, edit the string compare with a name of your own.
 */

using namespace std;

bool compareStrings(string s1, string s2) {
  int compare = s1.compare(s2);
  return compare == 0;
}

class TinyLfu: public Admittor{
private:
    policyStats *policystats;
    Frequency* sketch;

    Frequency* makeSketch(BasicSettings* settings ) {
      const char *type = settings->tinyLfu().getSketch();
      std::string myString(type);
      if (compareStrings(type, "count-min-4")) {
        const char *reset = settings->tinyLfu().countMin4().getReset();
        std::string myStr(reset);
        if (compareStrings(reset, "periodic")) {
          return new PeriodicResetCountMin4(settings);
        } else if (compareStrings(reset, "incremental")) {
          return new IncrementalResetCountMin4(settings);
        } else if (compareStrings(reset,"climber")) {
          return new ClimberResetCountMin4(settings);
//    } else if (compareStrings(reset,"indicator")) { // bug in the indicator working on it
//        return new IndicatorResetCountMin4(config);
        }
      } else if (compareStrings(type, "count-min-64")) {
        return new cm64TinyLFU(settings);
      } else if (compareStrings(type,"random-table")) {
        return new RandomRemovalFrequencyTable(settings);
//      } else if (compareStrings(type, "tiny-table")) { /// there is problem need to be updated in tinyTable classes
//            return new TinyCacheAdapter(settings);
      } else if (compareStrings(type, "perfect-table")) {
        return new perfectFrequency(settings);
      } else{
        throw std::invalid_argument("Unknown sketch type in TinyLFU");
      }
      return nullptr ;
    }

public:
    TinyLfu( policyStats *stats, BasicSettings* settings ): sketch(makeSketch(settings)) {
      this->policystats= stats;
    }


    ~TinyLfu(){
      delete sketch;
    }

    void record(long key) {
      sketch->increment(key);
    }


    bool admit(long candidateKey, long victimKey) {
      sketch->reportMiss();
      long candidateFreq = sketch->frequency(candidateKey);
      long victimFreq = sketch->frequency(victimKey);
      if (candidateFreq > victimFreq) {
        policystats->recordAdmission();
        return true;
      }
      policystats->recordRejection();
      return false;
    }
};
#endif //WTINYLFU_TINYLFU_H
