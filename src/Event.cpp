#include "Event.h"

bool operator<(const Event &a, const Event &b)
{
    return a.get_time() <  b.get_time()
       || (a.get_time() == b.get_time() && int(a.get_type()) <  int(b.get_type()))
       || (a.get_time() == b.get_time() && int(a.get_type()) == int(b.get_type())
           && a.get_process_id() < b.get_process_id());
}

bool operator>(const Event &a, const Event &b)
{
    return b < a;
}
