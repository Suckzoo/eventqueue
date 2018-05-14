#include <string>
#include "packet.hpp"
using namespace std;

#ifndef __EVENT_HPP__
#define __EVENT_HPP__

enum EventType {
    PACKET, VOTE, ACTION
};

class Event {
public:
    double timestamp;
    int id;
    EventType eventType;
    Packet *p;
    bool operator > (const Event &e) const {
        return timestamp > e.timestamp;
    }
};

void pprint(Event);
#endif