#include <vector>
#include <deque>
#include <stack>

#include "../src/Fcfs.h"
#include "../src/Sjf.h"


int main()
{
    std::vector<Bursts> bursts(1);
    std::deque<int> cpu_bursts{5, 63, 70, 91, 111, 53, 28, 190, 76, 30},
                    io_bursts {520, 1080, 1790, 710, 1110, 1100, 40, 320, 2820};
    bursts[0].cpu_bursts = std::stack(cpu_bursts);
    bursts[0].io_bursts  = std::stack(io_bursts );
    EventQ arrivals;
    arrivals.emplace(482, Event::Type::new_arrival, 'A');

    Fcfs().run(arrivals, bursts, 2);
    std::cout << '\n';
    Sjf().run(arrivals, bursts, std::vector<int>(1, 100), 2, 0.5);
    return 0;
}
