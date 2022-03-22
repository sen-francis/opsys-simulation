#include <vector>
#include <deque>
#include <stack>

#include "../src/Fcfs.h"


int main()
{
    std::vector<Bursts> bursts(2);
    std::deque<int>
        a_cpu_bursts{5, 63, 70, 91, 111, 53, 28, 190, 76, 30},
        a_io_bursts {520, 1080, 1790, 710, 1110, 1100, 40, 320, 2820},
        b_cpu_bursts{3, 299, 12, 292, 197, 109, 25, 167, 137, 228, 36, 41, 107, 156, 14, 6, 58, 34,
                     65, 15, 140, 21, 42, 126, 43, 119, 318, 85, 10, 136, 8, 119, 24, 39, 145, 137,
                     8, 121, 58, 44, 123, 72, 205, 142, 102, 31, 17, 33, 150, 11, 26, 131, 54, 54,
                     19, 274, 239, 190, 182, 20, 71, 55, 65, 84, 244, 57, 451, 324, 95, 338},
        b_io_bursts {1450, 610, 490, 2270, 1090, 1180, 950, 320, 170, 780, 30, 450, 250, 590, 570,
                     290, 2120, 1090, 1290, 2080, 2540, 110, 110, 620, 2370, 1340, 500, 1130, 510,
                     1500, 1990, 790, 840, 3280, 820, 60, 2080, 140, 250, 480, 400, 1490, 730, 10,
                     320, 430, 30, 80, 400, 3840, 1870, 180, 250, 980, 370, 3810, 1450, 2180, 850,
                     960, 510, 490, 170, 1080, 1070, 150, 1110, 2560, 170};
    bursts[0].cpu_bursts = std::stack(a_cpu_bursts);
    bursts[0].io_bursts  = std::stack(a_io_bursts );
    bursts[1].cpu_bursts = std::stack(b_cpu_bursts);
    bursts[1].io_bursts  = std::stack(b_io_bursts );
    EventQ arrivals;
    arrivals.emplace(482, Event::Type::new_arrival, 'A');
    arrivals.emplace(399, Event::Type::new_arrival, 'B');

    Fcfs().run(arrivals, bursts, 2);
    return 0;
}
