#include "RR.h"

void RR::run(const EventQ &arrivals, const std::vector<Bursts> &bursts, int half_tcs, int time_slice){
    event_q = arrivals;
    std::vector<Bursts> processes = bursts;
    std::vector<std::stack<int>> remaining(bursts.size());
    for(int i =0; i < bursts.size(); i++){
        remaining[i] = bursts[i].cpu_bursts;
    }
    // The process that is running on or switching into the CPU or '\0' if none
    char occupant = '\0';

    p_start();
    t = 0;
    while (!event_q.empty()) {
        const Event e = event_q.top();
        event_q.pop();

        t = e.get_time();
        const char id = e.get_process_id();
        Bursts &p = processes[id - 'A'];
        switch (e.get_type()) {
        case Event::Type::switch_in:
            if(remaining[id-'A'].top()!=p.cpu_bursts.top()){
                p_expire_finish(id, remaining[id-'A'].top(), p.cpu_bursts.top());
            }
            else{
                p_cpu_start(id, p.cpu_bursts.top());
                //t+=half_tcs;
            }
            if(remaining[id-'A'].top() > time_slice){
                event_q.emplace(t + time_slice, Event::Type::slice_expire, id);
            }
            else{
                event_q.emplace(t + remaining[id-'A'].top(), Event::Type::cpu_burst_end, id);
                p.cpu_bursts.pop();
                remaining[id-'A'].pop();
            }
            break;
        case Event::Type::slice_expire:
            if(!ready_q.empty()){
                //premption occurs, perform context switch
                //cout << "HERE : " << id << "\n";
                remaining[id-'A'].top() -= time_slice;
                p_expire_pre(id, p.cpu_bursts.top()-time_slice);
                //add current event to end of queue
                event_q.emplace(t+half_tcs, Event::Type::switch_out, id);
                ready_q.push(id);
            }
            else{
                //cout << "HERE : " << ready_q.size() << "\n";
                p_expire_no_pre();
                event_q.emplace(t-time_slice+p.cpu_bursts.top(), Event::Type::cpu_burst_end, id);
                p.cpu_bursts.pop();
                remaining[id-'A'].pop();
            }
            break;
        case Event::Type::cpu_burst_end:
            event_q.emplace(t + half_tcs, Event::Type::switch_out, id);
            if (p.io_bursts.empty())
                p_term(id);
            else {
                if(remaining[id-'A'].top()!=p.cpu_bursts.top()){
                    p_expire_finish(id, remaining[id-'A'].top(), p.cpu_bursts.top());
                }
                const int io_end = t + half_tcs + p.io_bursts.top();
                p.io_bursts.pop();
                p_cpu_end(id, p.cpu_bursts.size(), io_end);
                event_q.emplace(io_end, Event::Type::io_burst_end, id);
            }
            occupant = '\0';
            break;
        case Event::Type::io_burst_end:
        case Event::Type::new_arrival:
            if (p.cpu_bursts.empty())
                break;
            ready_q.push(id);
            p_arrive(id, e.get_type() == Event::Type::new_arrival);
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
    p_end();
}