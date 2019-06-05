README
======

This document contains the documentation for this discrete event simulator assignment 3.

Changes from assignment 2 to assignment 3:
------------------------------------------

Changed files:

* Makefile - changed to build new files
* tests/Makefile - changed to build with new files/tests
* tests/grade.py - updated for assignment 3 grading

New files:

* request_handler_random_queue.cpp/h
* request_handler_lcfs_queue.cpp/h
* request_handler_sjf_queue.cpp/h
* request_handler_plcfs_queue.cpp/h
* request_handler_psjf_queue.cpp/h
* request_handler_srpt_queue.cpp/h
* request_handler_ps_queue.cpp/h
* request_handler_fb_queue.cpp/h
* request_handler_central_queue_k.cpp/h
* stats_tail_latency.cpp/h
* tests/request_handler_random_queue_test.cpp
* tests/request_handler_lcfs_queue_test.cpp
* tests/request_handler_sjf_queue_test.cpp
* tests/request_handler_plcfs_queue_test.cpp
* tests/request_handler_psjf_queue_test.cpp
* tests/request_handler_srpt_queue_test.cpp
* tests/request_handler_ps_queue_test.cpp
* tests/request_handler_fb_queue_test.cpp
* tests/request_handler_central_queue_k_test.cpp
* tests/stats_tail_latency_test_test.cpp

Suggested order of completing the assignment:
---------------------------------------------

The components for you to implement are generally marked with:

`//TODO fill in`

You are free to add other code, but maintain the same interfaces to be able to test the code. Here is the suggested order of completing the assignment:

1.  Implement stats_tail_latency.cpp/h, which calculates tail latency stats for each time interval.
2.  Implement request_handler_random_queue.cpp/h, which selects the next job from the queue randomly.
3.  Implement request_handler_lcfs_queue.cpp/h, which selects the next job from the queue in a last come first serve order.
4.  Implement request_handler_sjf_queue.cpp/h, which selects the next job from the queue in a shortest job first order.
5.  Implement request_handler_plcfs_queue.cpp/h, which preemptively runs the jobs in last come first serve order.
6.  Implement request_handler_psjf_queue.cpp/h, which preemptively runs the jobs in a shortest job first order.
7.  Implement request_handler_srpt_queue.cpp/h, which preemptively runs the job with the shortest remaining processing time.
8.  Implement request_handler_ps_queue.cpp/h, which runs jobs in a processor sharing fashion.

9.  Implement request_handler_fb_queue.cpp/h, which runs jobs under the foreground-background policy where the jobs with the least attained service gets to run.
10. Implement request_handler_central_queue_k.cpp/h, which runs up to k jobs simultaneously and queues the other jobs in a fifo central queue.

Hints:
------

* For the tail latency stats, it is recommended to use the std::nth_element function and the provided getTailPercentileIndex function.
* To create a custom comparator for a sorted datastructure (e.g., std::set), you can write something like this:
class RequestCompare
{
public:
    bool operator() (Request* const& lhs, Request* const& rhs) const {
        //TODO return desired comparison result
    }
};
std::set<Request*, RequestCompare> _queue;
* If you use a custom comparator, the comparisons cannot change while the object is within the datastructure, otherwise, the datastructure will be corrupted.
* To handle preemption, you should consider how to use the removeEvent function in the simulator and the EventReference returned by addCompletionEvent.
* When preempting a request, you may consider updating its remaining work field through the setRemainingWork function.

Summary of classes/files:
-------------------------

