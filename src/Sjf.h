#ifndef SJF_H
#define SJF_H

#include "Algorithm.h"


class Sjf: public TauAlgorithm {
public:
    Sjf() : TauAlgorithm("SJF") {}
    void run(
        const EventQ &arrivals,
        const std::vector<Bursts> &all_bursts,
        const std::vector<int> &tau_init,
        int half_tcs,
        float alpha);
};


#endif // SJF_H
