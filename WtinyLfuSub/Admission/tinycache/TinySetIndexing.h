//
// Created by mahmo on 8/17/2020.
//

#ifndef WTINYLFU_TINYSETINDEXING_H
#define WTINYLFU_TINYSETINDEXING_H

#include <iostream>
#include <string>
#include "HashedItem.h"

using namespace std;
#define MAX(a ,b)   (a > b ? a : b)


unsigned int countSetBits(unsigned int n)
{
  unsigned int count = 0;
  while (n) {
    count += n & 1;
    n >>= 1;
  }
  return count;
}

class TinySetIndexing{

    long extendZero(long isLastIndex, int offset){
      long constantPartMask = (1L << offset) -1;
      return((isLastIndex& constantPartMask)
             | ((isLastIndex << 1L)
                & (~(constantPartMask))
                &(~(1L << offset))));
    }

     long shrinkOffset(long isLastIndex, int offset){
      long conMask = ((1L << offset)-1);
      return (isLastIndex & conMask) | (unsigned((~conMask) & isLastIndex) >> 1 );
    }

public:
     int chainStart;
     int chainEnd;

     int rank(long index, int bitNum){
      return countSetBits(index & ~(-1L << bitNum));
    }

     int getChainStart(HashedItem hashed, long* chainIndex, long* isLastIndex){
      int requeirdChainNumber= rank(chainIndex[hashed.set],hashed.chainId.to_ulong());
      int currentChainNumber = rank(isLastIndex[hashed.set], requeirdChainNumber);
      int currentOffset = requeirdChainNumber;
      long tempIsLastIndex = unsigned(isLastIndex[hashed.set])>>requeirdChainNumber;
      while(currentChainNumber < requeirdChainNumber){
        currentChainNumber += ((int) tempIsLastIndex)&1;
        currentOffset++;
        tempIsLastIndex >>=1;
      }
      return currentOffset;
    }

     int getChain(HashedItem item, long * chainIndex, long* isLastIndex){
      int requeirdChainNumber= rank(chainIndex[item.set],item.chainId.to_ulong());
      int currentChainNumber = rank(isLastIndex[item.set], requeirdChainNumber);
      int currentOffset = requeirdChainNumber;
      long tempIsLastIndex = unsigned(isLastIndex[item.set])>>requeirdChainNumber;
      while(currentChainNumber < requeirdChainNumber){
        currentChainNumber += ((int) tempIsLastIndex)&1;
        currentOffset++;
        tempIsLastIndex >>=1;
      }
      chainStart = currentOffset;
      while ((tempIsLastIndex & 1L) ==0){
        currentOffset++;
        tempIsLastIndex >>= 1;
      }
      chainEnd = currentOffset;
      return currentOffset;
    }

     bool chainExist(long chainIndex, int chainId){
      return (chainIndex | (1L << chainId)) == chainIndex;
    }

     int getChainAtOffset(HashedItem fpaux,
                                long* chainIndex, long* isLastIndex, int offset) {
      int nonEmptyChainsToSee = rank(isLastIndex[fpaux.set], offset);
      int nonEmptyChainSeen = rank(chainIndex[fpaux.set], nonEmptyChainsToSee);
      for (int i = nonEmptyChainsToSee; i <= 64;) {
        if (chainExist(chainIndex[fpaux.set], i)
            && (nonEmptyChainSeen == nonEmptyChainsToSee)) {
          return i;
        }
        i += MAX(1, nonEmptyChainsToSee - nonEmptyChainSeen);
        nonEmptyChainSeen = rank(chainIndex[fpaux.set], i);
      }
      throw ("exception tinyIndexing 98");
    }

      int addItem(HashedItem fpaux, long* chainIndex, long* lastIndex) {
      int offset = getChainStart(fpaux, chainIndex, lastIndex);
      long mask = 1L << fpaux.chainId.to_ulong();
      lastIndex[fpaux.set] = extendZero(lastIndex[fpaux.set], offset);
      // if the item is new...
      if ((mask | chainIndex[fpaux.set]) != chainIndex[fpaux.set]) {
        // add new chain to IO.
        chainIndex[fpaux.set] |= mask;
        // mark item as last in isLastIndex.
        lastIndex[fpaux.set] |= (1L << offset);
      }
      return offset;
    }

     void removeItem(HashedItem fpaux, long* chainIndex, long* isLastIndex) {
      int chainStart = getChainStart(fpaux, chainIndex, isLastIndex);
      // avoid an if command: either update chainIndex to the new state or keep it the way it is.
      chainIndex[fpaux.set] = (isLastIndex[fpaux.set] & (1L << chainStart)) == 0L
                              ? chainIndex[fpaux.set]
                              : chainIndex[fpaux.set] & ~(1L << fpaux.chainId.to_ulong());
      // update isLastIndex.
      isLastIndex[fpaux.set] = shrinkOffset(isLastIndex[fpaux.set], chainStart);
    }

    TinySetIndexing()= default;
};


#endif //WTINYLFU_TINYSETINDEXING_H
