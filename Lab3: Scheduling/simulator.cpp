#include <cassert>
#include <set>
#include "simulator.h"

namespace simulator
{
    // Event queue in timestamp order
    std::multiset<Event> queue;
    // Global simulator time (nanoseconds)
    uint64_t simTime = 0;

    // Add an event to the event queue
    EventReference schedule(const Event& e)
    {
        assert(e.timestamp >= getSimTime()); // ensure we don't go back in time
        //TODO fill in
        return queue.insert(e);
    }

    // Remove an event from the event queue
    void removeEvent(EventReference eventRef)
    {
        //TODO fill in
        queue.erase(eventRef);
    }

    // Run simulation until no more events
    void run()
    {
        //TODO fill in
        EventReference it;
        while (!queue.empty())
         {
           it = queue.begin();
           const Event& e = *it;
           simTime = e.timestamp;
           e.callback(e);
           removeEvent(it);
         }
    }

    // Run simulation until stopTime (nanoseconds)
    void run(uint64_t stopTime)
    {
        //TODO fill in
        EventReference it;
        while(!queue.empty())
          {
            it = queue.begin();
            const Event& e = *it;
            if (e.timestamp < stopTime)
            {
               simTime = e.timestamp;
               e.callback(e);
               removeEvent(it);
            }
           else
             break;
          }
         simTime = stopTime;
    }

    // Reset simulation
    void reset()
    {
        //TODO fill in
        simTime = 0;
	queue.clear();
    }
}
