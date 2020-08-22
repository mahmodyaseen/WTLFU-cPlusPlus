//
// Created by mahmo on 8/17/2020.
//

#ifndef WTINYLFU_HASHEDITEM_H
#define WTINYLFU_HASHEDITEM_H

#include <iostream>
#include <cstring>
#include<string>
#include <bitset>
#include <cstddef>

typedef std::bitset<8> byte;

using namespace std;
class HashedItem{
public:
    int set;
    byte chainId;
    byte fingerPrint;
    long value;

    HashedItem(int set, int chainId, int fingerPrint, long val){
      this->set=set;
      this->chainId = chainId;
      this->fingerPrint=fingerPrint;
      this->value=val;
    }

//    string toString(){
//        return "BucketId: "+  to_string(set) +  " chainID: "<< char(chainId) << " fingerprint: "<< fingerPrint);
//    }
};

#endif //WTINYLFU_HASHEDITEM_H
