#include "Fcfs.h"


void Fcfs::run(const EventQ &arrivals, const std::vector<Bursts> &all_bursts, int half_tcs)
{
    event_q = arrivals;
    std::vector<Bursts> bursts = all_bursts;
    // The process that is running on or switching into or out of the CPU or '\0' if none
    char occupant = '\0';

    p_start();
    t = 0;
    while (!event_q.empty()) {
        const Event e = event_q.top();
        event_q.pop();

        t = e.get_time();
        const char id = e.get_process_id();
        Bursts &b = bursts[id - 'A'];
        switch (e.get_type()) {
        case Event::Type::switch_in:
            p_cpu_start(id, b.cpu_bursts.top());
            event_q.emplace(t + b.cpu_bursts.top(), Event::Type::cpu_burst_end, id);
            b.cpu_bursts.pop();
            break;
        case Event::Type::cpu_burst_end:
            event_q.emplace(t + half_tcs, Event::Type::switch_out, id);
            if (b.io_bursts.empty())
                p_term(id);
            else {
                const int io_end = t + half_tcs + b.io_bursts.top();
                b.io_bursts.pop();
                p_cpu_end(id, b.cpu_bursts.size(), io_end);
                event_q.emplace(io_end, Event::Type::io_burst_end, id);
            }
            break;
        case Event::Type::io_burst_end:
        case Event::Type::new_arrival:
            if (b.cpu_bursts.empty())
                break;
            ready_q.push(id);
            p_arrive(id, e.get_type() == Event::Type::new_arrival);
            if (occupant == '\0') {
                event_q.emplace(t + half_tcs, Event::Type::switch_in, id);
                ready_q.pop();
                occupant = id;
            }
            break;
        case Event::Type::slice_expire:
        case Event::Type::switch_out:
            if (ready_q.empty())
                occupant = '\0';
            else {
                event_q.emplace(t + half_tcs, Event::Type::switch_in, ready_q.front());
                occupant = ready_q.front();
                ready_q.pop();
            }
        }
    }
    p_end();
}
