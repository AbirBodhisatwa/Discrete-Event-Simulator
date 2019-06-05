#ifndef REQUEST_HANDLER_DISPATCHER_ROUND_ROBIN_H
#define REQUEST_HANDLER_DISPATCHER_ROUND_ROBIN_H

#include "request_handler_dispatcher.h"
#include "request.h"
#include "json/json.h"

class RequestHandlerDispatcherRoundRobin : public RequestHandlerDispatcher
{
private:
    //TODO fill in
    int current_index;

public:
    RequestHandlerDispatcherRoundRobin(Json::Value& config);
    virtual ~RequestHandlerDispatcherRoundRobin();

    // Returns index of selected request handler to send request to
    virtual unsigned int selectRequestHandler(const Request* req, const std::vector<RequestHandler*>& reqHandlers);
};

#endif /* REQUEST_HANDLER_DISPATCHER_ROUND_ROBIN_H */
