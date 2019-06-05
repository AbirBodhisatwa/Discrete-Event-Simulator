#ifndef REQUEST_H
#define REQUEST_H

#include <cstdint>
#include <iostream>
#include "object_pool.h"
#include "simulator.h"
#include "perf_model.h"

// Request information
class Request
{
private:
    uint64_t _arrivalTime; // arrival time (nanoseconds)
    uint64_t _size; // request size in bytes
    uint64_t _remainingWork; // remaining work in nanoseconds

    // Private constructor/destructor to ensure all creation/destruction
    // is through object pool
    Request()
    {
    }
    virtual ~Request()
    {
    }

public:
    // Object pool will handle the creation/destruction of requests
    // via the create/destroy functions below
    friend class ObjectPool<Request>;
    
    // Create request
    static Request* create(uint64_t arrivalTime, uint64_t size)
    {
        Request* req = ObjectPool<Request>::create();
        if (req) {
            req->init(arrivalTime, size);
        }
        return req;
    }

    // Destroy request
    virtual void destroy()
    {
        ObjectPool<Request>::destroy(this);
    }

    // Initialize request
    void init(uint64_t arrivalTime, uint64_t size)
    {
        _arrivalTime = arrivalTime;
        _size = size;
        _remainingWork = 0;
    }

    // Get arrival time
    uint64_t getArrivalTime() const
    {
        return _arrivalTime;
    }

    // Get elapsed time (i.e., latency when called during request completion)
    uint64_t getElapsedTime() const
    {
        return simulator::getSimTime() - _arrivalTime;
    }

    // Get request size in bytes
    uint64_t getSize() const
    {
        return _size;
    }

    // Get request size in bytes
    void setSize(uint64_t size)
    {
        _size = size;
    }

    // Get remaining work in nanoseconds
    uint64_t getRemainingWork() const
    {
        return _remainingWork;
    }

    // Set remaining work in nanoseconds
    void setRemainingWork(uint64_t remainingWork)
    {
        _remainingWork = remainingWork;
    }
    // Set remaining work based on a performance model
    void setRemainingWork(PerfModel* model)
    {
        _remainingWork = model->estimateWork(this);
    }

    // Print request to stdout
    void print() const
    {
        std::cout << "t" << getArrivalTime() << " SIZE " << getSize() << std::endl;
    }
};

#endif /* REQUEST_H */
