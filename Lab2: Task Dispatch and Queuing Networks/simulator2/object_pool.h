#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H

#include <vector>

// Efficiently allocate/free commonly reused objects
template <class T>
class ObjectPool
{
private:
    // ensure static data is properly initialized before use
    static std::vector<T*>& objectPool()
    {
        static std::vector<T*>* data = new std::vector<T*>;
        return *data;
    }

public:
    ObjectPool()
    {
    }
    ~ObjectPool()
    {
    }

    // create an object
    static T* create()
    {
        static std::vector<T*>& _objectPool = objectPool();
        if (_objectPool.empty()) {
            return new T();
        } else {
            T* obj = _objectPool.back();
            _objectPool.pop_back();
            return obj;
        }
    }

    // destroy an object
    static void destroy(T* obj)
    {
        static std::vector<T*>& _objectPool = objectPool();
        _objectPool.push_back(obj);
    }

    // delete objects
    static void finish()
    {
        for (T* obj : objectPool()) {
            delete obj;
        }
        objectPool().clear();
    }
};

#endif /* OBJECT_POOL_H */
