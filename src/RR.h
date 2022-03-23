#ifndef RR_H
#define RR_H

#include "Algorithm.h"


class RR: public FifoAlgorithm {
public:
    RR() : FifoAlgorithm("RR") {}
    void run(const EventQ &arrivals, const std::vector<Bursts> &bursts, int half_tcs, int time_slice);
};


#endif // RR_H