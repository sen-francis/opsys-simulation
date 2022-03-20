#include <iostream>
#include <set>

#include "../src/Event.h"


int main()
{
    std::set<Event> event_queue;
    event_queue.emplace(55, Event::Type::new_arrival, 'C');
    event_queue.emplace(57, Event::Type::new_arrival, 'B');
    event_queue.emplace(57, Event::Type::new_arrival, 'A');

    // Should print out C, then A, then B
    std::set<Event>::const_iterator next_event = event_queue.begin();
    while (next_event != event_queue.end()) {
        std::cout << "time " << next_event->get_time() << "ms: Process "
                  << next_event->get_process_id() << " arrived\n";
        ++next_event;
    }
    return 0;
}
