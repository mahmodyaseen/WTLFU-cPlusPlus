//
// Created by mahmo on 8/9/2020.
//

#ifndef WTINYLFU_POLICY_H
#define WTINYLFU_POLICY_H

#include "PolicyStats.h"
#include "AccessEvent.h"
#include <set>

class Policy {
public:
    enum Characteristic {
        WEIGHTED
    };


    virtual std::set<Characteristic> characteristics() =0;


    virtual void record(AccessEvent event) =0;


    virtual void finished() {}


    virtual policyStats *stats() =0;

};

class KeyOnlyPolicy: public Policy {

    virtual std::set<Characteristic> characteristics() override {
      return std::set<Characteristic> ();
    }

    virtual void record(AccessEvent event) override {
      record(event.getKey());
    }

    virtual void record(long key) = 0;
};

#endif //WTINYLFU_POLICY_H
