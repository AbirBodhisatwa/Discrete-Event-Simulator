#ifndef REQUEST_HANDLER_DISPATCHER_H
#define REQUEST_HANDLER_DISPATCHER_H

#include <vector>
#include "request_handler.h"
#include "request.h"
#include "json/json.h"

class RequestHandlerDispatcher : public RequestHandler
{
private:
    Json::Value _config;
    std::vector<RequestHandler*> _reqHandlers;

public:
    RequestHandlerDispatcher(Json::Value& config);
    virtual ~RequestHandlerDispatcher();

    virtual void init();

    // Returns index of selected request handler to send request to
    virtual unsigned int selectRequestHandler(const Request* req, const std::vector<RequestHandler*>& reqHandlers) = 0;

    // Handle request sent to server
    virtual void handleRequest(Request* req, ListenerEnd<Request*>* completionCallback);

    // Called when request is complete
    virtual void notifyEnd(Request* req);
};

#endif /* REQUEST_HANDLER_DISPATCHER_H */
