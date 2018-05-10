#include "event.hpp"
#ifndef __INSTANCE_HPP__
#define __INSTANCE_HPP__
class Instance {
public:
    virtual void initialize() = 0;
    virtual void packetArrived(Event) = 0;
    virtual void action() = 0;
    virtual void vote() = 0;
};
#endif