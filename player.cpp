#include "instance.hpp"
#include "player.hpp"
#include "event.hpp"
#include "eventqueue.hpp"
#include "packet.hpp"
#include <time.h>
#include <stdlib.h>

void Player::initialize(int _id) {
    srand(time(NULL));

    myState.id = _id;
    myState.hp = INITIAL_HP;
    myState.mp = INITIAL_MP;

    gen = mt19937_64(rd());
    nd = normal_distribution<double>(0, deviation);

    // Seed action
    Event initial_action;
    initial_action.timestamp = reaction + getEventJittering();
    initial_action.id = myState.id;
    initial_action.eventType = ACTION;
    initial_action.p = nullptr;
    EventQueue::getInstance()->pushEvent(initial_action);

    // Seed vote
    Event initial_vote;
    initial_vote.timestamp = voteFrequency; /*+ getEventJittering();*/
    initial_vote.id = myState.id;
    initial_vote.eventType = VOTE;
    initial_vote.p = nullptr;
    EventQueue::getInstance()->pushEvent(initial_vote);
}

void Player::initializeGlobalState() {
    for(int i=0;i<EventQueue::numPlayers;i++) {
        globalState.push_back(playerState(i, INITIAL_HP, INITIAL_MP));
    }
}

void Player::packetArrived(Event e) {
    switch(e.p->packetType) {
        case ACTION_PACKET:
            actionBuffer.push(*(dynamic_cast <ActionPacket *> (e.p)));
            break;
        case VOTE_PACKET:
            voteBuffer.push(*(dynamic_cast <VotePacket *> (e.p)));
            break;
    }

    delete e.p;
}

int getRandomNumber(int n) {
    return rand() % n;
}

int getRandomNumberExceptFor(int n, int a) {
    while(1) {
        int temp = rand() % n;

        if(temp != a) return temp;
    }
}
void Player::action() {
    int i;
    int t;
    ActionType a;

    int aliveCnt = 0;

    for(int i=0;i<globalState.size();i++) {
        if(globalState[i].hp > 0) 
            aliveCnt++;
    }

    if(aliveCnt <= 1) return;

    if(globalState[myState.id].hp > 0) {
        while(1) {
            int operation = getRandomNumber(3);

            if(operation == GATHER) {
                a = GATHER;
                t = myState.id;

                myState.mp++;
                myState.lastAction = GATHER;

                if(EventQueue::truth[myState.id].hp > 0) {
                    EventQueue::truth[myState.id].mp++;
                    EventQueue::truth[myState.id].lastAction = GATHER;
                }

                break;
            }
            else if(operation == BOLT && myState.mp >= 1) {
                a = BOLT;
                t = getRandomNumberExceptFor(EventQueue::numPlayers, myState.id);

                myState.lastAction = BOLT;
                myState.mp--;


                if(EventQueue::truth[myState.id].hp > 0)
                    EventQueue::truth[myState.id].mp--;

                if(EventQueue::truth[myState.id].hp <= 0 || (EventQueue::truth[t].lastAction == SHIELD && EventQueue::truth[t].lastTimestamp >= EventQueue::getInstance()->getTime() - 1000)) ;
                else{
                    EventQueue::truth[t].hp--;
                }

                EventQueue::truth[myState.id].lastAction = BOLT;
                break;
            }
            else if(operation == SHIELD) {
                a = SHIELD;
                t = myState.id;

                myState.lastAction = SHIELD;

                if(EventQueue::truth[myState.id].hp > 0) {
                 EventQueue::truth[myState.id].lastAction = SHIELD;
                }

                break;
            }
        }

        myState.lastTimestamp = EventQueue::getInstance()->getTime();
        EventQueue::truth[myState.id].lastTimestamp = myState.lastTimestamp;

        printf("###### Player %d action : %d (%d --> %d)\n",myState.id, myState.lastAction, myState.id, t);

        for(int i=0;i<EventQueue::truth.size();i++) {
            printf("(%d, %d, %d)    ",i, EventQueue::truth[i].hp, EventQueue::truth[i].mp);
        }
        printf("\n############### \n");

        for(i=0;i<EventQueue::numPlayers;i++) {
            ActionPacket *packet = new ActionPacket();

            packet->action = a;
            packet->source = myState.id;
            packet->target = t;
            packet->timestamp = EventQueue::getInstance()->getTime();

            if (i == myState.id){ 
                actionBuffer.push(*packet);
            }
            else {
                sendPacket(i, static_cast<Packet*>(packet));
                //printf("action: %d\n",a);
            }

        }

        Event e;
        e.timestamp = EventQueue::getInstance()->getTime() + reaction + getEventJittering();
        e.id = myState.id;
        e.eventType = ACTION;
        EventQueue::getInstance()->pushEvent(e);
    }
}

void Player::vote() {
   // printf("player: %d       voteTurn: %lf,%lf\n", myState.id, EventQueue::getInstance()->getTime(),  voteFrequency);

    while(!actionBuffer.empty()) {
        ActionPacket myTop = actionBuffer.top();

        int currentTurn = EventQueue::getInstance()->getTime() / voteFrequency;
        int actionTurn = myTop.timestamp / voteFrequency;

        if(actionTurn <= currentTurn-2) {
            actionBuffer.pop();

            // printf("%lf: %d %d -> %d\n",myTop.timestamp, myTop.action, myTop.source, myTop.target);

            if(globalState[myTop.source].hp > 0) {
                if(myTop.action == GATHER) {
                    globalState[myTop.target].mp++;
                }
                else if(myTop.action == BOLT) {
                    if(globalState[myTop.target].lastAction == SHIELD && globalState[myTop.target].lastTimestamp >= myTop.timestamp - 1000) ;
                    else globalState[myTop.target].hp--;

                    globalState[myTop.source].mp--;
                }
                else if(myTop.action == SHIELD) {
                }

                globalState[myTop.target].lastAction = myTop.action;
                globalState[myTop.target].lastTimestamp = myTop.timestamp;
            }
/*
            for(int i=0;i<globalState.size();i++) {
                printf("(%d, %d, %d)   ",i, globalState[i].hp, globalState[i].mp);
            }

            printf("\n");*/
        }
        else break;
    }

    printf("voting done! %d ------------------------------------------- \n",myState.id);

    for(int i=0;i<EventQueue::numPlayers;i++) {
        printf("(%d, %d, %d)     ",i, globalState[i].hp, globalState[i].mp);
    }

    printf("\n");

    int aliveCnt = 0;

    for(int i=0;i<globalState.size();i++) {
        if(globalState[i].hp > 0) 
            aliveCnt++;
    }

    if(aliveCnt <= 1) {
        printf("Game done!! %d\n", myState.id);
    } 
    else {
        Event e;
        e.timestamp = EventQueue::getInstance()->getTime() + voteFrequency;
        e.id = myState.id;
        e.eventType = VOTE;
        EventQueue::getInstance()->pushEvent(e);
    }
}

double Player::getEventJittering() {
    return nd(gen);
}
