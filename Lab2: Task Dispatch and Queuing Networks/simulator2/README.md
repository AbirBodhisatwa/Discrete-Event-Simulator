README
======

This document contains the documentation for this discrete event simulator assignment 2.

Changes from assignment 1 to assignment 2:
------------------------------------------

Changed files:

* Makefile - changed to build new files
* tests/Makefile - changed to build with new files/tests
* tests/memory_leak_test.cpp - updated to include queueing network examples
* tests/grade.py - updated for assignment 2 grading

New files:

* distribution_sum.h
* distribution_sum.cpp
* distribution_mix.h
* distribution_mix.cpp
* perf_model_dist.h
* perf_model_dist.cpp
* request_handler_dispatcher.h
* request_handler_dispatcher.cpp
* request_handler_dispatcher_random.h
* request_handler_dispatcher_random.cpp
* request_handler_dispatcher_round_robin.h
* request_handler_dispatcher_round_robin.cpp
* request_handler_dispatcher_join_shortest_queue.h
* request_handler_dispatcher_join_shortest_queue.cpp
* request_handler_dispatcher_least_work_left.h
* request_handler_dispatcher_least_work_left.cpp
* request_handler_queueing_network.h
* request_handler_queueing_network.cpp
* tests/example_open_network.json
* tests/example_closed_network.json
* tests/request_handler_queueing_network_initial_probability_test.json
* tests/request_handler_queueing_network_loop_test.json
* tests/request_handler_queueing_network_many_links_test.json
* tests/request_handler_queueing_network_transition_probability_test.json
* tests/distribution_sum_test.cpp
* tests/distribution_mix_test.cpp
* tests/perf_model_dist_test.cpp
* tests/request_handler_dispatcher_random_test.cpp
* tests/request_handler_dispatcher_round_robin_test.cpp
* tests/request_handler_dispatcher_join_shortest_queue_test.cpp
* tests/request_handler_dispatcher_least_work_left_test.cpp

Suggested order of completing the assignment:
---------------------------------------------

The components for you to implement are generally marked with:

`//TODO fill in`

You are free to add other code, but maintain the same interfaces to be able to test the code. Here is the suggested order of completing the assignment:

1. Implement distribution_sum.cpp/h, which is a distribution that is the sum of a set of other distributions. This can be used, for example, to represent a hypoexponential.
2. Implement distribution_mix.cpp/h, which randomly chooses a distribution from a set of distributions according to given probabilities. This can be used, for example, to represent a hyperexponential.
3. Implement perf_model_dist.cpp/h, which calculates the service time (i.e., work) by randomly drawing from a given distribution. Note that the request size is ignored. Also note that the distribution returns the service time in seconds, whereas the function returns the service time in nanoseconds.
4. Read request_handler_dispatcher.cpp/h to see how selectRequestHandler is used. This function is overridden in child classes.
5. Implement request_handler_dispatcher_random.cpp/h, which selects a queue randomly.
6. Implement request_handler_dispatcher_round_robin.cpp/h, which selects a queue in round-robin order.
7. Implement request_handler_dispatcher_join_shortest_queue.cpp/h, which selects the queue with the shortest queue length.
8. Implement request_handler_dispatcher_least_work_left.cpp/h, which selects the queue with the least remaining work left.
9. Implement request_handler_queueing_network.cpp/h, which represents a queueing network. See the "initialTransitionProbability", "transitionProbabilities", Transition probability config, and Transition matrix config descriptions below for details on the configuration parameters for the queueing network.

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
RequestHandlerFifoQueue - simple first-in-first-out queue
RequestHandlerQueueingNetwork - represents a network of queues
RequestStream - base class for a sequence of requests
RequestStreamClosed - class representing a closed-loop request sequence
RequestStreamOpen - class representing an open-loop request sequence where the arrival process and request generation is independent of each other
run - contains the code to setup/teardown a simulation based on a JSON configuration (see below)
simulator - core discrete event simulator global functions
Stats - base class for statistics tracking
StatsLatency - measure mean latency of a RequestHandler
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

### Request handler config:

* "name": (required) string - name to identify the request handler
* "type": (required) string - request handler class name (e.g., RequestHandlerFifoQueue)
* "perfModel": (required for RequestHandlerFifoQueue) performance model config or string - performance model config or name of performance model
* "reqHandlers": (required for any RequestHandlerDispatcher class) list of request handler configs or strings - list of names or configs of request handlers that the dispatcher will forward requests to
* "initialTransitionProbability": (required for RequestHandlerQueueingNetwork) list of transision probability configs - list of initial transition probabilities to the queues; probabilities should sum to 1.0
* "transitionProbabilities": (required for RequestHandlerQueueingNetwork) list of transition matrix configs - list of transition matrix probabilities for each queue

### Transition probability config:

* "queue": (required) request handler config or string - name or config of request handler (destination)
* "probability": (required) double - probability of sending a request to the queue

### Transition matrix config:

* "queue": (required) request handler config or string - name or config of request handler (source)
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
