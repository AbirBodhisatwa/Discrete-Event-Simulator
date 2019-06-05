#!/usr/bin/env python

import os
import json

def isclose(a, b, rel_tol=1e-09, abs_tol=0.0):
    return abs(a-b) <= max(rel_tol * max(abs(a), abs(b)), abs_tol)

if __name__ == '__main__':
    tests = [("DistributionDet", 1),
             ("DistributionExp", 1),
             ("Simulator", 1),
             ("ArrivalProcessDist", 1),
             ("RequestGeneratorDist", 1),
             ("RequestStreamOpen", 1),
             ("RequestStreamClosed", 1),
             ("StatsThroughput", 1),
             ("StatsLatency", 1),
             ("PerfModelServiceRate", 1),
             ("RequestHandlerFifoQueue", 2),
             ("DistributionSum", 1),
             ("DistributionMix", 1),
             ("PerfModelDist", 1),
             ("RequestHandlerDispatcherRandom", 1),
             ("RequestHandlerDispatcherRoundRobin", 1),
             ("RequestHandlerDispatcherJoinShortestQueue", 1),
             ("RequestHandlerDispatcherLeastWorkLeft", 1),
             ("StatsTailLatency", 5),
             ("RequestHandlerRandomQueue", 5),
             ("RequestHandlerLcfsQueue", 5),
             ("RequestHandlerSjfQueue", 8),
             ("RequestHandlerPlcfsQueue", 8),
             ("RequestHandlerPsjfQueue", 8),
             ("RequestHandlerSrptQueue", 8),
             ("RequestHandlerPsQueue", 8),
             ("RequestHandlerFbQueue", 8),
             ("RequestHandlerCentralQueueK", 8),
             ("overall", 2),
    ]
    points = 0
    for testName, testValue in tests:
        status = os.system("./simulatorTest [%s]" %(testName))
        if status == 0:
            print "%s [%dpts]: passed" %(testName, testValue)
            points += testValue
        else:
            print "%s [%dpts]: failed" %(testName, testValue)

    testName = "request_handler_queueing_network_many_links_test.json"
    testValue = 2
    os.system("rm %s_res.json" %(testName))
    os.system("../simulator %s" %(testName))
    if os.path.isfile("%s_res.json" %(testName)):
        with open("%s_res.json" %(testName), "r") as fpjson:
            results = json.load(fpjson)
            if ((isclose(results["results"]["throughput1"][0]["X"], 999.9999, rel_tol=1e-3)) and
                (isclose(results["results"]["throughput2"][0]["X"], 999.9999, rel_tol=1e-3)) and
                (isclose(results["results"]["throughput3"][0]["X"], 999.9999, rel_tol=1e-3))):
                print "%s [%dpts]: passed" %(testName, testValue)
                points += testValue
            else:
                print "%s [%dpts]: failed" %(testName, testValue)
    else:
        print "%s [%dpts]: failed" %(testName, testValue)

    testName = "request_handler_queueing_network_initial_probability_test.json"
    testValue = 2
    os.system("rm %s_res.json" %(testName))
    os.system("../simulator %s" %(testName))
    if os.path.isfile("%s_res.json" %(testName)):
        with open("%s_res.json" %(testName), "r") as fpjson:
            results = json.load(fpjson)
            if ((isclose(results["results"]["throughput1"][0]["X"], 500, rel_tol=1e-3)) and
                (isclose(results["results"]["throughput2"][0]["X"], 333.333, rel_tol=1e-3)) and
                (isclose(results["results"]["throughput3"][0]["X"], 166.667, rel_tol=1e-3))):
                print "%s [%dpts]: passed" %(testName, testValue)
                points += testValue
            else:
                print "%s [%dpts]: failed" %(testName, testValue)
    else:
        print "%s [%dpts]: failed" %(testName, testValue)

    testName = "request_handler_queueing_network_transition_probability_test.json"
    testValue = 2
    os.system("rm %s_res.json" %(testName))
    os.system("../simulator %s" %(testName))
    if os.path.isfile("%s_res.json" %(testName)):
        with open("%s_res.json" %(testName), "r") as fpjson:
            results = json.load(fpjson)
            if ((isclose(results["results"]["throughput1"][0]["X"], 333.333, rel_tol=1e-3)) and
                (isclose(results["results"]["throughput2"][0]["X"], 166.667, rel_tol=1e-3)) and
                (isclose(results["results"]["throughput3"][0]["X"], 500, rel_tol=1e-3))):
                print "%s [%dpts]: passed" %(testName, testValue)
                points += testValue
            else:
                print "%s [%dpts]: failed" %(testName, testValue)
    else:
        print "%s [%dpts]: failed" %(testName, testValue)

    testName = "request_handler_queueing_network_loop_test.json"
    testValue = 2
    os.system("rm %s_res.json" %(testName))
    os.system("../simulator %s" %(testName))
    if os.path.isfile("%s_res.json" %(testName)):
        with open("%s_res.json" %(testName), "r") as fpjson:
            results = json.load(fpjson)
            if isclose(results["results"]["throughput1"][0]["X"], 4000, rel_tol=1e-3):
                print "%s [%dpts]: passed" %(testName, testValue)
                points += testValue
            else:
                print "%s [%dpts]: failed" %(testName, testValue)
    else:
        print "%s [%dpts]: failed" %(testName, testValue)

    print "Total score: %d" %(points)
