//
// Created by mahmo on 8/17/2020.
//

#ifndef WTINYLFU_PERFECTFREQUENCY_H
#define WTINYLFU_PERFECTFREQUENCY_H


#include <iostream>
#include <unordered_map>
#include "../Frequency.h"
#include "../../BasicSettings.h"

/**
 * A the perfect frequency with aging performed using a periodic reset.
 */

using namespace std;

class perfectFrequency: public Frequency {
private:;
    int sampleSize;
    int size;
    unordered_map<long, int>* count;

    void reset()  {
      for (auto x : *count)  {
        x.second= ((x.second) /2 );
      }
      this->size = (size / 2);
    }

public:

    perfectFrequency(BasicSettings* settings) {
      sampleSize = 10 *  settings->getMaximumSize();
      count = new unordered_map<long,int>();
      size =0;
    }

    ~perfectFrequency(){
      delete count;
    }

    int frequency(long e) override {
      return count->find(e)->second;
    }

    void increment(long e) override {
      if(count->find(e) == count->end()){
        count->insert(make_pair(e,1));
      }else {
        int number =count->find(e)->second;
        count->erase(e);
        count->insert(make_pair(e, (number + 1)));
      }
      size++;
      if (size == sampleSize) {
        reset();
      }
    }

};

#endif //WTINYLFU_PERFECTFREQUENCY_H
