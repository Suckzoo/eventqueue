#include "event.hpp"
#include "eventqueue.hpp"
#include "player.hpp"
#include <vector>
#include <unistd.h>

int EventQueue::numPlayers = 0;
EventQueue* EventQueue::singleton = nullptr;
vector <playerState> EventQueue::truth;

double EventQueue::getTime() {
    return time;
}

double EventQueue::getRTT() {
    double rtt = nd(gen);
    return rtt;
}

void EventQueue::registerInstance(Instance *instance) {
    EventQueue::numPlayers++;

    instance->initialize(instances.size());
    instances.push_back(instance);

    truth.push_back(playerState(truth.size(), INITIAL_HP, INITIAL_MP));
}

void EventQueue::run() {
    for(int i=0;i<EventQueue::numPlayers;i++) {
        instances[i]->initializeGlobalState();
    }

    Event event;
    while(!que.empty()) {
        event = que.top();
        que.pop();
        time = event.timestamp;
        pprint(event);
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
        sleep(0.8);
    }
}

void EventQueue::pushEvent(Event &e) {
    que.push(e);
}
