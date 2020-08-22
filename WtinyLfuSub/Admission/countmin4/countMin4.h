//
// Created by mahmo on 8/6/2020.
//

#ifndef WTINYLFU_COUNTMIN4_H
#define WTINYLFU_COUNTMIN4_H

#include "../Frequency.h"
#include "../../BasicSettings.h"

class countMin4 : public Frequency {
private:

    long long int SEED[4] = {static_cast<long long>((0xc3a5c85c97cb3127L)),
                             static_cast<long long>(0xb492b66fbe98f273L),
                             static_cast<long long>(0x9ae16a3b2f90404fL),
                             static_cast<long long>(0xcbf29ce484222325L)};
protected:
    bool conservative;
     int tableMask;
    long* table;
    long long RESET_MASK = 0x7777777777777777L;
    int step=1;
    int tableSize;

public:
    countMin4(BasicSettings *settings);

    ~countMin4(){
      delete[](table);
    }

    virtual void ensureCapacity(long maximumSize);

    static long long hashCode(unsigned long value) {
      return ((long long)value ^ ((long long) (value) >> 32));
    }

    static long long spread(int x) {
      x = ((unsigned (x) >> 16) ^ x) * 0x45d9f3b;
      x = ((unsigned (x) >> 16) ^ x) * 0x45d9f3b;
      return (unsigned (x) >> 16) ^ x;
    }


    int indexOf(int item, int i) {
      long long hash = (item + SEED[i]) * SEED[i];
      long long temp = (hash) >> 32;
      hash +=  unsigned (temp);
      return ((int) hash) & tableMask;
    }

    void fill(long* a, long val) {
      int i = 0;
      for(int len = tableSize; i < len; ++i) {
        a[i] = val;
      }
    }

    void clear()  {
      fill(table, 0L);
    }

    int frequency(long e) override ;

    void increment(long e) override ;

    virtual void tryReset(bool added) {};

    void regularIncrement(long e);

    bool incrementAt(int i, int j, long step) {
      int offset = j << 2;
      long mask = (0xfL << offset);
      if ((table[i] & mask) != mask) {
        long current = unsigned((table[i] & mask)) >> offset; ///might be a bug here mahmod
        long update = std::min(int(current + step), 15);
        table[i] = (table[i] & ~mask) | (update << offset);
        return true;
      }
      return false;
    }

    void conservativeIncrement(long e);

};


#endif //WTINYLFU_COUNTMIN4_H
