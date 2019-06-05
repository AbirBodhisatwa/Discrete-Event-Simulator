#include "request_stream.h"
#include "request.h"
#include "event.h"

// Add an arrival event to the event queue for the next request in reqStream
void RequestStream::addArrivalEvent()
{
    //TODO fill in
    Event e;
    RequestStream* rs = this;
    Request* next_req = rs->next();
    if (next_req != NULL)
    {
     e.data.arrivalEvent.req = next_req;
     e.data.arrivalEvent.req_str = rs;
     e.timestamp = next_req->getArrivalTime();
     e.callback = arrivalCallback;
     simulator::schedule(e);
    }
    
}

// Arrival callback function
void arrivalCallback(const Event& e)
{
    //TODO fill in
     
    e.data.arrivalEvent.req_str->notifyStart(e.data.arrivalEvent.req);
}
