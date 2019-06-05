#ifndef EVENT_H
#define EVENT_H

#include <cstdint>
#include "listener.h"

class Request;
class RequestStream;
class RequestHandler;

// Arrival event data
struct ArrivalEventData
{
    //TODO fill in
    Request* req;
    RequestStream *req_str;     
};

// Completion event data
struct CompletionEventData
{
    //TODO fill in
    Request* req;
    RequestHandler* req_hlr;
};

// Union of all event datatypes
union EventData
{
    ArrivalEventData arrivalEvent;
    CompletionEventData completionEvent;
};

// Simulation event datatype
struct Event
{
    uint64_t timestamp; // time (nanoseconds) at which callback is invoked
    void (*callback)(const Event& e); // callback to invoke
    EventData data; // event data

    // Event timestamp comparator for simulator
    bool operator<(const Event& e) const
    {
        //TODO fill in
        if (e.timestamp > this->timestamp)
           return true;
        
        return false;
    }
};

#endif /* EVENT_H */
