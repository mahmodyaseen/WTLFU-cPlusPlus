//
// Created by mahmo on 8/6/2020.
//

#ifndef WTINYLFU_FREQUENCY_H
#define WTINYLFU_FREQUENCY_H

#include <string>

class Frequency {
public:

    virtual int frequency(long e) =0;

    virtual void increment(long e)=0;

     virtual void reportMiss() {}

};
#endif //WTINYLFU_FREQUENCY_H
