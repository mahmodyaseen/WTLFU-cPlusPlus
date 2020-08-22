//
// Created by mahmo on 8/7/2020.
//

#ifndef WTINYLFU_POLICYSTATS_H
#define WTINYLFU_POLICYSTATS_H

#include "../Stopwatch.h"
#include <iostream>
#include <cstddef>

using namespace std;

class policyStats {
private:
    stopwatch::Stopwatch stopwatch;

    string name;
//    string hillsketch;
//    string wsketch;
    long hitCount;
    long missCount;
    long hitsWeight;
    long missesWeight;
    double hitPenalty;
    double missPenalty;
    long evictionCount;
    long admittedCount;
    long rejectedCount;
    long operationCount;

public:

    policyStats(string name) {
      if(name.empty())
        return;
      this->name = string(name);
      this->stopwatch = stopwatch::Stopwatch();
       hitCount=0;
       missCount=0;
       hitsWeight=0;
       missesWeight=0;
       hitPenalty=0;
       missPenalty=0;
       evictionCount=0;
       admittedCount=0;
       rejectedCount=0;
       operationCount=0;
    }

    policyStats() {
      this->stopwatch = stopwatch::Stopwatch();
    }

    stopwatch::Stopwatch getStopwatch() {
      return stopwatch;
    }

    basic_string<char> getName() {
      return name;
    }

    void setName(char* name) {
      this->name = (name);
    }

    void recordOperation() {
      operationCount++;
    }

    long getOperationCount() {
      return operationCount;
    }

    void addOperations(long operations) {
      operationCount += operations;
    }

    void recordHit() {
      hitCount++;
    }

    long getHitCount() {
      return hitCount;
    }

    void addHits(long hits) {
      hitCount += hits;
    }

    void recordWeightedHit(int weight) {
      hitsWeight += weight;
      recordHit();
    }

    long getHitsWeight() {
      return hitsWeight;
    }

    void recordHitPenalty(double penalty) {
      hitPenalty += penalty;
    }

    double getHitPenalty() {
      return hitPenalty;
    }

    void recordMiss() {
      missCount++;
    }

    long getMissCount() {
      return missCount;
    }

    void addMisses(long misses) {
      missCount += misses;
    }

    void recordWeightedMiss(int weight) {
      missesWeight += weight;
      recordMiss();
    }

    long getMissesWeight() {
      return missesWeight;
    }

    void recordMissPenalty(double penalty) {
      missPenalty += penalty;
    }

    double getMissPenalty() {
      return missPenalty;
    }

    long getEvictionCount() {
      return evictionCount;
    }

    void recordEviction() {
      evictionCount++;
    }

    void addEvictions(long evictions) {
      evictionCount += evictions;
    }

    long requestCount() {
      return hitCount + missCount;
    }

    long requestsWeight() {
      return hitsWeight + missesWeight;
    }

    long admissionCount() {
      return admittedCount;
    }

    void recordAdmission() {
      admittedCount++;
    }

    long rejectionCount() {
      return rejectedCount;
    }

    void recordRejection() {
      rejectedCount++;
    }

    double totalPenalty() {
      return hitPenalty + missPenalty;
    }

    double hitRate() {
      long requestCount = this->requestCount();
      return (requestCount == 0) ? 1.0 : (double) hitCount / requestCount;
    }

    double weightedHitRate() {
      long requestsWeight = this->requestsWeight();
      return (requestsWeight == 0) ? 1.0 : (double) hitsWeight / requestsWeight;
    }

    double missRate() {
      long requestCount = this->requestCount();
      return (requestCount == 0) ? 0.0 : (double) missCount / requestCount;
    }

    double weightedMissRate() {
      long requestsWeight = this->requestsWeight();
      return (requestsWeight == 0) ? 1.0 : (double) missesWeight / requestsWeight;
    }

    double admissionRate() {
      long candidateCount = admittedCount + rejectedCount;
      return (candidateCount == 0) ? 1.0 : (double) admittedCount / candidateCount;
    }

    double complexity() {
      long requestCount = this->requestCount();
      return (requestCount == 0) ? 0.0 : (double) operationCount / requestCount;
    }

    double avergePenalty() {
      long requestCount = this->requestCount();
      return (requestCount == 0) ? 0.0 : totalPenalty() / requestCount;
    }

    double avergeHitPenalty() {
      return (hitCount == 0) ? 0.0 : hitPenalty / hitCount;
    }

    double averageMissPenalty() {
      return (missCount == 0) ? 0.0 : missPenalty / missCount;
    }


//    public String toString() {
//            return MoreObjects.toStringHelper(this).addValue(name).toString();
//        }
//    }

};


#endif //WTINYLFU_POLICYSTATS_H
