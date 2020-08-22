//
// Created by mahmo on 8/6/2020.
//

#include <stdexcept>
#include <cmath>
#include "countMin4.h"


countMin4::countMin4(BasicSettings* settings) {
  table= nullptr;
  tableSize=0;
  conservative = settings->tinyLfu().conservative();
  double countersMultiplier = settings->tinyLfu().countMin4().getCountersMultiplier();
  long counters = (long) (countersMultiplier * settings->getMaximumSize());
  ensureCapacity(counters);
}

int countMin4::frequency(long e)  {
    int hash = spread(hashCode(e));
    int start = (hash & 3) << 2;
    int frequency = 2147483647;
    for (int i = 0; i < 4; i++) {
      int index = indexOf(hash, i);
      int count = (int) ((unsigned (table[index]) >> unsigned (((start + i) << 2))) & 0xfL);
      frequency = std::min(frequency, count);
    }
    return frequency;
}

void countMin4::increment(long e) {
  if (conservative) {
    conservativeIncrement(e);
  } else {
    regularIncrement(e);
  }
}

void countMin4::regularIncrement(long e) {
  int hash = spread(hashCode(e));
  int start = (hash & 3) << 2;

  // Loop unrolling improves throughput by 5m ops/s
  int index0 = indexOf(hash, 0);
  int index1 = indexOf(hash, 1);
  int index2 = indexOf(hash, 2);
  int index3 = indexOf(hash, 3);

  bool added = incrementAt(index0, start, step);
  added |= incrementAt(index1, start + 1, step);
  added |= incrementAt(index2, start + 2, step);
  added |= incrementAt(index3, start + 3, step);

  tryReset(added);
}

void countMin4::conservativeIncrement(long e) {
  int hash = spread(hashCode(e));
  int start = (unsigned(hash) & 3) << 2;
  int* index = new int[4];
  int* count = new int[4];
  int min = 2147483647;
  for (int i = 0; i < 4; i++) {
    index[i] = indexOf(hash, i);
    count[i] = (int) (unsigned ((table[index[i]]) >> ((start + i) << 2)) & 0xfL); ///might be a bug here mahmod
    min = std::min(min, count[i]);
  }
  if (min == 15) {
    tryReset(false);
    return;
  }
  for (int i = 0; i < 4; i++) {
    if (count[i] == min) {
      incrementAt(index[i], start + i, step);
    }
  }
  tryReset(true);
}

void countMin4::ensureCapacity(long maximumSize) {
  if(maximumSize<0) {
    tableSize =0;
    std::invalid_argument("maximumSize should be bigger or equal 0");
    return;
  }
  int maximum =  std::min(int(maximumSize), 2147483647 >> 1);

  if ((table != nullptr) && (tableSize >= maximum)) {
    return;
  }
  int tmp = (int)std::ceil(maximum);
  table = new long[(maximum == 0) ? 1 : tmp ];
  tableSize = (maximum == 0) ? 1 : tmp;
  tableMask = std::max(0, tableSize- 1);
//  clear();
}


