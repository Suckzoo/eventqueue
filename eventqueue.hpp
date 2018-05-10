#include <queue>
#include <functional>
#include <vector>
#include "event.hpp"
#include "instance.hpp"
using namespace std;

#ifndef __EVENTQUEUE_HPP__
#define __EVENTQUEUE_HPP__

class EventQueue {
private:
    priority_queue<Event, vector<Event>, greater<Event>> que;
    vector <Instance*> instances;
    static EventQueue *singleton;
    long long time;
    EventQueue() {

    }
public:
    static EventQueue *getInstance() {
        if (singleton == nullptr) {
            singleton = new EventQueue();
        }
        return singleton;
    }

    void registerInstance(Instance *instance);
    void run();
};

#endif