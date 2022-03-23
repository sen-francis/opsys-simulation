#include <ios>
#include <iomanip>

#include "Algorithm.h"


bool operator<(const TauAlgorithm::Ready a, const TauAlgorithm::Ready b)
{
    return a.len >  b.len
       || (a.len == b.len && a.id > b.id);
}

void Algorithm::p_stats(std::ostream &ostr, int t_half_cs)
{
    const int sum_wait = sum_turnaround - sum_bursts - num_cs * t_half_cs * 2;
    ostr << "Algorithm "<< name << "\n" << std::setprecision(3) << std::fixed
         << "-- average CPU burst time: " << (double)sum_bursts / num_bursts << " ms\n"
         << "-- average wait time: " << (double)sum_wait / num_bursts << " ms\n"
         << "-- average turnaround time: " << (double)sum_turnaround / num_bursts << " ms\n"
         << "-- total number of context switches: " << num_cs << '\n'
         << "-- total number of preemptions: " << num_preepmt << '\n'
         << "-- CPU utilization: " << (double)sum_bursts / t * 100 << "%\n";
}
