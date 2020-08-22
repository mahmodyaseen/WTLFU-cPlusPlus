//
// Created by mahmo on 8/16/2020.
//

#ifndef WTINYLFU_HILLCLIMBERWINDOWTINYLFUPOLICY_H
#define WTINYLFU_HILLCLIMBERWINDOWTINYLFUPOLICY_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "../../Policy.h"
#include "../../../Admission/TinyLfu.h"
#include "../../../Admission/Admittor.h"
#include "HillClimber.h"
#include "HillClimberType.h"
#include "AbstractClimber.h"
#include "../../PolicyStats.h"
#include "../../../BasicSettings.h"
#include <unordered_map>

#define min_value -2147483648

class hillNode {
public:
    long key;

    HillClimber::QueueType queue;
    hillNode* prev;
    hillNode* next;

    /** Creates a new sentinel node. */
    hillNode() {
      this->key = min_value;
      this->queue = HillClimber::NONE;
      this->prev = this;
      this->next = this;
    }

    ~hillNode(){
      key = min_value;
      queue = HillClimber::NONE;
      next = prev = nullptr;
    }

    bool operator==(const hillNode& node){
      return this->key == node.key;
    }

    /** Creates a new, unlinked node. */
    hillNode(long key, HillClimber::QueueType queue) {
      this->queue = queue;
      this->key = key;
      this->prev = this;
      this->next = this;
    }
};

class HillClimberWindowTinyLfuPolicy: public KeyOnlyPolicy{
    HillClimber* climber;
    Admittor* admittor;
    policyStats* Pstats;
    int maximumSize;
    hillNode* headWindow;
    hillNode* headProbation;
    hillNode* headProtected;
    int maxWindow;
    int maxProtected;
    int windowSize;
    int protectedSize;

    list<hillNode>* hillWindowList;
    list<hillNode>* hillProbList;
    list<hillNode>* hillProtectList;
    unordered_map<long, hillNode>* data;

    void onMiss(long key) {
      hillNode node = hillNode(key, AbstractClimber::WINDOW);
      hillWindowList->push_back(node);
      data->insert(make_pair(key, node));
      windowSize++;
      evict();
    }

    void onWindowHit(hillNode* node) {
      for (auto & it : *hillWindowList){
        if(it.key == node->key){
          hillWindowList->remove(it);
          break;
        }
      }
      hillWindowList->push_back(*node);
    }

    void onProbationHit(hillNode* node) {
      for (auto & it : *hillProbList){
        if(it.key == node->key){
          hillProbList->remove(it);
          break;
        }
      }
      node->queue = AbstractClimber::PROTECTED;
      hillProtectList->push_back(*node);
      protectedSize++;
      demoteProtected();
    }

    void demoteProtected() {
      if (protectedSize > maxProtected) {
        hillNode* demote = new hillNode(hillProtectList->front().key, hillProtectList->front().queue);
        demote->prev=hillProtectList->front().prev;
        demote->next=hillProtectList->front().next;
        hillProtectList->pop_front();
        demote->queue = AbstractClimber::PROBATION;
        hillProbList->push_back(*demote);
        data->erase(demote->key);
        data->insert(make_pair(demote->key, *demote));
        protectedSize--;
      }
    }

    void onProtectedHit(hillNode* node) {
      for (auto & it : *hillProtectList){
        if(it.key == node->key){
          hillProtectList->remove(it);
          break;
        }
      }
      hillProtectList->push_back(*node);
    }

    void evict() {
      if (windowSize <= maxWindow) {
        return;
      }
      hillNode* candidate=new hillNode(hillWindowList->front().key, hillWindowList->front().queue);
      candidate->prev=hillWindowList->front().prev;
      candidate->next=hillWindowList->front().next;
      hillWindowList->pop_front();
      windowSize--;

      candidate->queue = AbstractClimber::PROBATION;
      hillProbList->push_back(*candidate);
      data->erase(candidate->key);
      data->insert(make_pair(candidate->key, *candidate));

      if (data->size() > maximumSize) {
        hillNode* victim =new hillNode(hillProbList->front().key, hillProbList->front().queue);
        victim->prev=hillProbList->front().prev;
        victim->next=hillProbList->front().next;
        hillNode* evict = admittor->admit(candidate->key, victim->key) ? victim : candidate;
        if(evict->key == victim->key){
          hillProbList->pop_front();
        }else{
          hillProbList->pop_back();
        }
        data->erase(evict->key);
        Pstats->recordEviction();
      }
    }

    void climb(long key,AbstractClimber::QueueType queue, bool isFull) {
      if (queue == AbstractClimber::NONE) {
        climber->onMiss(key, isFull);
      } else {
        climber->onHit(key, queue, isFull);
      }
      double probationSize = maximumSize - windowSize - protectedSize;
      AbstractClimber::Adaptation adaptation = climber->adapt(windowSize, probationSize, protectedSize, isFull);
      if (adaptation.typee == AbstractClimber::Adaptation::INCREASE_WINDOW) {
        increaseWindow(adaptation.amount);
      } else if (adaptation.typee == AbstractClimber::Adaptation::DECREASE_WINDOW) {
        decreaseWindow(adaptation.amount);
      }
    }

