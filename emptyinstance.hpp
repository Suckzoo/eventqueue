#include "event.hpp"

#ifndef __EMPTY_INSTANCE_HPP__
#define __EMPTY_INSTANCE_HPP__


class EmptyInstance : public Instance {
private:
    int id;
public:
    void initialize(int);
    void packetArrived(Event);
    void action();
    void vote();
};

#endif