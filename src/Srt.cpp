#include <cmath>

#include "Srt.h"


void Srt::run(
    const EventQ &arrivals,
    const std::vector<Bursts> &all_bursts,
    const std::vector<int> &tau_init,
    int half_tcs,
    float alpha)
{
    event_q = arrivals;
    std::vector<Bursts> bursts = all_bursts;
    cpu_start = bursts_sofar = std::vector<int>(arrivals.size(), 0);
    tau = tau_init;
    // The process that is running on or switching into or out of the CPU or '\0' if none
    char occupant = '\0';
    int switch_status = 0;

    p_start();
    while (!event_q.empty()) {
        const Event e = event_q.top();
        event_q.pop();

        t = e.get_time();
        const char id = e.get_process_id();
        Bursts &b = bursts[id - 'A'];
        switch (e.get_type()) {
        case Event::Type::switch_in:
            if (id != occupant) // This switch_in was preempted
                break;
            if (bursts_sofar[id - 'A'] == 0)
                p_cpu_start(id, b.cpu_bursts.top());
            else
                p_cpu_start(id, b.cpu_bursts.top(), b.cpu_bursts.top() - bursts_sofar[id - 'A']);
            cpu_start[id - 'A'] = t;
            if (should_preempt(ready_q.top(), occupant, false)) {
                p_will_preempt(ready_q.top().id, occupant);
                switch_status = -1;
                event_q.emplace(t + half_tcs, Event::Type::switch_out, occupant);
            } else {
                switch_status = 0;
                event_q.emplace(t + b.cpu_bursts.top() - bursts_sofar[id - 'A'],
                                Event::Type::cpu_burst_end, id);
            }
            break;
        case Event::Type::cpu_burst_end:
            if (total_sofar(id, false) != b.cpu_bursts.top()) // This CPU burst was preempted, ignore
                break;
            switch_status = -1;
            event_q.emplace(t + half_tcs, Event::Type::switch_out, id);
            bursts_sofar[id - 'A'] = 0;
            if (b.io_bursts.empty())
                p_term(id);
            else {
                const int io_end = t + half_tcs + b.io_bursts.top();
                b.io_bursts.pop();
                const int old_tau = tau[id - 'A'];
                tau[id - 'A'] = std::ceil(alpha * b.cpu_bursts.top() + (1 - alpha) * old_tau);
                b.cpu_bursts.pop();
                p_cpu_end(id, b.cpu_bursts.size(), old_tau, io_end);
                event_q.emplace(io_end, Event::Type::io_burst_end, id);
            }
            break;
        case Event::Type::io_burst_end:
        case Event::Type::new_arrival:
            if (b.cpu_bursts.empty())
                break;
            ready_q.emplace(id, tau[id - 'A']);
            if (occupant == '\0') {
                switch_status = t + half_tcs;
                event_q.emplace(switch_status, Event::Type::switch_in, id);
                occupant = id;
                // ready_q.pop() moved to the end of this case
            } else if (should_preempt(Ready(id, tau[id - 'A']), occupant, switch_status)) {
                if (switch_status > 0 && switch_status == t + half_tcs) {
                    ready_q.emplace(occupant, tau[occupant - 'A'] - bursts_sofar[occupant - 'A']);
                    event_q.emplace(switch_status, Event::Type::switch_in, id);
                    occupant = id;
                } else if (switch_status == 0) { // No switch in progress
                    switch_status = -1;
                    event_q.emplace(t + half_tcs, Event::Type::switch_out, occupant);
                    bursts_sofar[occupant - 'A'] = total_sofar(occupant, false);
                    p_arrive(id, e.get_type() == Event::Type::new_arrival, occupant);
                    break; // Hacky method to skip p_arrive call at the end of this case
                }
                // If occupant is switching in, it will check for preemption when it's in
                // If occupant is already on its way out, nothing to be done
            }
            p_arrive(id, e.get_type() == Event::Type::new_arrival);
            // Must pop after p_arrive so it doesn't print [Q empty]
            if (occupant == id) ready_q.pop();
            break;
        case Event::Type::switch_out:
            if (ready_q.empty()) {
                occupant = '\0';
                switch_status = 0;
            } else {
                if (bursts_sofar[occupant - 'A'] != 0) // If occupant was preempted
                    ready_q.emplace(occupant, tau[occupant - 'A'] - bursts_sofar[occupant - 'A']);
                switch_status = t + half_tcs;
                event_q.emplace(switch_status, Event::Type::switch_in, ready_q.top().id);
                occupant = ready_q.top().id;
                ready_q.pop();
            }
        }
    }
    p_end();
}

void Srt::p_will_preempt(char elect, char occupant) const
{
    ptp(elect); std::cout << "will preempt " << occupant; pq();
}

bool Srt::should_preempt(const Ready candidate, char occupant, bool switching) const
{
    // Estimate occupant's remaining time
    const Ready o(occupant, tau[occupant - 'A'] - total_sofar(occupant, switching));
    return o < candidate;
}

int Srt::total_sofar(char id, bool switching) const
{
    return bursts_sofar[id - 'A'] + (switching ? 0 : t - cpu_start[id - 'A']);
}
