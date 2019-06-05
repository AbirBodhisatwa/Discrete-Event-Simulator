#ifndef FACTORY_H
#define FACTORY_H
#include <cassert>
#include <string>
#include <unordered_map>
#include "json/json.h"

// factory class for creating all objects
template <class T>
class Factory
{
private:
    // ensure static data is properly initialized before use
    static std::unordered_map<std::string, T*>& nameInstanceMap()
    {
        static std::unordered_map<std::string, T*>* data = new std::unordered_map<std::string, T*>;
        return *data;
    }
    static std::unordered_map<std::string, Factory<T>*>& typeFactoryMap()
    {
        static std::unordered_map<std::string, Factory<T>*>* data = new std::unordered_map<std::string, Factory<T>*>;
        return *data;
    }

    virtual T* createObject(Json::Value& config) = 0;

protected:
    // register a type name to the corresponding factory
    static void registerType(std::string type, Factory<T>* factory)
    {
        typeFactoryMap()[type] = factory;
    }

public:
    Factory()
    {
    }
    virtual ~Factory()
    {
    }

    // create an object
    static T* create(Json::Value& config)
    {
        if (config.isString()) {
            return getObjectByName(config.asString());
        } else {
            // check all necessary properties are set
            assert(config.isMember("type"));
            assert(config.isMember("name"));
            // check type exists
            assert(typeFactoryMap().count(config["type"].asString()) == 1);
            // create object
            T* obj = typeFactoryMap()[config["type"].asString()]->createObject(config);
            // check object name still available
            assert(nameInstanceMap().count(config["name"].asString()) == 0);
            // store in object map
            nameInstanceMap()[config["name"].asString()] = obj;
            return obj;
        }
    }

    // destroy an object (if it exists)
    static void destroy(std::string name)
    {
        // find object
        auto it = nameInstanceMap().find(name);
        if (it != nameInstanceMap().end()) {
            // delete object
            T* obj = it->second;
            nameInstanceMap().erase(it);
            delete obj;
        }
    }

    // lookup an object by its name (set in the json)
    static T* getObjectByName(std::string name)
    {
        return nameInstanceMap()[name];
    }

    // get all objects
    static const std::unordered_map<std::string, T*>& getObjects()
    {
        return nameInstanceMap();
    }

    // delete objects
    static void finish()
    {
        for (auto &it : nameInstanceMap()) {
            delete it.second;
        }
        nameInstanceMap().clear();
    }
};

// class for registering classes with factory
template <class T, class ClassType>
class ClassFactory : public Factory<T>
{
private:
    virtual T* createObject(Json::Value& config)
    {
        return new ClassType(config);
    }

public:
    ClassFactory(std::string type)
    {
        Factory<T>::registerType(type, this);
    }
    virtual ~ClassFactory()
    {
    }
};

// macro for registering classes with factory
#define REGISTER_CLASS(BaseType, ClassType) static ClassFactory<BaseType, ClassType> BaseType ## ClassType ## Factory(#ClassType);

#endif /* FACTORY_H */
