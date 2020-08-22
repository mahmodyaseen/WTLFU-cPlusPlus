//
// Created by mahmo on 8/17/2020.
//

#ifndef WTINYLFU_HASHFUNCTIONPRASER_H
#define WTINYLFU_HASHFUNCTIONPRASER_H

#include <iostream>
#include <string>
#include "TinySetIndexing.h"

typedef std::bitset<8> byte;
#define MAX_VALUE_P 0x7fffffff

using namespace std;

class hashFunctionParser {
    // currently chain is bounded to be 64.
    int fpSize = 8; // this implementation assumes byte.
    int fpMask = 255; // (all bits in byte are 1, (logical value of -1));
    long chainMask = 63L; // (6 first bit are set to 1).
    int nrSets;
    unsigned int Seed64 = 0xe17a1465;
    unsigned long long m = 0xc6a4a7935bd1e995L;
    int r = 47;

public:
    HashedItem fpaux = HashedItem(0,0,0,0); // used just to avoid allocating new memory as a return value.

    explicit hashFunctionParser(int nrSets) {
      this->nrSets = nrSets;
      fpaux = HashedItem(fpMask, fpMask, fpMask, 0L);
    }

    HashedItem createHash(long item) {
      unsigned long long h = (Seed64 ^ m);
      item *= m;
      item ^= unsigned (item) >> r;
      item *= m;
      h ^= item;
      h *= m;
      fpaux.fingerPrint = h;
      // the next line is a dirty fix as I do not want the value of 0 as a fingerprint.
      // It can be eliminated if we want very short fingerprints.
      fpaux.fingerPrint = (fpaux.fingerPrint == 0L) ? 1 : fpaux.fingerPrint;
      h >>= unsigned(fpSize);
      fpaux.chainId =  (h & chainMask);
      h >>= 6;
      fpaux.set = (int) ((h & MAX_VALUE_P) % nrSets);

      fpaux.value = (item << 1) | 1;
      if (item == 0) {
        fpaux.value = 1;
      }
      return fpaux;
    }
};

#endif //WTINYLFU_HASHFUNCTIONPRASER_H
