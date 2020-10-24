
#include <fstream>
#include <string>
#include <regex>
#include <iostream>
#include "policy/PolicyStats.h"
#include "policy/AccessEvent.h"
//#include <list>
#include "policy/sketch/WindowTinyLfuPolicy.h"
#include "policy/sketch/climbing/HillClimberWindowTinyLfuPolicy.h"

using namespace std;

int main(int argc,char* argv[]){
  const char* path = argv[1];
  /* any of count-min-4
   * periodic
   * incremental
   * climber
   * count-min-64
   * random-table
   * perfect-table
   * HillClimber:-
   * Adam
   * Nadam
   * AmsGrad
   * Stochastic
   * simple climber
   */
  std::string myType(argv[2]);

  ifstream infile;
  long  reqs = 0;
  long  id;

  cout << "running..." << endl;

  infile.open(path);

  BasicSettings *settings= new BasicSettings();
  list<AccessEvent> *events = new list<AccessEvent>;

  while (infile >> id  )
  {
    AccessEvent event(id);
    reqs++;
    events->push_back(event);
  }
  bool sketchFlag = false;
//  policyStats* policystats;

/*
 * Choose the version to run, adaptive or sketch(defult 1%)
 */

  if (compareStrings(myType, "adaptive")) {
    HillClimberWindowTinyLfuPolicy policyNew(0.99 , settings);
    sketchFlag = false;
    for (list<AccessEvent>::iterator it=events->begin(); it != events->end(); ++it)
      policyNew.record(it->getKey());

    policyStats* policystats = policyNew.stats();
    cout << policystats->getName() <<endl;
    infile.close();
    cout <<"sketch type:- "<< settings->tinyLfu().getSketch() ;
    cout << ", hillClimber.sketch:- "<< settings->hillClimberWindowTinyLfu().climberSketch<< endl;
    cout << "--------------------------------------------->> "<<endl;
    cout << ">> hit rate:- " << policystats->hitRate() << endl;
    cout << ">> eviction count:- " << policystats->getEvictionCount()<< endl;
    cout <<">> hit count:- "<< policystats->getHitCount()<< endl ;
    cout <<">> miss count:- " << policystats->getMissCount()<< endl;
    cout << ">> Evictions:- " << policystats->getEvictionCount() << endl;
    cout <<">> time:- " << policystats->getStopwatch().elapsed()<< endl;

  } else if (compareStrings(myType, "sketch")) {
    WindowTinyLFUPolicy policyNew(0.99, settings);
    sketchFlag = true;
    for (list<AccessEvent>::iterator it = events->begin(); it != events->end(); ++it)
      policyNew.record(it->getKey());

    policyStats* policystats = policyNew.stats();
    cout << policystats->getName() <<endl;
    infile.close();
    cout <<"sketch type:- "<< settings->tinyLfu().getSketch() ;
    if(sketchFlag == true) {
      if (compareStrings(string(settings->tinyLfu().getSketch()), "count-min-4")) {
        cout << ", " << settings->tinyLfu().countMin4().reset ;
      }
      cout<< endl;
    }
    cout << "--------------------------------------------->> "<<endl;
    cout << ">> hit rate:- " << policystats->hitRate() << endl;
    cout << ">> eviction count:- " << policystats->getEvictionCount()<< endl;
    cout <<">> hit count:- "<< policystats->getHitCount()<< endl ;
    cout <<">> miss count:- " << policystats->getMissCount()<< endl;
    cout << ">> Evictions:- " << policystats->getEvictionCount() << endl;
    cout <<">> time:- " << policystats->getStopwatch().elapsed()<< endl;

  }




  return 0;
}