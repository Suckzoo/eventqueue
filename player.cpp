#include "instance.hpp"
#include "player.hpp"
#include "event.hpp"
#include "eventqueue.hpp"


void Player::initialize(int _id) {
    this->id = _id;
    gen = mt19937_64(rd());
    nd = normal_distribution<double>(reaction, deviation);
    Event e;
    e.timestamp = getActionDelay();
    e.id = id;
    e.eventType = ACTION;
    EventQueue::getInstance()->pushEvent(e);
}

void Player::packetArrived(Event e) {

}

void Player::action() {
    Event e;
    e.timestamp = EventQueue::getInstance()->getTime() + Player::getActionDelay();
    e.id = id;
    e.eventType = ACTION;
    EventQueue::getInstance()->pushEvent(e);
}

void Player::vote() {

}

double Player::getActionDelay() {
    return nd(gen);
}
