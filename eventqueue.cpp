#include "eventqueue.hpp"

EventQueue* EventQueue::singleton = nullptr;

void EventQueue::registerInstance(Instance *instance) {
    instance->initialize();
    instances.push_back(instance);
}

void EventQueue::run() {
    Event event;
    while(!que.empty()) {
        event = que.top();
        que.pop();
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
    }
}