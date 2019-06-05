#ifndef REQUEST_HANDLER_DISPATCHER_LEAST_WORK_LEFT_H
#define REQUEST_HANDLER_DISPATCHER_LEAST_WORK_LEFT_H

#include "request_handler_dispatcher.h"
#include "request.h"
#include "json/json.h"

class RequestHandlerDispatcherLeastWorkLeft : public RequestHandlerDispatcher
{
public:
    RequestHandlerDispatcherLeastWorkLeft(Json::Value& config);
    virtual ~RequestHandlerDispatcherLeastWorkLeft();

    // Returns index of selected request handler to send request to
    virtual unsigned int selectRequestHandler(const Request* req, const std::vector<RequestHandler*>& reqHandlers);
};

#endif /* REQUEST_HANDLER_DISPATCHER_LEAST_WORK_LEFT_H */
