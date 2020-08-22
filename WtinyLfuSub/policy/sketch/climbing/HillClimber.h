//
// Created by mahmo on 8/16/2020.
//

#ifndef WTINYLFU_HILLCLIMBER_H
#define WTINYLFU_HILLCLIMBER_H


#include <cmath>
using namespace std;

class HillClimber {
public:
    enum QueueType {
        NONE, WINDOW, PROBATION, PROTECTED
    };

    virtual void onHit(long key, QueueType queue, bool isFull)=0;

    virtual void onMiss(long key, bool isFull) = 0;

    class Adaptation {
    public:
        enum Type {
            HOLD, INCREASE_WINDOW, DECREASE_WINDOW
        }typee;

        Adaptation(double amount, Type type) {
          if(amount<0)
            throw("amount in Adaptation should be positive");
          this->typee = (type);
          this->amount = amount;
        }
        double amount;

        static Adaptation hold() {
          return Adaptation(0, Adaptation::Type::HOLD);
        }

        static Adaptation adaptBy(double amount) {
          if (amount == 0) {
            return hold();
          } else if (amount < 0) {
            return decreaseWindow(abs(amount));
          } else {
            return increaseWindow(amount);
          }
        }

        int roundToInt(double amount) {
          return (amount < 0) ? (int) floor(amount) : (int) ceil(amount);
        }

        static Adaptation increaseWindow(double amount) {
          return Adaptation(amount, Type::INCREASE_WINDOW);
        }
        static Adaptation decreaseWindow(double amount) {
          return Adaptation(amount, Type::DECREASE_WINDOW);
        }

    };

    virtual Adaptation adapt(double windowSize, double probationSize, double protectedSize, bool isFull) = 0;
};


#endif //WTINYLFU_HILLCLIMBER_H
