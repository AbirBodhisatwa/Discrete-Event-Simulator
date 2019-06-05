#ifndef REQUEST_HANDLER_QUEUEING_NETWORK_H
#define REQUEST_HANDLER_QUEUEING_NETWORK_H

#include <unordered_map>
#include <map>
#include "request_handler.h"
#include "request.h"
#include "listener.h"
#include "json/json.h"

typedef std :: map <RequestHandler*, double> trans_prob; 

class RequestHandlerQueueingNetwork : public RequestHandler
{
private:
    //TODO fill in
    Json::Value config_copy;
    std :: map <RequestHandler*,double> init_prob;
    std :: vector <RequestHandler*> init_que_vect;
    std :: map <RequestHandler*,trans_prob> trans_list;
    std :: map <Request*, RequestHandler*> curr_state;
    std :: unordered_map<Request*, ListenerEnd<Request*>*> _requestCompletionCallbacks;
public:
    RequestHandlerQueueingNetwork(Json::Value& config);
    virtual ~RequestHandlerQueueingNetwork();

    virtual void init();

    // Handle request sent to server
    virtual void handleRequest(Request* req, ListenerEnd<Request*>* completionCallback);

    // Called when request is complete
    virtual void notifyEnd(Request* req);
};

#endif /* REQUEST_HANDLER_QUEUEING_NETWORK_H */
