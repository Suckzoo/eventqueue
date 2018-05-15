#include "instance.hpp"
#include "player.hpp"
#include "event.hpp"
#include "eventqueue.hpp"
#include "packet.hpp"


void Player::initialize(int _id) {
    this->id = _id;
    gen = mt19937_64(rd());
    nd = normal_distribution<double>(0, deviation);

    // Seed action
    Event initial_action;
    initial_action.timestamp = reaction + getEventJittering();
    initial_action.id = id;
    initial_action.eventType = ACTION;
    initial_action.p = nullptr;
    EventQueue::getInstance()->pushEvent(initial_action);

    // Seed vote
    Event initial_vote;
    initial_vote.timestamp = voteFrequency + getEventJittering();
    initial_vote.id = id;
    initial_vote.eventType = VOTE;
    initial_vote.p = nullptr;
    EventQueue::getInstance()->pushEvent(initial_vote);
}

void Player::packetArrived(Event e) {
    switch(e.p->packetType) {
        case ACTION_PACKET:
            break;
        case VOTE_PACKET:
            break;
    }
    delete e.p;
}

void Player::action() {
    int i;
    for(i=0;i<EventQueue::numPlayers;i++) {
        if (i == id) continue;
        ActionPacket *packet = new ActionPacket();
        packet->action = GATHER;
        packet->sender = id;
        packet->myMp = mp + 1;
        sendPacket(i, static_cast<Packet*>(packet));
    }

    Event e;
    e.timestamp = EventQueue::getInstance()->getTime() + reaction + getEventJittering();
    e.id = id;
    e.eventType = ACTION;
    EventQueue::getInstance()->pushEvent(e);
}

void Player::vote() {
    Event e;
    e.timestamp = EventQueue::getInstance()->getTime() + voteFrequency;
    e.id = id;
    e.eventType = VOTE;
    EventQueue::getInstance()->pushEvent(e);
}

double Player::getEventJittering() {
    return nd(gen);
}
