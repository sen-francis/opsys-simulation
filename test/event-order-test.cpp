#include <iostream>
#include <set>

#include "../src/Algorithm.h"


int main()
{
    EventQ event_q;
    event_q.emplace(55, Event::Type::new_arrival, 'C');
    event_q.emplace(57, Event::Type::new_arrival, 'B');
    event_q.emplace(57, Event::Type::new_arrival, 'A');

    // Should print out C, then A, then B
    while (!event_q.empty()) {
        std::cout << "time " << event_q.top().get_time() << "ms: Process "
                  << event_q.top().get_process_id() << " arrived\n";
        event_q.pop();
    }
    return 0;
}
