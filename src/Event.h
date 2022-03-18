#ifndef EVENT_H
#define EVENT_H


// Event represents one of three types of interesting events in the simulation:
// the end of a CPU burst, the end of an I/O burst, or the arrival of a new
// process. Only this subset of the interesting events in the section "Required
// terminal output" of opsys-simulation.pdf must be tracked in an event queue.
class Event {
public:
    // Must be declared in tie-break order (see section Handling "ties" in
    // opsys-simulation.pdf)
    enum class Type {
        cpu_burst_end,
        io_burst_end,
        new_process_arrival,
    };
    // time is when the event will occur in milliseconds since simulation start,
    // and 'A' <= process_id <= 'Z'
    Event(int time, Type type, char process_id)
        : time(time), type(type), process_id(process_id) {}
    int  get_time()       const { return time; }
    Type get_type()       const { return type; }
    char get_process_id() const { return process_id; }
private:
    int time;
    Type type;
    char process_id;
};

// Events are sorted first by time, then by type, then by process ID (see
// section Handling "ties" in opsys-simulation.pdf)
bool operator<(const Event &a, const Event &b)
{
    return a.get_time() <  b.get_time()
       || (a.get_time() == b.get_time() && int(a.get_type()) <  int(b.get_type()))
       || (a.get_time() == b.get_time() && int(a.get_type()) == int(b.get_type())
           && a.get_process_id() < b.get_process_id());
}


#endif // EVENT_H
