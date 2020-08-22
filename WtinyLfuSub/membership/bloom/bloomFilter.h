//
// Created by mahmo on 8/6/2020.
//

#ifndef WTINYLFU_BLOOMFILTER_H
#define WTINYLFU_BLOOMFILTER_H

#include <iostream>
#include <cmath>
#include "../../BasicSettings.h"

class bloomFilter {
private:
    long long SEED[4] ={ // A mixture of seeds from FNV-1a, CityHash, and Murmur3
            static_cast<long long>((0xc3a5c85c97cb3127L)),
            static_cast<long long>((0xb492b66fbe98f273L)),
            static_cast<long long>((0x9ae16a3b2f90404fL)),
            static_cast<long long>((0xcbf29ce484222325L))};
    int BITS_PER_LONG_SHIFT = 6; // 64-bits
    int BITS_PER_LONG_MASK = 64 - 1;

    int tableShift;
    long* table;
    int bloomSize;
public:

     int seeded(int item, int i) {
       long long hash = (item + SEED[i]) * SEED[i];
       long long temp = (hash) >> 32;
      hash +=  unsigned (temp);
      return (int) hash;
    }

    bool setAt(long long item, int seedIndex) {
      int hash = seeded(item, seedIndex);
      int temp = unsigned (hash) >> tableShift;
      int index = temp;
      long previous = table[index];
      table[index] |= bitmask(hash);
      return (table[index] != previous);
    }

    static long long spread(int x) {
      x = ((unsigned (x) >> 16) ^ x) * 0x45d9f3b;
      x = ((unsigned (x) >> 16) ^ x) * 0x45d9f3b;
      return (unsigned (x) >> 16) ^ x;
    }


    long bitmask(int hash) {
      return 1L << (hash & BITS_PER_LONG_MASK);
    }

    bloomFilter(BasicSettings* basicSettings) {
       table =nullptr;
         ensureCapacity(basicSettings->memberShipSet().expectedInsertions(),
                        basicSettings->memberShipSet().getfpp());
    }

    bloomFilter() = default;

    bloomFilter(int expectef, double fpp){
      ensureCapacity(expectef, fpp);
    }

    ~bloomFilter(){
      delete [] table;
    }

    static int numberOfLeadingZeros(int i) {
      // HD, Figure 5-6
      if (i == 0)
        return 32;
      int n = 1;
      if (unsigned (i) >> 16 == 0) { n += 16; i <<= 16; }
      if (unsigned (i) >> 24 == 0) { n +=  8; i <<=  8; }
      if (unsigned (i) >> 28 == 0) { n +=  4; i <<=  4; }
      if (unsigned (i) >> 30 == 0) { n +=  2; i <<=  2; }
      n -= unsigned (i) >> 31;
      return n;
    }

    void ensureCapacity(long expectedInsertions, double fpp) {
      if(expectedInsertions < 0 || fpp<0){
        bloomSize=0;
         throw std::invalid_argument(" fpp and expectedInsertions should be >0");
      }
      if(!(fpp > 0 && fpp < 1)){
        bloomSize=0;
         throw std::invalid_argument(" fpp and fpp should be between 0-1");
      }
      double optimalBitsFactor = -std::log(fpp) / (log(2) * log(2));
      int optimalNumberOfBits = (int) (expectedInsertions * optimalBitsFactor);
      int optimalSize = unsigned (optimalNumberOfBits) >> unsigned (BITS_PER_LONG_SHIFT);
      if ((table != nullptr)) {
        if(bloomSize >= optimalSize)
            return;
      } else if (optimalSize == 0) {
        tableShift = 32 - 1;
        bloomSize=1;
        table = new long[1];
      } else {
        int powerOfTwoShift = 32 - numberOfLeadingZeros(optimalSize - 1);
        tableShift = 32 - powerOfTwoShift;
        table = new long[1 << powerOfTwoShift];
        bloomSize=( 1 << powerOfTwoShift);
        clear();
      }
    }


    static long long hashCode(unsigned long value) {
      return ((long long)value ^ ((long long) (value) >> 32));
    }

    virtual bool mightContain(long e)  {
      int item = spread(hashCode(e));
      for (int i = 0; i < 4; i++) {
        int hash = seeded(item, i);
        int index = unsigned (hash) >>  (tableShift);
        if ((table[index] & bitmask(hash)) == 0L) {
          return false;
        }
      }
      return true;
    }

    void fill(long* a, long val) {
      int i = 0;
      for(int len = bloomSize; i < len; ++i) {
        a[i] = val;
      }
    }

    virtual void clear()  {
      fill(table, 0L);
    }

    virtual bool put(long e)  {
      int item = spread(hashCode(e));
      return setAt(item, 0) |setAt(item, 1) | setAt(item, 2) |setAt(item,3);
    }

};

#endif //WTINYLFU_BLOOMFILTER_H
