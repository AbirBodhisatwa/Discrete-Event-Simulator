#ifndef LISTENER_H
#define LISTENER_H

template <class T>
class ListenerStart
{
public:
    virtual ~ListenerStart()
    {
    }
    virtual void notifyStart(T obj) = 0;
};

template <class T>
class ListenerEnd
{
public:
    virtual ~ListenerEnd()
    {
    }
    virtual void notifyEnd(T obj) = 0;
};

#endif /* LISTENER_H */
