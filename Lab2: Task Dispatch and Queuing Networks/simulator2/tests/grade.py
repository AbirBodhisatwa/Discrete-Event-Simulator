#!/usr/bin/env python

import os
import json

def isclose(a, b, rel_tol=1e-09, abs_tol=0.0):
    return abs(a-b) <= max(rel_tol * max(abs(a), abs(b)), abs_tol)

if __name__ == '__main__':
    tests = [("DistributionDet", 2),
             ("DistributionExp", 2),
             ("Simulator", 4),
             ("ArrivalProcessDist", 2),
             ("RequestGeneratorDist", 2),
             ("RequestStreamOpen", 4),
             ("RequestStreamClosed", 4),
             ("StatsThroughput", 2),
             ("StatsLatency", 2),
             ("PerfModelServiceRate", 2),
             ("RequestHandlerFifoQueue", 4),
             ("DistributionSum", 5),
             ("DistributionMix", 5),
             ("PerfModelDist", 5),
             ("RequestHandlerDispatcherRandom", 5),
             ("RequestHandlerDispatcherRoundRobin", 5),
             ("RequestHandlerDispatcherJoinShortestQueue", 5),
             ("RequestHandlerDispatcherLeastWorkLeft", 5),
             ("overall", 15),
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
    testValue = 5
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
    testValue = 5
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
    testValue = 5
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
    testValue = 5
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
