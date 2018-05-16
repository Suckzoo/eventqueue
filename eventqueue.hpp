#include <queue>
#include <functional>
#include <vector>
#include <random>
#include "event.hpp"
#include "instance.hpp"
#include "player.hpp"

using namespace std;

#ifndef __EVENTQUEUE_HPP__
#define __EVENTQUEUE_HPP__

class EventQueue {
private:
    priority_queue<Event, vector<Event>, greater<Event>> que;
    vector <Instance*> instances;
    static EventQueue *singleton;
    random_device rd;
    mt19937_64 gen;
    normal_distribution<double> nd;

    double time;
    EventQueue() {
        time = 0;
        gen = mt19937_64(rd());
        nd = normal_distribution<double>(2500, 50);
    }

public:
    static vector <playerState> truth;

    static const int numPlayers = 2;
    static EventQueue *getInstance() {
        if (singleton == nullptr) {
            singleton = new EventQueue();
        }
        return singleton;
    }

    double getTime();
    double getRTT();

    void registerInstance(Instance *instance);
    void run();
    void pushEvent(Event &e);
};

#endif
