#include <iostream>
#include <stdio.h>
#include <fstream>
#include <set>
#include <stack>
#include <deque>
#include <cstdlib>
#include <cmath>

#include "Algorithm.h"
#include "Fcfs.h"
#include "Sjf.h"
#include "Srt.h"
#include "RR.h"


using namespace std;

double next_exp(double lambda, double bound){
    double r = -std::log(drand48()) / lambda;
    while (r>bound){
        r = -std::log(drand48()) / lambda;
    }
    return r;
}

void generate_arrivals_and_bursts(int n, double lambda, double bound, EventQ &arrivals, std::vector<Bursts> &bursts, bool print){
    char process_id = 'A';
    for(int i = 0; i < n; i++){
        int arrival_time = int(floor(next_exp(lambda, bound)));
        Event e(arrival_time, Event::Type::new_arrival, process_id);
        arrivals.push(e);
        int num_bursts = int(ceil(drand48()*100));
        if(print){
            cout << "Process " << process_id << " (arrival time " << arrival_time << " ms) " << num_bursts << " CPU bursts (tau " << 1/lambda <<"ms)\n";
        }
        Bursts b;
        std::deque<int> cpu_bursts, io_bursts;
        for(int j = 0; j < num_bursts; j++){
            int cpu_burst = int(ceil(next_exp(lambda, bound)));
            //need to check bound for ceil
            while (cpu_burst > bound){
               cpu_burst = int(ceil(next_exp(lambda, bound)));
            }
            cpu_bursts.push_front(cpu_burst);
            if(j!=num_bursts-1){
                int io_burst = int(ceil(next_exp(lambda, bound)));
                //need to check bound for ceil
                while(io_burst > bound){
                   io_burst = int(ceil(next_exp(lambda, bound)));
                }
                io_burst*=10;
                io_bursts.push_front(io_burst);
                if(print){
                    cout << "--> CPU burst " << cpu_burst << " ms --> I/O burst " << io_burst << " ms\n";
                }
            }
            else{
                if(print){
                    cout << "--> CPU burst " << cpu_burst << " ms\n";
                }
            }
        }
        bursts[i].cpu_bursts = std::stack<int>(cpu_bursts);
        bursts[i].io_bursts  = std::stack<int>(io_bursts );
        process_id++;
    }
    cout << "\n";
}

int main(int argc, char **argv)
{
    if (argc != 8 or atoi(argv[1])>26) {
        std::cerr <<
            "Usage: " << argv[0] << " <n processes> <seed> <lambda> <upper bound> <context switch time> <alpha> <slice time>\n"
            "<n processes> is the number of processes to simulate\n"
            "<seed> is passed to srand48 to seed the random number generator\n"
            "<lambda> is the rate parameter in the exponential distribution function\n"
            "<upper bound> is the upper bound for valid pseudo-random numbers\n"
            "<context switch time> is the time in milliseconds to perform a context switch\n"
            "<alpha> \n"
            "<slice time> is the time slice value for the RR algorithm\n";
        return EXIT_FAILURE;
    }

    int n = atoi(argv[1]);
    int seed = atoi(argv[2]);
    double lambda = atof(argv[3]);
    double bound = atof(argv[4]);
    double context_switch_time = atof(argv[5]);
    double alpha = atof(argv[6]);
    double time_slice = atof(argv[7]);
    ofstream simout("simout.txt");

    //seed + generate times for algo
    srand48(seed);
    EventQ arrivals;
    std::vector<Bursts> bursts(n);
    generate_arrivals_and_bursts(n, lambda, bound, arrivals, bursts, true);
    Fcfs fcfs;
    fcfs.run(arrivals, bursts, context_switch_time/2);
    fcfs.p_stats(simout, int(context_switch_time/2));

    srand48(seed);
    arrivals = EventQ();
    bursts = std::vector<Bursts>(n);
    generate_arrivals_and_bursts(n, lambda, bound, arrivals, bursts, false);
    std::vector<int> tau = {n, int(1/lambda)};
    Sjf sjf;
    sjf.run(arrivals, bursts, tau, context_switch_time/2, alpha);
    sjf.p_stats(simout, int(context_switch_time/2));

    srand48(seed);
    arrivals = EventQ();
    bursts = std::vector<Bursts>(n);
    generate_arrivals_and_bursts(n, lambda, bound, arrivals, bursts, false);
    Srt srt;
    srt.run(arrivals, bursts, tau, context_switch_time/2, alpha);
    srt.p_stats(simout, int(context_switch_time/2));

    srand48(seed);
    arrivals = EventQ();
    bursts = std::vector<Bursts>(n);
    generate_arrivals_and_bursts(n, lambda, bound, arrivals, bursts, false);
    RR rr;
    rr.run(arrivals, bursts, context_switch_time/2, time_slice);
    rr.p_stats(simout, int(context_switch_time/2));
    return EXIT_SUCCESS;
}
