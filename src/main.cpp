#include <iostream>
#include <set>
#include <stack>
#include <deque>
#include <stdlib.h>
#include <math.h>
#include "Algorithm.h"
#include "Fcfs.h"

double next_exp(double lambda, double bound){
    double u = drand48() / (bound + 1.0);
    return -std::log(1- u) / lambda;
}

void generate_arrivals_and_bursts(int n, double lambda, double bound, EventQ &arrivals, std::vector<Bursts> &bursts){
    char process_id = 'A';
    for(int i = 0; i < n; i++){
        int arrival_time = int(floor(next_exp(lambda, bound)));
        Event e(arrival_time, Event::Type::switch_in, process_id);
        arrivals.push(e);
        int num_bursts = int(ceil(drand48()*100));
        Bursts b;
        for(int j = 0; j < num_bursts; j++){
            b.cpu_bursts.push(int(ceil(next_exp(lambda, bound))));
            if(j!=num_bursts-1){
                b.io_bursts.push(int(ceil(next_exp(lambda, bound)))*10);
            }
        }
        bursts.push_back(b);
        process_id++;
    }
}

int main(int argc, char **argv)
{
    if (argc != 8) {
        std::cerr <<
            "Usage: " << argv[0] << " <n processes> <seed> <lambda> <upper bound> <context switch time> <alpha> <slice time>\n"
            "<n processes> is the number of processes to simulate\n"
            "<seed> is passed to srand48 to seed the random number generator\n"
            "<lambda> is the rate parameter in the exponential distribution function\n"
            "<upper bound> is the upper bound for valid pseudo-random numbers\n"
            "<context switch time> is the time in milliseconds to perform a context switch\n"
            "<alpha> \n"
            "<slice time> is the time slice value for the RR algorithm\n";
        return 1;
    }
    int n = atoi(argv[1]);
    int seed = atoi(argv[2]);
    double lambda = atof(argv[3]);
    double bound = atof(argv[4]);
    double context_switch_time = atof(argv[5]);
    double alpha = atof(argv[6]);
    double slice_time = atof(argv[7]);
    //seed 
    srand48(seed);
    //generate arrival and burst times
    EventQ arrivals;
    std::vector<Bursts> bursts;
    generate_arrivals_and_bursts(n, lambda, bound, arrivals, bursts);
    //call algorithm function
    Fcfs().run(arrivals, bursts, context_switch_time/2);
    //re-seed before calling each algo
    return 0;
}
