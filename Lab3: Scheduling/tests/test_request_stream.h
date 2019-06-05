#ifndef TEST_REQUEST_STREAM_H
#define TEST_REQUEST_STREAM_H

#include <unordered_map>
#include "../request_stream.h"
#include "../request.h"
#include "../json/json.h"

class TestRequestStream : public RequestStream
{
private:
    Json::Value _config;
    unsigned int _index;
    std::unordered_map<Request*, uint64_t> _completionTimesMap;

public:
    TestRequestStream(Json::Value& config);

    virtual ~TestRequestStream();

    // Initialize request stream
    virtual void init();

    // Next request in stream
    virtual Request* next();

    // Called when request arrives
    virtual void notifyStart(Request* req);

    // Called when request is completed at request handler
    virtual void notifyEnd(Request* req);
};

void testRequestHandler(Json::Value& config, Json::Value& reqs);
void addTestRequestStreamConfig(Json::Value& reqs, double arrivalTime, double size, double completionTime);

#endif /* TEST_REQUEST_STREAM_H */
