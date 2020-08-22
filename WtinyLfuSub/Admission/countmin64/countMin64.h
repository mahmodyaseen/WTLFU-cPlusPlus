//
// Created by mahmo on 8/17/2020.
//

#ifndef WTINYLFU_COUNTMIN64_H
#define WTINYLFU_COUNTMIN64_H

#define MIN(a,b)    (a > b ? b : a)
#define MAX(a ,b)   (a > b ? a : b)
#define LONG_PRIME 4294967311l

# include <iostream>
# include <cmath>
# include <cstdlib>
# include <ctime>
# include <limits>

using namespace std;

class countMin64 {
    float eps, confidence;
    unsigned int total;
    int **hashArr; //not sure if to make it liner

    void hashFun(int **hashes, int i){
      hashes[i][0] = int(float(rand())*float(LONG_PRIME)/float(RAND_MAX) + 1);
      hashes[i][1] = int(float(rand())*float(LONG_PRIME)/float(RAND_MAX) + 1);
    }

    void add(int item, int count) {
      unsigned int hashval = 0;
      for (unsigned int i = 0; i < depth; i++) {
        hashval = ((long)hashArr[i][0]*item + hashArr[i][1]) % width;
        table[i][hashval] = table[i][hashval] + count;
      }
    }

    void consAdd(int item, int count){
      unsigned int hashval = 0;
      int* temp=new int [depth];
      for (unsigned int i = 0; i < depth; i++) {
        temp[i] = ((long)hashArr[i][0]*item + hashArr[i][1]) % width;
      }
      int min=table[0][temp[0]];
      for( int i = 1; i<depth ; i++){
        min=MIN(min, table[i][temp[i]]);
      }
      for( int i = 0; i < depth ; ++i){
        int maxVal = MAX( (min+count), table[i][temp[i]]);
        table[i][temp[i]] = maxVal;
      }
    }

public:
    unsigned int width, depth;
    int **table;

    countMin64(double eps, double confidence){
      // 2/w = eps ; w = 2/eps
      // 1/2^depth <= 1-confidence ; .depth >= -log2 (1-confidence)
      if (!(0.00009 <= eps && eps < 1)) {
        std::out_of_range( "eps must be in this range: [0.01, 1)" );
        exit(EXIT_FAILURE);
      } else if (!(0 < confidence && confidence < 1)) {
        std::out_of_range(  "confidence must be in this range: (0,1)" );
        exit(EXIT_FAILURE);
      }
      this->width=ceil(2/eps);
      this->depth = ceil(-log(1 - confidence) / log(2));
      this->table = new int *[this->depth];
      this->total=0;
      unsigned int i,j;
      for( i = 0 ; i < depth ; ++i ){
        table[i] = new int[width];
        for( j = 0 ; j < width ; ++j ){
          table[i][j] = 0 ;
        }
      }
      srand(time(NULL));
      this->hashArr=new int* [this->depth];
      for( i = 0 ; i < depth ; ++i ){
        hashArr[i] = new int[2];
        hashFun(hashArr , i);
      }
    }

    void update(bool conservative, int item, int count){
      if (count < 0) {
        std::invalid_argument("count should be positive" );
      }
      this->total = this->total + count;
      if (conservative) {
        consAdd(item, count);
      } else {
        add(item, count);
      }
    }

    unsigned int estimate(int item){
      int minVal = numeric_limits<int>::max();
      unsigned int hashval = 0;
      for (unsigned int j = 0; j < depth; j++) {
        hashval = ((long)hashArr[j][0]*item + hashArr[j][1]) % width;
        minVal = MIN(minVal, table[j][hashval]);
      }
      return minVal;
    }

    unsigned int totalcount(){
      return this->total;
    }

    ~countMin64(){
      unsigned int i;
      for (i = 0; i < depth; i++) {
        delete[] table[i];
      }
      for (i = 0; i < depth; i++) {
        delete[] hashArr[i];
      }
      delete[] table;
      delete[] hashArr;
    }

};


#endif //WTINYLFU_COUNTMIN64_H
