#include "eventqueue.hpp"
#include <unistd.h>

EventQueue* EventQueue::singleton = nullptr;

double EventQueue::getTime() {
    return time;
}

double EventQueue::getRTT() {
    double rtt = nd(gen);
    return rtt;
}

void EventQueue::registerInstance(Instance *instance) {
    instance->initialize(instances.size());
    instances.push_back(instance);
}

void EventQueue::run() {
    Event event;
    while(!que.empty()) {
        event = que.top();
        que.pop();
        time = event.timestamp;
        Instance *instance = instances[event.id];
        switch(event.eventType) {
            case PACKET:
                instance->packetArrived(event);
                break;
            case VOTE:
                instance->vote();
                break;
            case ACTION:
                instance->action();
                break;
        }
        sleep(3);
    }
}

void EventQueue::pushEvent(Event &e) {
    que.push(e);
}