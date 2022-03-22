#include <iostream>
#include <fstream>
#include <set>
#include <stack>
#include <deque>

#include "Fcfs.h"
#include "Sjf.h"


using namespace std;

int main(int argc, char **argv)
{
    if (argc != 8 or atoi(argv[1])>26) {
        std::cerr <<
            "Usage: " << argv[0] << " <n processes> <seed> ...\n"
            "<n processes> is the number of processes to simulate\n"
            "<seed> is passed to srand48 to seed the random number generator\n"
            "...\n";
        return 1;
    }

    srand48(atol(argv[2]));
    ofstream simout("simout.txt");
    vector<Bursts> processes;
    for (int i = 0; i < atoi(argv[1]); ++i)
        processes.push_back(Bursts('A' + i, atof(argv[6]), atof(argv[3]), atof(argv[4])));

    Fcfs fcfs;
    fcfs.run();
    fcfs.p_stats(simout);
    cout << endl;
    // Rr rr(atoi(argv[5]), atoi(argv[7]), processes);
    // rr.run();
    // rr.p_stats(simout);

    return 0;
}
