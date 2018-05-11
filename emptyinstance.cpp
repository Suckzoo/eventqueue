#include "instance.hpp"
#include "emptyinstance.hpp"
#include "event.hpp"
#include "eventqueue.hpp"


void EmptyInstance::initialize(int _id) {
    this->id = _id;
    Event e;
    e.timestamp = 100;
    e.id = id;
    e.eventType = ACTION;
    EventQueue::getInstance()->pushEvent(e);
}

void EmptyInstance::packetArrived(Event e) {

}

void EmptyInstance::action() {
    printf("action called!\n");
    Event e;
    e.timestamp = EventQueue::getInstance()->getTime() + 100;
    e.id = id;
    e.eventType = ACTION;
    EventQueue::getInstance()->pushEvent(e);
}

void EmptyInstance::vote() {

}
