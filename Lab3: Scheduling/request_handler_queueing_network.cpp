#include "request_handler_queueing_network.h"
#include "request.h"
#include "random_helper.h"
#include "factory.h"

#include <stdlib.h>
#include <iterator>

REGISTER_CLASS(RequestHandler, RequestHandlerQueueingNetwork)

//TODO fill in
RequestHandlerQueueingNetwork :: RequestHandlerQueueingNetwork(Json::Value& config):RequestHandler(config)
{
     config_copy = config;   
}

RequestHandlerQueueingNetwork :: ~RequestHandlerQueueingNetwork()
{

}

bool not_at_end(std :: map <RequestHandler*, trans_prob> trans_list, RequestHandler* curr_state)
{
   std :: map<RequestHandler*, trans_prob> :: iterator it;
   it = trans_list.find(curr_state);
   if (it != trans_list.end())
      return true;
   return false;
}

RequestHandler* getRequest(const std::vector<RequestHandler*>& req_han, std:: map<RequestHandler*,double> prob_list)
{
  double p = uniform01();
  double cp = 0.0;

  for (std::vector<RequestHandler*>::const_iterator it = req_han.begin() ; it != req_han.end(); ++it)
  {
    cp += prob_list[*it];
    if (cp >= p)
    {
      return *it;
    }
  }
  return NULL; 
}

std :: vector<RequestHandler*> getPossibleTransitions(std :: map <RequestHandler*,double> m)
{
 std :: vector <RequestHandler*> keys;

 for (std :: map <RequestHandler*,double> :: iterator it = m.begin(); it != m.end(); ++it )
   {
        keys.push_back(it->first);
   }
  return keys;
}
void RequestHandlerQueueingNetwork :: init()
{
   int size  = (int)config_copy["initialTransitionProbability"].size();
   Json :: Value inner_config = config_copy["transitionProbabilities"];
   int size2 = (int)inner_config.size();
   
    for (int i = 0; i < size; i ++) //for the initial probability list
    {
     RequestHandler* obj = Factory<RequestHandler>::create(config_copy["initialTransitionProbability"][i]["queue"]);
     init_que_vect.push_back(obj);

   init_prob.insert(std::pair <RequestHandler*,double> (obj,config_copy["initialTransitionProbability"][i]["probability"].asDouble()));
    }

    for (int i = 0; i < size2; i ++) //for the transition probability matrix
    {
     RequestHandler* obj = Factory<RequestHandler>::create(inner_config[i]["queue"]);
     Json :: Value innermost_config = inner_config[i]["transitionProbability"]; 
     int s = (int)innermost_config.size();
     trans_prob t1;

     for (int j = 0; j < s; j ++)
     {
       RequestHandler* obj2 = Factory<RequestHandler>::create(innermost_config[j]["queue"]);

       t1.insert(std::pair <RequestHandler*,double> (obj2,innermost_config[j]["probability"].asDouble()));
     }
       trans_list.insert(std::pair <RequestHandler*,trans_prob> (obj,t1));
    }
}

void RequestHandlerQueueingNetwork :: handleRequest(Request* req, ListenerEnd<Request*>* completionCallback)
{
    RequestHandler* req_h = getRequest(init_que_vect, init_prob);//call handle request for this queue
    req_h->handleRequest(req, this);
    curr_state[req] = req_h;
   
   _requestCompletionCallbacks[req] = completionCallback;
}

void RequestHandlerQueueingNetwork :: notifyEnd(Request* req)
{
  //first get a request here and call handleRequest
  //call completionCallback for the whole thing
  RequestHandler* req_h;
  if(not_at_end(trans_list,curr_state[req]))
  {
    std :: vector<RequestHandler*> poss_trans = getPossibleTransitions(trans_list[curr_state[req]]);
    req_h = getRequest(poss_trans,trans_list[curr_state[req]]);
  }
  else
  {
    req_h = NULL;
  }
  
  if (req_h == NULL) //request is out of the network
   {
     ListenerEnd<Request*>* completionCallback = _requestCompletionCallbacks[req];
    _requestCompletionCallbacks.erase(req);
    curr_state.erase(req);
    notifyListenersEnd(req);
    completionCallback->notifyEnd(req);
   }
  else
   {
     req_h->handleRequest(req, this);
     curr_state[req] = req_h;
   }
}
