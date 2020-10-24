//
// Created by mahmo on 8/17/2020.
//

#ifndef WTINYLFU_RANDOMREMOVALFREQUENCYTABLE_H
#define WTINYLFU_RANDOMREMOVALFREQUENCYTABLE_H


#include <cstdio>      /* printf, NULL */
#include <cstdlib>
#include "../Frequency.h"
#include "../../BasicSettings.h"
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <ctime>

/**
 * A probabilistic multiset for estimating the popularity of an element within a time window. The
 * maximum frequency of an element. The size of the sample in relation to the cache size can be
 * controlled with a sample factor. Instead of halving the popularity of elements a random element
 * is dropped when table is full.
 *
 * This class is used to check the feasibility of using TinyTable instead of CountMin Sketch.
 *
 * the build is based on the Java version of (Gil Einiziger) from Caffeiene library
 * gilg1983@gmail.com (Gil Einziger)
 */

class RandomRemovalFrequencyTable : public Frequency{
private:
    /** sum of total items */
    int maxSum;
    /** total sum of stored items **/
    int currSum;
    /** controls both the max count and how many items are remembered (the sum) */
    int sampleFactor = 8;
    /** used to dropped items at random */
    float random;
    /** a place holder for TinyTable */
    std::unordered_map<long, int>* table;
public:
    RandomRemovalFrequencyTable(BasicSettings* settings) {
      srand(time(NULL));
      maxSum = sampleFactor * settings->getMaximumSize();
      srand(settings->getRandomSeed());
      random = (float)rand();
      table = new std::unordered_map<long,int> (maxSum);
      currSum=0;
    }

    ~RandomRemovalFrequencyTable(){
      delete table;
    }

    int frequency(long e) override {
      if(table->find(e) == table->end()){
        return 0;
      }else {
        return table->find(e)->second;
      }
    }


    void increment(long e) override {
        int value;
        if(table->find(e) == table->end()){
           value=1;
        }else {
           value= table->find(e)->second +1 ;
        }
        // if the value is big enough there is no point in dropping a value so we just quit
        if (value > sampleFactor) {
          return;
        }
      if(table->find(e) == table->end()){
        table->insert(make_pair(e,1));
      }else {
        table->erase(e);
        table->insert(make_pair(e, value));
      }
      if (currSum < maxSum) {
        currSum++;
      }
      int maxVal = 2147483648;
      long key;
      if (currSum == maxSum) {
        list<long>* array = new list<long>;
        for (auto & it : *table){
          if(it.second < maxVal){
            maxVal = it.second;
            key = it.first;
          }
        }
        value = table->erase(key);
        if (value > 1) {
          table->insert(make_pair(e, value - 1));
        }
      }
    }
};


#endif //WTINYLFU_RANDOMREMOVALFREQUENCYTABLE_H