ArrivalProcess - base class representing an open-loop sequence of arrivals over time
ArrivalProcessDist - arrival process with i.i.d. interarrival times following a given distribution
Distribution - base class representing a distribution
DistributionDet - deterministic distribution
DistributionExp - exponential distribution
DistributionSum - distribution corresponding to a sum of distributions
DistributionMix - distribution corresponding to a mixture of distributions, where each distribution has a probability of being selected
Event - simulator event class
Factory - factory design pattern for constructing objects
Listener - base class for supporting listening capability
main - contains the main function
Notifier - base class for supporting notification capability
ObjectPool - optimizes creation/destruction of frequently used objects (e.g., Request)
PerfModel - represents the performance model of a queue
PerfModelServiceRate - represents a queue that has a constant serviceRate (bytes/sec)
PerfModelDist - represents a queue that has random service times following a given distribution regardless of the size (bytes) of the request
random_helper - contains a global c++ random number generator and helper functions for seeding the generator
Request - class representing a request
RequestGenerator - base class for generating requests
RequestGeneratorDist - request generator for i.i.d. request sizes following a given distribution
RequestHandler - base class for an object that handles requests (e.g., queue, server, load balancer, etc)
RequestHandlerDispatcher - base class for dispatchers
RequestHandlerDispatcherRandom - random dispatcher
RequestHandlerDispatcherRoundRobin - round-robin dispatcher
RequestHandlerDispatcherJoinShortestQueue - join the shortest queue dispatcher
RequestHandlerDispatcherLeastWorkLeft - least work left dispatcher
RequestHandlerCentralQueueK - central first-in-first-out queue with k servers
RequestHandlerFbQueue - foreground-background queue
RequestHandlerFifoQueue - simple first-in-first-out queue
RequestHandlerLcfsQueue - last come first serve queue
RequestHandlerPlcfsQueue - preemptive last come first serve queue
RequestHandlerPsQueue - processor sharing queue
RequestHandlerPsjfQueue - preemptive shortest job first queue
RequestHandlerRandomQueue - random queue
RequestHandlerSjfQueue - shortest job first queue
RequestHandlerSrptQueue - shortest remaining processing time queue
RequestHandlerQueueingNetwork - represents a network of queues
RequestStream - base class for a sequence of requests
RequestStreamClosed - class representing a closed-loop request sequence
RequestStreamOpen - class representing an open-loop request sequence where the arrival process and request generation is independent of each other
run - contains the code to setup/teardown a simulation based on a JSON configuration (see below)
simulator - core discrete event simulator global functions
Stats - base class for statistics tracking
StatsLatency - measure mean latency of a RequestHandler
StatsTailLatency - measure tail latency of a RequestHandler
StatsThroughput - measure throughput of a RequestHandler

Simulator input configuration:
------------------------------

The simulator is configured via a JSON configuration format. JSON is an easy-to-use format for representing dictionaries (i.e., key-value pairs) and lists. Manipulating JSON is handled via the open-source JsonCpp library, included in the json directory. Two example config files are provided: tests/example_open.json and tests/example_closed.json. The format of the config file is as follows:

### Root level config:

* "seed": (optional) unsigned int - random number generator seed
* "stopTime": (optional) unsigned int - time to stop simulation (nanoseconds)
* "stats": (optional) list of stats configs - statistics to gather
* "requestHandlers": (optional) list of request handler configs - queues, servers, load balancers, etc in the simulation
* "requestStreams": (required) list of request stream configs - workloads in the simulation
* "distributions": (optional) list of distribution configs - distributions in the simulation
* "perfModels": (optional) list of performance model configs - performance models in the simulation

### Stats config:

* "name": (required) string - name to identify the stats collector
* "type": (required) string - stats class name (e.g., StatsLatency)
* "outputJson": (optional) bool - if true, stats will be included in output file
* "outputPrint": (optional) bool - if true, stats will be print to stdout while simulator runs
* "outputIntervalNs": (optional) unsigned int - time interval for outputting stats (nanoseconds)
* "targets": (required) list of strings - names of objects to collect stats for
* "percentiles": (required for StatsTailLatency) list of double - percentiles to measure, with each percentile in the range [0.0, 1.0] inclusive

### Request handler config:

* "name": (required) string - name to identify the request handler
* "type": (required) string - request handler class name (e.g., RequestHandlerFifoQueue)
* "perfModel": (required for RequestHandlerFifoQueue, RequestHandlerFbQueue, RequestHandlerLcfsQueue, RequestHandlerPlcfsQueue, RequestHandlerPsQueue, RequestHandlerPsjfQueue, RequestHandlerRandomQueue, RequestHandlerSjfQueue, RequestHandlerSrptQueue, and RequestHandlerCentralQueueK) performance model config or string - performance model config or name of performance model
* "reqHandlers": (required for any RequestHandlerDispatcher class) list of request handler configs or strings - list of names or configs of request handlers that the dispatcher will forward requests to
* "initialTransitionProbability": (required for RequestHandlerQueueingNetwork) list of transision probability configs - list of initial transition probabilities to the queues; probabilities should sum to 1.0
* "transitionProbabilities": (required for RequestHandlerQueueingNetwork) list of transition matrix configs - list of transition matrix probabilities for each queue
* "k": (required for RequestHandlerCentralQueueK) unsigned int - number of servers

