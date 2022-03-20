#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <queue>
#include <vector>
#include <functional>

#include "Event.h"
#include "Process.h"


typedef std::priority_queue<Event, std::vector<Event>, std::greater<Event>> EventQ;

void fcfs(
    const EventQ &arrivals,
    const std::vector<Process> &bursts,
    int half_tcs);


#endif // ALGORITHMS_H
