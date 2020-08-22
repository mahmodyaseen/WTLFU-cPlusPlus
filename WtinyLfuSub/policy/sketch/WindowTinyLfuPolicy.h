

//
// Created by mahmo on 8/9/2020.
//

#ifndef WTINYLFU_WINDOWTINYLFUPOLICY_H
#define WTINYLFU_WINDOWTINYLFUPOLICY_H


#include "../PolicyStats.h"
#include "../Policy.h"
#include "../../Admission/Admittor.h"
#include "../../Admission/TinyLfu.h"
#include "../../BasicSettings.h"
#include <list>
#include <unordered_map>

#define min_value -2147483648

#include <cstdio>
#include <cstdlib>
#include <iostream>


enum Status {
    NONE, WINDOW, PROBATION, PROTECTED
}status ;

class Node {
public:
    long key;
    Status status;
    Node* prev;
    Node* next;

    Node(){
      this->key = min_value;
      this->status = NONE;
      this->prev = this;
      this->next = this;
    }

    Node(long key, Status status) {
      this->status = status;
      this->key = key;
      this->prev = this;
      this->next = this;
    }

    ~Node() {
      key = min_value;
      status = NONE;
      next = prev = nullptr;
    }

    bool operator==(const Node& node){
      return this->key == node.key;
    }

};

class WindowTinyLFUPolicy : public KeyOnlyPolicy{
private:
    policyStats* policystats;
    TinyLfu* tinylfu ;
    int maximumSize;
    Node* headWindow;
    Node* headProbation;
    Node* headProtected;
    int maxWindow;
    int maxProtected;
    int sizeWindow;
    int sizeProtected;

    list<Node>* windowList;
    list<Node>* probList;
    list<Node>* protectList;
    unordered_map<long, Node>* data;

public:
    WindowTinyLFUPolicy(double percentMain, BasicSettings *settings) {
      std::string name = string("sketch.WindowTinyLfu");
      this->policystats = new policyStats(name);
      this->tinylfu = new TinyLfu(this->policystats, settings);
      int maxMain = (int) (settings->getMaximumSize() * percentMain) ;
      this->maxProtected = (int) (maxMain * settings->windowTinyLfu().percentMainProtected);
      this->maxWindow = settings->getMaximumSize() - maxMain;
      this->data = new unordered_map<long,Node>();
      this->windowList =new list<Node>();
      this->probList =new list<Node>();
      this->protectList =new list<Node>();
      this->maximumSize = settings->getMaximumSize();
      this->headProtected = new Node();
      this->headProbation = new Node();
      this->headWindow = new Node();
      this->sizeWindow=0;
      this->sizeProtected=0;
    }

    ~WindowTinyLFUPolicy(){
      delete policystats;
      delete(tinylfu);
      delete(data);
      delete(headProtected);
      delete(headWindow);
      delete(headProbation);
      delete windowList;
      delete probList;
      delete protectList;
    }

    policyStats* stats() override  {
      return this->policystats;
    }

    void record(long key) override {
      this->policystats->recordOperation();
        Node* node = &data->find(key)->second;
        if( node == &data->end()->second) {
          onMiss(key);
          policystats->recordMiss();
        } else if (node->status == WINDOW) {
          onWindowHit(node);
          policystats->recordHit();
        } else if (node->status == PROBATION) {
          onProbationHit(node);
          policystats->recordHit();
        } else if (node->status == PROTECTED) {
          onProtectedHit(node);
          policystats->recordHit();
        }else {
          throw std::invalid_argument("shiiiit  IllegalStateException");
        }
    }

    void onMiss(long key) {
      tinylfu->record(key);

      Node node = Node(key, WINDOW);
      windowList->push_back(node);
      data->insert(make_pair(key, node));
      sizeWindow++;
      evict();
    }

    void onWindowHit(Node* node) {
      tinylfu->record(node->key);
      for (auto & it : *windowList){
        if(it.key == node->key){
          windowList->remove(it);
          break;
        }
      }
      windowList->push_back(*node);
    }

    void onProbationHit(Node* node) {
      tinylfu->record(node->key);
      for (auto & it : *probList){
        if(it.key == node->key){
          probList->remove(it);
          break;
        }
      }
      node->status = PROTECTED;
      protectList->push_back(*node);
      sizeProtected++;

      if (sizeProtected > maxProtected) {
        Node* demote = new Node(protectList->front().key, protectList->front().status);
        demote->prev=protectList->front().prev;
        demote->next=protectList->front().next;
        protectList->pop_front();
        demote->status = PROBATION;
        probList->push_back(*demote);
        data->erase(demote->key);
        data->insert(make_pair(demote->key, *demote));
        sizeProtected--;
      }
    }

    void onProtectedHit(Node* node) {
      tinylfu->record(node->key);
      for (auto & it : *protectList){
        if(it.key == node->key){
          protectList->remove(it);
          break;
        }
      }
      protectList->push_back(*node);
    }

    void evict() {
      if (sizeWindow <= maxWindow) {
        return;
      }
      Node* candidate=new Node(windowList->front().key, windowList->front().status);
      candidate->prev=windowList->front().prev;
      candidate->next=windowList->front().next;
      windowList->pop_front();
      sizeWindow--;

      candidate->status = PROBATION;

      probList->push_back(*candidate);
      data->erase(candidate->key);
      data->insert(make_pair(candidate->key, *candidate));

      if (data->size() > maximumSize) {
        Node* victim =new Node(probList->front().key, probList->front().status);
        victim->prev=probList->front().prev;
        victim->next=probList->front().next;
        Node* evict = tinylfu->admit(candidate->key, victim->key) ? victim : candidate;
        if(evict->key == victim->key){
          probList->pop_front();
        }else{
          probList->pop_back();
        }
        data->erase(evict->key);
        policystats->recordEviction();
      }
    }

    static int countMemByStatus(unordered_map<long,Node>* data, Status status){
      int count=0;
      for (auto & it : *data){
        if(it.second.status == status){
          count ++;
        }
      }
      return count;
    }

    void finished() {
      long windowSize = countMemByStatus(data, WINDOW);
      long probationSize = countMemByStatus(data, PROBATION);
      long protectedSize = countMemByStatus(data, PROTECTED);

      if(!(windowSize == sizeWindow)){
        throw std::invalid_argument("bad insuer 1");
      }else if(!(protectedSize == sizeProtected)){
         throw std::invalid_argument("bad insuer 2");
       }else if(!(probationSize == data->size() - windowSize - protectedSize)){
         throw std::invalid_argument("bad insuer 3");
       }else if( !(data->size() <= maximumSize)){
       throw std::invalid_argument("bad insuer");
      }
    }

};

#endif //WTINYLFU_WINDOWTINYLFUPOLICY_H
