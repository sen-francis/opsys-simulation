#include <iostream>
#include <set>
#include <stack>
#include <deque>

#include "algorithms.h"


int main(int argc, char **argv)
{
    if (argc != 8) {
        std::cerr <<
            "Usage: " << argv[0] << " <n processes> <seed> ...\n"
            "<n processes> is the number of processes to simulate\n"
            "<seed> is passed to srand48 to seed the random number generator\n"
            "...\n";
        return 1;
    }
    return 0;
}
