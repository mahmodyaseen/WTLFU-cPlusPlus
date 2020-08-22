//
// Created by mahmo on 8/9/2020.
//

#ifndef WTINYLFU_ACCESSEVENT_H
#define WTINYLFU_ACCESSEVENT_H



class AccessEvent {
private:
    long Key;

    class PenaltiesAccessEvent {
    private:
        double missPenalty;
        double hitPenalty;
    public:

        PenaltiesAccessEvent(long key, double hitPenalty, double missPenalty) {
    //      Key=key;
          if(hitPenalty<0)
            throw ("hitpenlty should be greater than 0");
          if ( missPenalty < hitPenalty)
            throw ("hitpenlty should be greater than missPenlty (acessEvent)");
          this->hitPenalty = hitPenalty;
          this->missPenalty = missPenalty;
        }

        double getMissPenalty() {
          return missPenalty;
        }

        double getHitPenalty() {
          return hitPenalty;
        }
    };

public:
     void setKey(long keyy){
      Key=keyy;
    }

    AccessEvent(long key) {
      setKey(key);
    }

    long getKey() {
      return AccessEvent::Key;
    }

private:
    class WeightedAccessEvent {
        int weight;
    public:
        WeightedAccessEvent(long key, int weight) {
          long hashcode=cantorHashCode(key, weight);
     //     Key=hashcode;
          if(weight>=0)
            this->weight = weight;
          else
            throw ("Weighted access error wight limit");
        }

        int getWeight() {
          return weight;
        }
    };

public:

    /** Returns the weight of the entry. */
    int getWeight() {
      return 1;
    }

    /** Returns the hit penalty of the entry */
    double hitPenalty() {
      return 0;
    }

    double missPenalty() {
      return 0;
    }

//    bool equals(Object o) {
//            if (o == this) {
//                return true;
//            } else if (!(o instanceof AccessEvent)) {
//                return false;
//            }
//            AccessEvent event = (AccessEvent) o;
//            return Objects.equals(key(), event.key())
//                   && Objects.equals(weight(), event.weight())
//                   && Objects.equals(hitPenalty(), event.hitPenalty())
//                   && Objects.equals(missPenalty(), event.missPenalty());
//        }
//
//    T& operator=(const T& other) // copy assignment
//    {
//        if (this != &other) { // self-assignment check expected
//            if (other.size != size) {         // storage cannot be reused
//                delete[] mArray;              // destroy storage in this
//                size = 0;
//                mArray = nullptr;             // preserve invariants in case next line throws
//                mArray = new int[other.size]; // create storage in this
//                size = other.size;
//            }
//            std::copy(other.mArray, other.mArray + other.size, mArray);
//        }
//        return *this;
//    }
//
//     int hashCode() {
//            return Objects.hash(key(), weight(), missPenalty(), hitPenalty());
//        }
//
//     String toString() {
//            return MoreObjects.toStringHelper(this)
//                    .add("key", key())
//                    .add("weight", weight())
//                    .add("hit penalty", hitPenalty())
//                    .add("miss penalty", missPenalty())
//                    .toString();
//        }

    /** Returns an event for the given key. */
    static AccessEvent forKey(long key) {
      return AccessEvent(key);
    }

    /** Returns an event for the given key and weight. */
    static WeightedAccessEvent forKeyAndWeight(long key, int weight) {
      return WeightedAccessEvent(key, weight);
    }

    /** Returns an event for the given key and penalties. */
    static PenaltiesAccessEvent forKeyAndPenalties(long key, double hitPenalty, double missPenalty) {
      return PenaltiesAccessEvent(key, hitPenalty, missPenalty);
    }

    /** Cantor pairing function. */
    static long cantorHashCode(long key, int weight) {
      return (key + weight) * (key + weight + 1) / 2 + weight;
    }
};


#endif //WTINYLFU_ACCESSEVENT_H
