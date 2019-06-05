#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <cstdint>
#include <set>
#include "event.h"

// Simulation event loop and global time
namespace simulator
{
    typedef std::multiset<Event>::const_iterator EventReference;

    // Global simulator time in nanoseconds (declared in simulator.cpp)
    extern uint64_t simTime;

    // Gets simulator time (nanoseconds)
    inline uint64_t getSimTime()
    {
        return simTime;
    }

    // Add an event to the event queue
    EventReference schedule(const Event& e);

    // Remove an event from the event queue
    void removeEvent(EventReference eventRef);

    // Run simulation until no more events
    void run();

    // Run simulation until stopTime (nanoseconds)
    void run(uint64_t stopTime);

    // Reset simulation
    void reset();
}

#endif /* SIMULATOR_H */
