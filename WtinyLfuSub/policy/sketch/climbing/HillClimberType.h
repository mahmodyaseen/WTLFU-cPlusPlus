//
// Created by mahmo on 8/16/2020.
//

#ifndef WTINYLFU_HILLCLIMBERTYPE_H
#define WTINYLFU_HILLCLIMBERTYPE_H

#include "AbstractClimber.h"
#include "gradient/Adam.h"
#include "gradient/Nadam.h"
#include "gradient/Stochastic.h"
#include "gradient/AmsGrad.h"
#include "hill/SimpleClimber.h"
#include "../../PolicyStats.h"
//#include " ../../../../../BasicSettings.h"

/*
 * get the chosen adaptive version,
 * change the comparstion that suits you to include your own adaptive algorithm
 *
 */

static bool compareStrings1(string s1, string s2) {
  int compare = s1.compare(s2);
  if (compare != 0)
    return false;
  return true;
}

class HillClimberType : public HillClimber{
    AbstractClimber* climber;
    policyStats * policystats;
    BasicSettings* settings;

    AbstractClimber* climber1(BasicSettings* settings){
      const char *type = settings->hillClimberWindowTinyLfu().getClimberSketch();
      std::string myString(type);
      if (compareStrings1(type, "adam")) {
        return new Adam(settings);
      } else if(compareStrings(type, "Nadam")){
        return new Nadam(settings);
      }else if(compareStrings1(type, "simple-climber")){
        return new SimpleClimber(settings);
      }else if (compareStrings(type, "stochastic")){
        return new Stochastic(settings);
      }else if (compareStrings(type, "ams-grad")){
        return new AmsGrad(settings);
      }else{
        throw std::invalid_argument("Unknown sketch type in TinyLFU");
      }
      return nullptr ;
    }

public:
    HillClimberType(policyStats *stats, BasicSettings* settings):
            climber(climber1(settings))
    {
      this->settings = settings;
      this->policystats= stats;
    }

    ~HillClimberType(){
      delete climber;
    }

    void onHit(long key, QueueType queue, bool isFull) override{
      climber->onHit(key,queue,isFull);
    }

    void onMiss(long key, bool isFull) override {
      climber->onMiss(key,isFull);
    }

    Adaptation adapt(double windowSize, double probationSize, double protectedSize, bool isFull) override{
      return climber->adapt(windowSize,probationSize,protectedSize,isFull);
    }

};


#endif //WTINYLFU_HILLCLIMBERTYPE_H