### Transition probability config:

* "queue": (required) string - name of request handler (destination)
* "probability": (required) double - probability of sending a request to the queue

### Transition matrix config:

* "queue": (required) string - name of request handler (source)
* "transitionProbability": (required) list of transision probability configs - list of transition probabilities from the source queue to the other destination queues; probabilities should sum to <= 1.0, with any leftover probability indicating a departure from the queueing network

### Request stream config:

* "name": (required) string - name to identify the request stream
* "type": (required) string - request stream class name (e.g., RequestStreamOpen)
* "requestHandler": (required) request handler config or string - request handler config or name of request handler to send stream of requests to
* "requestGeneratorConfig": (required for RequestStreamOpen and RequestStreamClosed) request generator config or string - request generator config or name of request generator
* "arrivalProcessConfig": (required for RequestStreamOpen) arrival process config or string - arrival process config or name of arrival process
* "thinkTime": (required for RequestStreamClosed) distribution config or string - think time distribution (seconds) or name of think time distribution
* "MPL": (required for RequestStreamClosed) unsigned int - number of concurrent requests

### Request generator config:

* "name": (required) string - name to identify the request generator
* "type": (required) string - request generator class name (e.g., RequestGeneratorDist)
* "dist": (required for RequestGeneratorDist) distribution config or string - request size distribution (bytes) or name of request size distribution

### Arrival process config:

* "name": (required) string - name to identify the arrival process
* "type": (required) string - arrival process class name (e.g., ArrivalProcessDist)
* "dist": (required for ArrivalProcessDist) distribution config or string - interarrival distribution (seconds) or name of interarrival distribution

### Distribution config:

* "name": (required) string - name to identify the distribution
* "type": (required) string - distribution class name (e.g., DistributionExp)
* "rate": (required for DistributionExp) double - exponential distribution rate
* "val": (required for DistributionDet) double - deterministic distribution value
* "dists": (required for DistributionSum and DistributionMix) list of distribution configs or strings - list of names or configs of distributions
* "probabilities": (required for DistributionMix) list of double - list of probabilities for each distribution in the dists list. Probabilities and distributions are correlated by list index, and the probabilities should sum to 1.0

### Performance model config:
* "name": (required) string - name to identify the performance model
* "type": (required) string - performance model class name (e.g., PerfModelServiceRate)
* "serviceRate": (required for PerfModelServiceRate) double - service rate in bytes/sec
* "dist": (required for PerfModelDist) distribution config or string - service time distribution (seconds) or name of service time distribution; Note that service time ignores the request size (bytes)

Your implementation should follow the above config format and support all of the options. The "name" and "type" keys are automatically handled as part of factory.h. Some of the other keys are handled in the starter code. You are responsible for supporting the rest of the options that aren't setup in the starter files.

When creating config files, note that all object names should be unique. Managing the creation/destruction of all these objects are handled via the factory design pattern (see factory.h), and is already setup in the starter code. Adding an additional class type only requires including factory.h and using the REGISTER_CLASS macro.

To build the code:
------------------

Run:

`make`

The makefile supports incremental builds and automatically tracks header dependencies. To add another source file, simply add OBJS += source_file.o to the Makefile.

To run the simulator:
---------------------

Run:

`./simulator tests/example_open.json`

The parameter contains the path of the config file (see above). The output file will be written to pathname_res.json. For example, tests/example_open.json_res.json. The output will be in JSON format, and will contain the entire input file, along with the list of stats indexed by name in "results".

Test code:
----------

Test code is located in the tests directory and is written in the Catch framework (see tests/catch/catch.hpp). To build the test code, change to the tests directory and run:

`make`

Then the tests can be run via:

`./simulatorTest`

The grading script can be run via:

`./grade.py`

Note that while some test cases are provided, it is not exhaustive, and you should perform your own testing and supplement the tests as you see fit.

Debugging:
----------

To help with debugging, you can compile the code with debug symbols by running the following commands in both the main directory and tests directory:

`make clean`
`make debug`

From here, you can run your program with gdb:

`gdb simulator`

or the tests in the tests directory:

`gdb simulatorTest`

Using gdb will be extremely helpful in finding segfaults and stepping through code to identify bugs.

Handin instructions:
--------------------

Run:

`make handin`

The Makefile is set to automatically clean when generating the handin, but please remove any large files unaffected by make clean. Upload the resulting handin.tar.gz to canvas.
