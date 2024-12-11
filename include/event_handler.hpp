#pragma once

#include "container.hpp"
#include "network.hpp"

enum class EventType
{
    INVOKE,
    NOTHING,
    ERROR
};

class IEventHandler
{
public:
    virtual void handle_event(EventType event) = 0;
    virtual ~IEventHandler() {}
};

class EventHandler : public IEventHandler
{
private:
    Container &container;
    Network &network;
    uint64_t time_window;

    void event_job();

public:
    EventHandler(Container &rc, Network &net, uint64_t time_window)
        : container(rc), network(net), time_window(time_window) {}

    void handle_event(EventType event);
};