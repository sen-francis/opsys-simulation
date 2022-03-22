#include "Algorithm.h"


bool operator<(const TauAlgorithm::Ready a, const TauAlgorithm::Ready b)
{
    return a.len >  b.len
       || (a.len == b.len && a.id > b.id);
}
