#include <iostream>
#include <set>
#include <stack>
#include <deque>
#include <fstream>
#include "Algorithm.h"
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
  std::ofstream simout("simout.txt");
  vector<Process> processes;
  for (int i = 0; i < atoi(argv[1]); ++i)
    processes.push_back(Process('A' + i, atof(argv[6]), atof(argv[3]), atof(argv[4]));

  SRT srt(atoi(argv[5]), processes);
  srt.Algorithm();
  srt.printStats(simout);
  cout << endl;
  RR rr(atoi(argv[5]), atoi(argv[7]), processes);
  rr.Algorithm();
  rr.printStats(simout);

  return 0;
}