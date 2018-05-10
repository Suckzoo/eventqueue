#include <string>
using namespace std;

#ifndef __EVENT_HPP__
#define __EVENT_HPP__

enum EventType {
    PACKET, VOTE, ACTION
};

class Event {
public:
    long long timestamp;
    int id;
    EventType eventType;
    bool operator > (const Event &e) const {
        return timestamp > e.timestamp;
    }
};
#endif