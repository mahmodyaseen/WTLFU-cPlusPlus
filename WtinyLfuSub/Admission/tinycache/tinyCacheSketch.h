//
// Created by mahmo on 8/17/2020.
//

#ifndef WTINYLFU_TINYCACHESKETCH_H
#define WTINYLFU_TINYCACHESKETCH_H

#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include "TinySetIndexing.h"
#include "HashFunctionPraser.h"
#include <bitset>

using namespace std;

typedef std::bitset<8> byte;

class tinyCacheSketch{
    long* chainIndex;
    long* lastIndex;
    hashFunctionParser hashFun = hashFunctionParser(0);
    TinySetIndexing indexing;
    int itemsPerSet;
    byte* cache;
    double rnd ;

    void replaceItems(int idx, byte value, int start, int delta) {
      start += idx;
      byte $;
      do {
        $ = this->cache[start];
        this->cache[start] = value;
        value = $;
        start += delta;
      } while (value != 0);
    }

    void selectVictim(int bucketStart) {
      srand (time(NULL));
      rnd = (double)rand() / RAND_MAX;
      byte victimOffset = (byte) rnd;
      int victimChain =
              indexing.getChainAtOffset(hashFun.fpaux, chainIndex, lastIndex, victimOffset.to_ulong());
      if (indexing.chainExist(chainIndex[hashFun.fpaux.set], victimChain)) {
        replace(hashFun.fpaux, (byte) victimChain, bucketStart, victimOffset.to_ulong());
      } else {
        throw ("Failed to replace");
      }
    }

public:
    tinyCacheSketch(int nrSets, int itemsPerSet, int randomSeed) {
      chainIndex = new long[nrSets];
      lastIndex = new long[nrSets];
      this->itemsPerSet = itemsPerSet;
      hashFun = hashFunctionParser(nrSets);
      cache = new byte[nrSets * itemsPerSet];
      srand (time(NULL));
      rnd = (double)rand() / RAND_MAX;;
    }

    int countItem(long item) {
      hashFun.createHash(item);
      int counter= 0;
      if (!((indexing.chainExist(chainIndex[hashFun.fpaux.set], hashFun.fpaux.chainId.to_ulong()))) ){
        return 0;
      }
      indexing.getChain(hashFun.fpaux, chainIndex, lastIndex);
      int offset = this->itemsPerSet * hashFun.fpaux.set;
      indexing.chainStart += offset;
      indexing.chainEnd += offset;
      // Gil : I think some of these tests are, I till carefully examine this function when I have
      // time. As far as I understand it is working right now.
      while (indexing.chainStart <= indexing.chainEnd) {
        try {
          counter += (cache[indexing.chainStart % this->cache->count()] == hashFun.fpaux.fingerPrint) /// maybe wrong
               ? 1
               : 0;
          indexing.chainStart++;

        } catch (exception e) {
//                System.out.println(" length: " + cache->count() + " Access: " + TinySetIndexing.chainStart);
          // e.printStackTrace(); /// somthing wrong
        }
      }
      return counter;
    }

    int replace(HashedItem fpaux, byte victim, int bucketStart, int removedOffset) {
      byte chainId = fpaux.chainId;
      fpaux.chainId = victim;

      this->cache[bucketStart + removedOffset] = 0;

      indexing.removeItem(fpaux, chainIndex, lastIndex);
      fpaux.chainId = chainId;
      int idxToAdd = indexing.addItem(fpaux, chainIndex, lastIndex);
      int delta = (removedOffset < idxToAdd) ? -1 : 1;

      replaceItems(idxToAdd, fpaux.fingerPrint.to_ulong(), bucketStart, delta);

      return removedOffset;
    }

    void addItem(long item) {
      hashFun.createHash(item);
      int bucketStart = this->itemsPerSet * hashFun.fpaux.set;
      if (cache[bucketStart + this->itemsPerSet - 1] != 0) {
        selectVictim(bucketStart);
        return;
      }

      int idxToAdd = indexing.addItem(hashFun.fpaux, chainIndex, lastIndex);
      this->replaceItems(idxToAdd, hashFun.fpaux.fingerPrint, bucketStart, 1);
    }

};

#endif //WTINYLFU_TINYCACHESKETCH_H
