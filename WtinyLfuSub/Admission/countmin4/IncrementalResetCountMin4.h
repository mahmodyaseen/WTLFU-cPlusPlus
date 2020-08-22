//
// Created by mahmo on 8/17/2020.
//

#ifndef WTINYLFU_INCREMENTALRESETCOUNTMIN4_H
#define WTINYLFU_INCREMENTALRESETCOUNTMIN4_H



#include "../countmin4/countMin4.h"
#include "../../BasicSettings.h"


class IncrementalResetCountMin4: public countMin4 {
    int interval;
    int additions;
    int cursor;

    void tryReset(bool added) override {
      if (!added) {
        return;
      }
      additions++;
      if (additions != interval) {
        return;
      }
      int i = cursor & countMin4::tableMask;
      countMin4::table[i] = (unsigned (countMin4::table[i]) >> 1) & countMin4::RESET_MASK;
      cursor++;
      additions = 0;
    }

public:
    IncrementalResetCountMin4(BasicSettings* settings): countMin4(settings) {
      interval = settings->tinyLfu().countMin4().incremental().getInterval();
      cursor = settings->getRandomSeed();
    }

};


#endif //WTINYLFU_INCREMENTALRESETCOUNTMIN4_H