    void increaseWindow(double amount) {
      if (amount < 0.0){
        throw std::invalid_argument(" on increase window something wrong 1");
      }
      if (maxProtected == 0) {
        return;
      }
      double quota = std::min((int)amount, maxProtected);
      int steps = (int) (windowSize + quota) - (int) windowSize;
      windowSize += quota;

      for (int i = 0; i < steps; i++) {
        maxWindow++;
        maxProtected--;
        demoteProtected();
        hillNode* candidate =new hillNode(hillProbList->front().key, hillProbList->front().queue);
        candidate->prev=hillProbList->front().prev;
        candidate->next=hillProbList->front().next;
        hillProbList->pop_front();
        candidate->queue = HillClimber::WINDOW;
        hillWindowList->push_back(*candidate);
        data->erase(candidate->key);
        data->insert(make_pair(candidate->key, *candidate));
      }
      if(windowSize < 0 || maxWindow < 0 || maxProtected< 0){
        throw std::invalid_argument(" on increase window something wrong2");
      }
    }

    void decreaseWindow(double amount) {
      if (amount < 0.0){
        throw std::invalid_argument(" on decrease window something wrong");
      }
      if (maxWindow == 0) {
        return;
      }
      double quota = std::min((int)amount, maxWindow);
      int steps = (int) windowSize - (int) (windowSize - quota);
      windowSize -= quota;

      for (int i = 0; i < steps; i++) {
        maxWindow--;
        maxProtected++;
        hillNode* candidate=new hillNode(hillWindowList->front().key, hillWindowList->front().queue);
        candidate->prev=hillWindowList->front().prev;
        candidate->next=hillWindowList->front().next;
        hillWindowList->pop_front();
        candidate->queue = HillClimber::PROBATION;
        hillProbList->push_back(*candidate);
        data->erase(candidate->key);
        data->insert(make_pair(candidate->key, *candidate));
      }
      if(windowSize < 0 || maxWindow < 0 || maxProtected< 0){
        throw std::invalid_argument(" on decrease window something wrong 2");
      }
    }

    static int countMemByStatus(unordered_map<long,hillNode>* data, HillClimber::QueueType status){
      int count=0;
      for (auto & it : *data){
        if(it.second.queue == status){
          count ++;
        }
      }
      return count;
    }


public:
    HillClimberWindowTinyLfuPolicy(double percentMain,BasicSettings* settings) {
      std::string name = string("sketch.HillClimberWindowTinyLfu");
      this->Pstats = new policyStats(name);
      this->admittor = new TinyLfu(this->Pstats, settings);
      int maxMain = (int) (settings->getMaximumSize() * percentMain);
      this->maxProtected = (int) (maxMain * settings->hillClimberWindowTinyLfu().percentMainProtected);
      this->maxWindow = settings->getMaximumSize() - maxMain;
      this->data = new unordered_map<long,hillNode>();
      this->maximumSize = settings->getMaximumSize();
      this->headProtected = new hillNode();
      this->headProbation = new hillNode();
      this->headWindow = new hillNode();
      this->windowSize=0;
      this->protectedSize=0;
      this->hillWindowList =new list<hillNode>();
      this->hillProbList =new list<hillNode>();
      this->hillProtectList =new list<hillNode>();

      this->climber = new HillClimberType(Pstats,settings);
    }

    ~HillClimberWindowTinyLfuPolicy(){
      delete Pstats;
      delete(admittor);
      delete(data);
      delete(headProtected);
      delete(headWindow);
      delete(headProbation);
      delete hillWindowList;
      delete hillProbList;
      delete hillProtectList;
    }

    void record(long key) {
      bool isFull = (data->size() >= maximumSize);
      Pstats->recordOperation();
      hillNode* node = &data->find(key)->second;
      HillClimber::QueueType queue = AbstractClimber::NONE;
      if( node == &data->end()->second) {
        admittor->record(key);
        onMiss(key);
        Pstats->recordMiss();
      } else {
        admittor->record(key);
        queue = node->queue;
        if (queue == AbstractClimber::WINDOW) {
          onWindowHit(node);
          Pstats->recordHit();
        } else if (queue == AbstractClimber::PROBATION) {
          onProbationHit(node);
          Pstats->recordHit();
        } else if (queue == AbstractClimber::PROTECTED) {
          onProtectedHit(node);
          Pstats->recordHit();
        } else {
          throw  std::invalid_argument("shiiiit  IllegalStateException hillClimber");
        }
      }
      climb(key, queue, isFull);
    }

    void finished() {
      long windowSize = countMemByStatus(data, HillClimber::WINDOW);
      long probationSize = countMemByStatus(data, HillClimber::PROBATION);
      long protectedSize = countMemByStatus(data, HillClimber::PROTECTED);

      if(!(windowSize == this->windowSize)){
        throw std::invalid_argument("bad insuer 1");
      }else if(!(protectedSize == this->protectedSize)){
        throw std::invalid_argument("bad insuer 2");
      }else if(!(probationSize == data->size() - this->windowSize - this->protectedSize)){
        throw std::invalid_argument("bad insuer 3");
      }else if( !(data->size() <= this->maximumSize)){
        throw std::invalid_argument("bad insuer");
      }
    }

    policyStats* stats() override {
      return this->Pstats;
    }
};

#endif //WTINYLFU_HILLCLIMBERWINDOWTINYLFUPOLICY_H
