#ifndef REQUEST_HANDLER_DISPATCHER_RANDOM_H
#define REQUEST_HANDLER_DISPATCHER_RANDOM_H

#include "request_handler_dispatcher.h"
#include "request.h"
#include "json/json.h"

class RequestHandlerDispatcherRandom : public RequestHandlerDispatcher
{
public:
    RequestHandlerDispatcherRandom(Json::Value& config);
    virtual ~RequestHandlerDispatcherRandom();

    // Returns index of selected request handler to send request to
    virtual unsigned int selectRequestHandler(const Request* req, const std::vector<RequestHandler*>& reqHandlers);
};

#endif /* REQUEST_HANDLER_DISPATCHER_RANDOM_H */
