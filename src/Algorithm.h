#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <queue>
#include <vector>
#include <functional>
#include <iostream>
#include <string>

#include "Event.h"
#include "Bursts.h"


using std::cout;

// A very long type name
typedef std::priority_queue<Event, std::vector<Event>, std::greater<Event>> EventQ;

// Abstract base class for all CPU scheduling algorithms. Mainly defines output
// printing methods, many of which are common to all algorithms.
class Algorithm {
public:
    void p_stats(std::ostream &ostr, int t_half_cs);
protected:
    Algorithm(const std::string &name) : name(name) {}
    void pt() const { cout << "time " << t << "ms: "; } // Print time label (line prefix)
    virtual void pq() const = 0;                        // Print ready queue (line suffix)
    virtual void pp(char id) const = 0;                 // Print process name and maybe tau
    void ptp(char id) const { pt(); pp(id); cout << ' '; } // A very common output prefix
    void p_start() const { 
        if(t < 1000){
            pt(); cout << "Simulator started for " << name; pq(); 
        }
    }
    void p_end  () const { pt(); cout << "Simulator ended for "   << name; pq(); }
    void p_arrive(char id, bool is_new) const
    { 
        if(t < 1000){
            ptp(id); cout << (is_new ? "arrived" : "completed I/O") << "; added to ready queue"; pq(); 
        }
    }
    void p_arrive(char id, bool is_new, char preempt) const
    { 
        if(t < 1000){
            ptp(id); cout << (is_new ? "arrived" : "completed I/O") << "; preempting " << preempt; pq(); 
        }
    }
    void p_cpu_start(char id, int burst) const
    { 
        if(t < 1000){
            ptp(id); cout << "started using the CPU for " << burst << "ms burst"; pq(); 
        }
    }
    void p_cpu_start(char id, int burst, int remain) const
    { 
        if(t < 1000){
            ptp(id); cout << "started using the CPU for remaining "
                          << remain << "ms of " << burst << "ms burst"; pq(); 
        }
    }
    void p_term(char id) const { pt(); cout << "Process " << id << " terminated"; pq(); }
    int sum_turnaround=0;
    int sum_bursts = 0;
    int num_cs=0;
    //int t_half_cs=atoi(argv[5])/2;
    int num_bursts=0;
    int num_preepmt=0;
    const std::string name; // FCFS, SJF, etc.
    EventQ event_q;
    int t = 0;
};

// Algorithms whose ready queue is FIFO, like FCFS and RR
class FifoAlgorithm: public Algorithm {
protected:
    FifoAlgorithm(const std::string &name) : Algorithm(name) {}
    void pq() const
    {
        std::queue<char> q = ready_q;
        cout << " [Q ";
        if (q.empty()) cout << "empty";
        else while (!q.empty()) { cout << q.front(); q.pop(); }
        cout << "]\n";
    }
    void pp(char id) const { cout << "Process " << id; }
    void p_cpu_end(char id, int remain, int io_end) const
    {
        if(t < 1000){
            ptp(id); cout << "completed a CPU burst; " << remain
                        << (remain == 1 ? " burst" : " bursts") << " to go"; pq();
            ptp(id); cout << "switching out of CPU; will block on I/O until time "
                        << io_end << "ms"; pq();
        }
    }
    void p_expire_pre(char id, int time_left){
        if(t < 1000){
            pt(); cout << "Time slice expired; process "<< id
                    << " preempted with " << time_left
                    <<"ms to go"; pq();
        }
    }
    void p_expire_no_pre(){
        if(t < 1000){
            pt(); cout << "Time slice expired; no preemption because ready queue is empty"; pq();
        }
    }

    void p_expire_finish(char id, int remaining, int total){
        if(t < 1000){
            ptp(id); cout << "started using the CPU for remaining " << remaining
                        <<"ms of " << total
                        <<"ms burst"; pq();
        }
    }
    void p_RR_start(int time_slice) const { 
        if(t < 1000){
            pt(); cout << "Simulator started for " << name << " with time slice " << time_slice<<"ms"; pq(); 
        }
    }

    std::queue<char> ready_q;
};

// Algorithms whose ready queue is a priority queue based on tau or estimated
// remaining portion of tau, like SJF and SRT. Unlike the FifoAlgorithms, these
// algorithms print tau on several kinds of output lines.
class TauAlgorithm: public Algorithm {
public:
    // Represents a process in the ready queue for SJF or SRT with an ID and an
    // estimated burst length or estimated remaining burst length
    struct Ready { char id; int len;
                   Ready(char id, int len) : id(id), len(len) {} };
protected:
    TauAlgorithm(const std::string &name) : Algorithm(name) {}
    void pq() const
    {
        std::priority_queue<Ready> q = ready_q;
        cout << " [Q ";
        if (q.empty()) cout << "empty";
        else while (!q.empty()) { cout << q.top().id; q.pop(); }
        cout << "]\n";
    }
    void pp(char id) const
    { cout << "Process " << id << " (tau " << tau[id - 'A'] << "ms)"; }
    void p_cpu_end(char id, int remain, int old_tau, int io_end) const
    {
        if(t < 1000){
            pt(); cout << "Process " << id << " (tau " << old_tau << "ms) completed a CPU burst; "
                    << remain << (remain == 1 ? " burst" : " bursts") << " to go"; pq();
            pt(); cout << "Recalculated tau from " << old_tau << "ms to "
                    << tau[id - 'A'] << "ms for process " << id; pq();
            pt(); cout << "Process " << id << " switching out of CPU; will block on I/O until time "
                    << io_end << "ms"; pq();
        }
    }

    std::priority_queue<Ready> ready_q;
    std::vector<int> tau;
};

// Whether a should come AFTER b in an SJF or SRT ready queue
bool operator<(const TauAlgorithm::Ready a, const TauAlgorithm::Ready b);


#endif // ALGORITHM_H
