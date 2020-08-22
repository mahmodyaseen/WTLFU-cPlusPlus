////
//// Created by mahmo on 8/17/2020.
////
//
//#ifndef WTINYLFU_TINYCACHE_H
//#define WTINYLFU_TINYCACHE_H
//
//#include "hashFunctionPraser.h"
//#include <iostream>
//#include <bitset>
//#include <time.h>
//#include <cstddef>
//
//using namespace std;
//typedef std::bitset<8> byte;
//
//class tinyCache{
//private:
//    hashFunctionParser hashFunc = hashFunctionParser(0);
//    int itemsPerSet;
//    long* cache;
//    double rnd;
//
//    bool selectVictim(int bucketStart) {
//      srand (time(NULL));
//      rnd = (double)rand() / RAND_MAX;;
//      byte victimOffset = (byte) rnd;
//      int victimChain =
//              tinySetIndexing::getChainAtOffset(hashFunc.fpaux, chainIndex, lastIndex, victimOffset.to_ulong());
//
//      if (tinySetIndexing::chainExist(chainIndex[hashFunc.fpaux.set], victimChain)) {
//        replace(hashFunc.fpaux, (byte) victimChain, bucketStart, victimOffset.to_ulong());
//        return true;
//      } else {
//        throw ("Failed to replace");
//      }
//    }
//
//    void replaceItems( int idx, long value, int start, int delta) {
//      start += idx;
//      long $;
//      do {
//        $ = this->cache[start];
//        this->cache[start] = value;
//        value = $;
//        start += delta;
//      } while (value != 0);
//    }
//
//public:
//    long* chainIndex;
//    long* lastIndex;
//
//    tinyCache(int nrSets, int itemsPerSet, int randomSeed) {
//      lastIndex = new long[nrSets];
//      chainIndex = new long[nrSets];
//      this->itemsPerSet = itemsPerSet;
//      hashFunc =hashFunctionParser(nrSets);
//      cache = new long[nrSets * itemsPerSet];
//      srand (time(nullptr));
//      rnd = (double)rand() / RAND_MAX;;
//    }
//
//    int getLength(long * array){
//      int i =0;
//      while(array[i] != NULL){
//        i++;
//      }
//      return i;
//    }
//
//    bool contains(long item) {
//      hashFunc.createHash(item);
//      if (!tinySetIndexing::chainExist(chainIndex[hashFunc.fpaux.set], hashFunc.fpaux.chainId.to_ulong())) {
//        return false;
//      }
//      tinySetIndexing::getChain(hashFunc.fpaux, chainIndex, lastIndex);
//      int offset = this->itemsPerSet * hashFunc.fpaux.set;
//      tinySetIndexing::chainStart += offset;
//      tinySetIndexing::chainEnd += offset;
//      // Gil : I think some of these tests are, I till carefully examine this function when I have
//      // time. As far as I understand it is working right now.
//      while (tinySetIndexing::chainStart <= tinySetIndexing::chainEnd) {
//        try {
//          if (cache[tinySetIndexing::chainStart % getLength(this->cache)] == hashFunc.fpaux.value) {
//            return true;
//          }
//          tinySetIndexing::chainStart++;
//        } catch (exception e) {
////                System.out.println(" length: " + cache.length + " Access: " + TinySetIndexing.chainStart);
//        }
//      }
//      return false;
//    }
//
//    int replace(HashedItem fpaux, byte victim, int bucketStart, int removedOffset) {
//      byte chainId = fpaux.chainId;
//      fpaux.chainId = victim;
//      this->cache[bucketStart + removedOffset] = 0;
//      tinySetIndexing::removeItem(fpaux, chainIndex, lastIndex);
//      fpaux.chainId = chainId;
//      int idxToAdd = tinySetIndexing::addItem(fpaux, chainIndex, lastIndex);
//      int delta = (removedOffset < idxToAdd) ? -1 : 1;
//      replaceItems(idxToAdd, fpaux.value, bucketStart, delta);
//      return removedOffset;
//    }
//
//    bool addItem(long item) {
//      hashFunc.createHash(item);
//      int bucketStart = this->itemsPerSet * hashFunc.fpaux.set;
//      if (cache[bucketStart + this->itemsPerSet - 1] != 0) {
//        return selectVictim(bucketStart);
//      }
//      int idxToAdd = tinySetIndexing::addItem(hashFunc.fpaux, chainIndex, lastIndex);
//      this->replaceItems(idxToAdd, hashFunc.fpaux.value, bucketStart, 1);
//      return false;
//    }
//
//
//};
//
//#endif //WTINYLFU_TINYCACHE_H
