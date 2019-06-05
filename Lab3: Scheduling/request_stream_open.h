#ifndef REQUEST_STREAM_OPEN_H
#define REQUEST_STREAM_OPEN_H

#include "json/json.h"
#include "request_stream.h"
#include "arrival_process.h"
#include "request_generator.h"

// Open-loop request sequence
class RequestStreamOpen : public RequestStream
{
private:
    //TODO fill in
    uint64_t _initial_time;
    ArrivalProcess* _arrival;
    RequestGenerator* _req_gen;

public:
    RequestStreamOpen(Json::Value& config);
    virtual ~RequestStreamOpen();

    //TODO fill in
    void init();
    Request* next();
    void notifyStart(Request *req);
};

#endif /* REQUEST_STREAM_OPEN_H */
