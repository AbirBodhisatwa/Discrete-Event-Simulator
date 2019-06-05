#include "request_handler.h"
#include "request.h"
#include "event.h"
#include "simulator.h"

// Add a completion event to the event queue at the given time for a given request
simulator::EventReference RequestHandler::addCompletionEvent(uint64_t timestamp, Request* req)
{
    //TODO fill in
    Event e;
    RequestHandler* rh = this;
    e.data.completionEvent.req = req;
    e.data.completionEvent.req_hlr = rh;
    e.timestamp = timestamp;
    e.callback = completionCallback;
    simulator :: EventReference er = simulator :: schedule(e);
    return er;

}

// Completion callback function
void completionCallback(const Event& e)
{ 
    //TODO fill in
    e.data.completionEvent.req_hlr->notifyEnd(e.data.completionEvent.req);    
}
