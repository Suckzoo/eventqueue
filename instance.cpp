#include "event.hpp"
#include "eventqueue.hpp"
#include "instance.hpp"
#include "packet.hpp"

void Instance::sendPacket(int id, Packet *p) {
    printf("[%.2lf] Sending packets... %d -> %d\n",EventQueue::getInstance()->getTime(), p->sender, id);
    Event e;
    e.timestamp = EventQueue::getInstance()->getTime() + EventQueue::getInstance()->getRTT();
    e.eventType = PACKET;
    e.id = id;
    e.p = p;
    EventQueue::getInstance()->pushEvent(e);
}