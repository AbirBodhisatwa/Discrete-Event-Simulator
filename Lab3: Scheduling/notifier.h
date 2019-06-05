#ifndef NOTIFIER_H
#define NOTIFIER_H

#include "listener.h"

template <class T>
class NotifierStart
{
public:
    virtual ~NotifierStart()
    {
    }
    virtual void addListenerStart(ListenerStart<T>* listener) = 0;
};

template <class T>
class NotifierEnd
{
public:
    virtual ~NotifierEnd()
    {
    }
    virtual void addListenerEnd(ListenerEnd<T>* listener) = 0;
};

#endif /* NOTIFIER_H */
