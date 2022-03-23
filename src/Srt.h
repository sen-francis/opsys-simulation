#ifndef SRT_H
#define SRT_H

#include "Algorithm.h"


class Srt: public TauAlgorithm {
public:
    Srt() : TauAlgorithm("SRT") {}
    void run(
        const EventQ &arrivals,
        const std::vector<Bursts> &all_bursts,
        const std::vector<int> &tau_init,
        int half_tcs,
        float alpha);
protected:
    void p_will_preempt(char elect, char occupant, int time) const;
    bool should_preempt(const Ready candidate, char occupant, bool switching) const;
    int total_sofar(char id, bool switching) const;
    // bursts_sofar stores how long each process has run on its current burst
    // (which may have been split up by preemption)
    std::vector<int> bursts_sofar;
    // cpu_start stores the last time each process started using the CPU
    // (which may not be the same as the start of the current burst due to
    // preemption)
    std::vector<int> cpu_start;
};


#endif // SRT_H
