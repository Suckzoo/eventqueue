#include "event.hpp"
#include "eventqueue.hpp"
#include "instance.hpp"

// void Instance::sendPacket(int id, Packet *p) {
//     Event e;
//     e.timestamp = EventQueue::getInstance()->getTime() + EventQueue::getInstance()->getRTT();
//     e.eventType = PACKET;
//     e.id = id;
//     // e.packet = p
//     EventQueue::getInstance()->pushEvent(e);
// }