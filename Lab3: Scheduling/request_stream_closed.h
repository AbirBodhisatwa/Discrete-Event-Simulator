#ifndef REQUEST_STREAM_CLOSED_H
#define REQUEST_STREAM_CLOSED_H

#include "json/json.h"
#include "request_stream.h"
#include "distribution.h"
#include "request_generator.h"

// Closed-loop request sequence
class RequestStreamClosed : public RequestStream
{
private:
    //TODO fill in
    RequestGenerator* _req_gen;

    Distribution* _think_dist_obj;
    //const char* think_dist_name;
    //const char* think_dist_type;
    //double think_dist_value;

    uint64_t _initial_time;
    int MPL;
  

public:
    RequestStreamClosed(Json::Value& config);
    virtual ~RequestStreamClosed();

    //TODO fill in
     void init();
     Request* next();
     void notifyEnd(Request *req);
};

#endif /* REQUEST_STREAM_CLOSED_H */
