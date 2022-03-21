#ifndef FCFS_H
#define FCFS_H

#include "Algorithm.h"


class Fcfs: public FifoAlgorithm {
public:
    Fcfs() : FifoAlgorithm("FCFS") {}
    void run(const EventQ &arrivals, const std::vector<Bursts> &bursts, int half_tcs);
};


#endif // FCFS_H
