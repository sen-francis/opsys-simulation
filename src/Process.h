#ifndef PROCESS_H
#define PROCESS_H

#include <stack>


class Process {
public:
    std::stack<int> cpu_bursts, io_bursts;
};


#endif // PROCESS_H
