#include "event.hpp"
#ifndef __INSTANCE_HPP__
#define __INSTANCE_HPP__
class Instance {
public:
    virtual void initialize(int _id) = 0;
    virtual void initializeGlobalState() = 0;
    virtual void packetArrived(Event) = 0;
    virtual void action(int actionType, int target) = 0;
    virtual void vote() = 0;
    void sendPacket(int, Packet*);
};
#endif
