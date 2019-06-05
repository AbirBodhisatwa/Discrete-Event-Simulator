#ifndef REQUEST_HANDLER_DISPATCHER_JOIN_SHORTEST_QUEUE_H
#define REQUEST_HANDLER_DISPATCHER_JOIN_SHORTEST_QUEUE_H

#include "request_handler_dispatcher.h"
#include "request.h"
#include "json/json.h"

class RequestHandlerDispatcherJoinShortestQueue : public RequestHandlerDispatcher
{
public:
    RequestHandlerDispatcherJoinShortestQueue(Json::Value& config);
    virtual ~RequestHandlerDispatcherJoinShortestQueue();

    // Returns index of selected request handler to send request to
    virtual unsigned int selectRequestHandler(const Request* req, const std::vector<RequestHandler*>& reqHandlers);
};

#endif /* REQUEST_HANDLER_DISPATCHER_JOIN_SHORTEST_QUEUE_H */
