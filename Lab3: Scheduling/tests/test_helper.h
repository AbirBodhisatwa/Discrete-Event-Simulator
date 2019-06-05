#ifndef TEST_HELPER_H
#define TEST_HELPER_H

#include "../simulator.h"

namespace simulator
{
    inline void setSimTime(uint64_t newSimTime)
    {
        simTime = newSimTime;
    }
}

#endif /* TEST_HELPER_H */
