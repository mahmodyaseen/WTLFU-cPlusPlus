//
// Created by mahmo on 8/6/2020.
//

#ifndef WTINYLFU_ADMITTOR_H
#define WTINYLFU_ADMITTOR_H


class Admittor {
    friend class AlwaysAdmit;
public:
    virtual void record(long key) =0;

    virtual bool admit(long candidateKey, long victimKey)=0;


/** Returns an admittor that admits every candidate. */
//        static Admittor always();
};

class AlwaysAdmit: public Admittor {
public:

    AlwaysAdmit()= default;

    void record(long key) override {}


    bool admit(long candidateKey, long victimKey) override {
      return true;
    }
};

//static Admittor always() {
//    return AlwaysAdmit();
//}
#endif //WTINYLFU_ADMITTOR_H
