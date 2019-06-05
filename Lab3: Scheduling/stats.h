#ifndef STATS_H
#define STATS_H

#include <cassert>
#include "listener.h"
#include "request.h"
#include "request_handler.h"
#include "factory.h"
#include "json/json.h"
#include "simulator.h"

// Base class for statistics tracking
class Stats : public ListenerEnd<Request*>
{
private:
    uint64_t _outputIntervalNs; // write statistics every interval (nanoseconds)
    uint64_t _lastIntervalTime; // time of last interval (nanoseconds)
    bool _outputJson; // whether to write statistics to json
    bool _outputPrint; // whether to write statistics to stdout
    Json::Value _results; // stores statistical results over time
    Json::Value& _targets; // target objects to track statistics

    // Functions for outputting statistics
    // Stats classes should override these functions for outputting stats
    virtual void printStats(uint64_t lastIntervalBegin, uint64_t lastIntervalEnd)
    {
    }
    virtual Json::Value jsonStats(uint64_t lastIntervalBegin, uint64_t lastIntervalEnd)
    {
        return Json::nullValue;
    }

    // Internal functions for binding stats objects to monitored objects
    template<typename T>
    bool bind(std::string name)
    {
        T* target = Factory<T>::getObjectByName(name);
        if (target == nullptr)
        {
            return false;
        }
        target->addListenerEnd(this);
        return true;
    }

    template<typename T, typename E, typename... Next>
    bool bind(std::string name)
    {
        return bind<T>(name) ? true : bind<E, Next...>(name);
    }

    template<typename T>
    bool bind(Json::Value& config)
    {
        for (auto &it : config)
        {
            if (!bind<T>(it.asString()))
            {
                return false;
            }
        }
        return true;
    }

    template<typename T, typename E, typename... Next>
    bool bind(Json::Value& config)
    {
        for (auto &it : config)
        {
            bool result = bind<T, E, Next...>(it.asString());
            if (!result)
            {
                return false;
            }
        }
        return true;
    }

public:
    Stats(Json::Value& config)
        : _outputIntervalNs(0),
          _lastIntervalTime(0),
          _outputJson(false),
          _outputPrint(false),
          _results(Json::nullValue),
          _targets(config["targets"])
    {
        if (config.isMember("outputJson")) {
            _outputJson = config["outputJson"].asBool();
        }
        if (config.isMember("outputPrint")) {
            _outputPrint = config["outputPrint"].asBool();
        }
        if (config.isMember("outputIntervalNs")) {
            _outputIntervalNs = config["outputIntervalNs"].asUInt64();
        }
    }
    virtual ~Stats()
    {
    }
    // Initialize statistics tracking
    virtual void init()
    {
        bool bSuccess = bind<RequestHandler>(_targets);
        assert(bSuccess);
    }
    // Outputs statistics if an interval has passed (or forced with bForceInterval true)
    // Returns if an interval occurred
    bool checkOutputStats(bool bForceInterval = false)
    {
        bool bInterval = false;
        if (bForceInterval || (simulator::getSimTime() >= _lastIntervalTime + _outputIntervalNs)) {
            uint64_t lastIntervalTime = _lastIntervalTime;
            // Update last interval time
            if (bForceInterval) {
                _lastIntervalTime = simulator::getSimTime();
                bInterval = true;
            } else if (_outputIntervalNs > 0) {
                while (simulator::getSimTime() >= _lastIntervalTime + _outputIntervalNs) {
                    _lastIntervalTime += _outputIntervalNs;
                }
                bInterval = true;
            }
            // Output stats
            if (_outputJson) {
                _results.append(jsonStats(lastIntervalTime, _lastIntervalTime));
            }
            if (_outputPrint) {
                printStats(lastIntervalTime, _lastIntervalTime);
            }
        }
        return bInterval;
    }
    // Gets outputted JSON stats
    Json::Value getStats()
    {
        return _results;
    }
    // Clear stored JSON stats
    void clearStats()
    {
        _results = Json::nullValue;
    }
    // Reset stats for current interval
    // Stats classes should override this function if there is state to reset every interval
    virtual void resetStats()
    {
    }
};

#endif /* STATS_H */
