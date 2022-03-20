#include <iostream>
#include <string>

#include "algorithms.h"


void print_time(int time);
void print_q(std::queue<char> q);
void print_process(char id);
void print_start(const std::string &algorithm);
void print_end(int time, const std::string &algorithm);
void print_terminated(int time, char process_id, const std::queue<char> &ready_q);
void print_cpu_start(int time, char process_id, int burst, const std::queue<char> &ready_q);
void print_cpu_end(int time, char process_id, int remaining_cpu, int io_end,
                   const std::queue<char> &ready_q);
void print_arrival(const Event &e, const std::queue<char> &ready_q);

void fcfs(
    const std::priority_queue<Event, std::vector<Event>, std::greater<Event>> &arrivals,
    const std::vector<Process> &bursts,
    int half_tcs)
{
    std::priority_queue<Event, std::vector<Event>, std::greater<Event>> event_q = arrivals;
    std::vector<Process> processes = bursts;
    std::queue<char> ready_q;
    // The process that is running on or switching into the CPU or '\0' if none
    char occupant = '\0';

    print_start("FCFS");
    int t = 0;
    while (!event_q.empty()) {
        const Event e = event_q.top();
        event_q.pop();

        t = e.get_time();
        const char id = e.get_process_id();
        Process &p = processes[id - 'A'];
        switch (e.get_type()) {
        case Event::Type::switch_in:
            print_cpu_start(t, id, p.cpu_bursts.top(), ready_q);
            event_q.emplace(t + p.cpu_bursts.top(), Event::Type::cpu_burst_end, id);
            p.cpu_bursts.pop();
            break;
        case Event::Type::cpu_burst_end:
            event_q.emplace(t + half_tcs, Event::Type::switch_out, id);
            if (p.io_bursts.empty())
                print_terminated(t, id, ready_q);
            else {
                const int io_end = t + half_tcs + p.io_bursts.top();
                p.io_bursts.pop();
                print_cpu_end(t, id, p.cpu_bursts.size(), io_end, ready_q);
                event_q.emplace(io_end, Event::Type::io_burst_end, id);
            }
            occupant = '\0';
            break;
        case Event::Type::io_burst_end:
        case Event::Type::new_arrival:
            if (p.cpu_bursts.empty())
                break;
            ready_q.push(id);
            print_arrival(e, ready_q);
            if (occupant == '\0' && ready_q.size() == 1) {
                event_q.emplace(t + half_tcs, Event::Type::switch_in, id);
                ready_q.pop();
                occupant = id;
            }
            break;
        case Event::Type::switch_out:
            if (ready_q.empty())
                break;
            event_q.emplace(t + half_tcs, Event::Type::switch_in, ready_q.front());
            occupant = ready_q.front();
            ready_q.pop();
        }
    }
    print_end(t, "FCFS");
}

void print_time(int time)
{
    std::cout << "time " << time << "ms: ";
}

void print_q(std::queue<char> q)
{
    std::cout << " [Q ";
    if (q.empty())
        std::cout << "empty";
    else while (!q.empty()) {
        std::cout << q.front();
        q.pop();
    }
    std::cout << "]\n";
}

void print_process(char id)
{
    std::cout << "Process " << id;
}

void print_start(const std::string &algorithm)
{
    print_time(0); std::cout << "Simulator started for " << algorithm; print_q({});
}

void print_end(int time, const std::string &algorithm)
{
    print_time(time); std::cout << "Simulator ended for " << algorithm; print_q({});
}

void print_terminated(int time, char process_id, const std::queue<char> &ready_q)
{
    print_time(time); print_process(process_id); std::cout << " terminated"; print_q(ready_q);
}

void print_cpu_start(int time, char process_id, int burst, const std::queue<char> &ready_q)
{
    print_time(time); print_process(process_id);
    std::cout << " started using the CPU for " << burst << "ms burst";
    print_q(ready_q);
}

void print_cpu_end(int time, char process_id, int remaining_cpu, int io_end,
                   const std::queue<char> &ready_q)
{
    print_time(time); print_process(process_id);
    std::cout << " completed a CPU burst; " << remaining_cpu
              << (remaining_cpu == 1 ? " burst" : " bursts") << " to go";
    print_q(ready_q);
    print_time(time); print_process(process_id);
    std::cout << " switching out of CPU; will block on I/O until time " << io_end << "ms";
    print_q(ready_q);
}

void print_arrival(const Event &e, const std::queue<char> &ready_q)
{
    print_time(e.get_time()); print_process(e.get_process_id());
    switch (e.get_type()) {
    case Event::Type::io_burst_end:
        std::cout << " completed I/O";
        break;
    case Event::Type::new_arrival:
        std::cout << " arrived";
    default:;
    }
    std::cout << "; added to ready queue";
    print_q(ready_q);
}
