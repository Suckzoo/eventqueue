#include "instance.hpp"
#include "player.hpp"
#include "event.hpp"
#include "eventqueue.hpp"
#include "packet.hpp"
#include <time.h>
#include <stdlib.h>
#include <algorithm>
#include <set>

using namespace std;

void Player::initialize(int _id) {
    srand(time(NULL));

    myState.id = _id;
    myState.hp = INITIAL_HP;
    myState.mp = INITIAL_MP;

    gen = mt19937_64(rd());
    nd = normal_distribution<double>(0, deviation);

    // Seed action
    /*
    Event initial_action;
    initial_action.timestamp = reaction + getEventJittering();
    initial_action.id = myState.id;
    initial_action.eventType = ACTION;
    initial_action.p = nullptr;
    EventQueue::getInstance()->pushEvent(initial_action);*/

    // Seed vote
    /*
    Event initial_vote;
    initial_vote.timestamp = voteFrequency; 
    initial_vote.id = myState.id;
    initial_vote.eventType = VOTE;
    initial_vote.p = nullptr;
    EventQueue::getInstance()->pushEvent(initial_vote);
    */
}

void Player::initializeGlobalState() {
    for(int i=0;i<EventQueue::numPlayers;i++) {
        globalState.push_back(playerState(i, INITIAL_HP, INITIAL_MP));
    }
}

void Player::packetArrived(Event e) {
    switch(e.p->packetType) {
        case ACTION_PACKET:
            // printf("ACTION PACKET ARRIVED %lf %d %d %d\n", e.timestamp, e.p->packetId, e.id, myState.id);
            actionBuffer.push(*(dynamic_cast <ActionPacket *> (e.p)));
            break;
        case VOTE_PACKET:
            voteBuffer.push(*(dynamic_cast <VotePacket *> (e.p)));
            break;
        case AGGREGATE_PACKET:
            aggregateBuffer.push(*(dynamic_cast <AggregatePacket*> (e.p)));
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

void Player::action(int actionType, int target) {
    int i;
    int t;
    ActionType a;

    int aliveCnt = 0;

    for(int i=0;i<globalState.size();i++) {
        if(globalState[i].hp > 0) 
            aliveCnt++;
    }

    if(aliveCnt <= 1){
        doneFlag = true;
        return;
    }

    if(globalState[myState.id].hp > 0) {
        actionSendingCount++;

        while(1) {
            if(actionType == 1) {
                a = BOLT;
                t = target;

                if(myState.mp > 0) {
                    myState.lastAction = BOLT;
                    myState.mp--;

                    if(EventQueue::truth[myState.id].hp > 0)
                        EventQueue::truth[myState.id].mp--;

                    if(EventQueue::truth[myState.id].hp <= 0 || (EventQueue::truth[t].lastAction == SHIELD && EventQueue::truth[t].lastTimestamp >= EventQueue::getInstance()->getTime() - 1000)) ;
                    else{
                        EventQueue::truth[t].hp--;
                    }

                    EventQueue::truth[myState.id].lastAction = BOLT;
                }
                break;
            }
            else if(actionType == 2) {
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
            else {
                a = SHIELD;
                t = myState.id;

                myState.lastAction = SHIELD;

                if(EventQueue::truth[myState.id].hp > 0) {
                    EventQueue::truth[myState.id].lastAction = SHIELD;
                }

                break;
            }
        }
        /*
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
        */

        myState.lastTimestamp = EventQueue::getInstance()->getTime();
        EventQueue::truth[myState.id].lastTimestamp = myState.lastTimestamp;

        printf("[%lf] ###### Player %d action : %d (%d --> %d)\n",myState.lastTimestamp, myState.id, myState.lastAction, myState.id, t);

        for(int i=0;i<EventQueue::truth.size();i++) {
            printf("(%d, %d, %d)    ",i, EventQueue::truth[i].hp, EventQueue::truth[i].mp);
        }
        printf("\n############### \n");

        int packetId = rand();
        int myProxy = rand() % EventQueue::numPlayers;

        EventQueue::actionOwner[packetId] = myState.id;

        printf("REGISTER %d %d\n",packetId, myState.id);

        ActionPacket *packet = new ActionPacket();

        packet->packetId = packetId;
        packet->action = a;
        packet->source = myState.id;
        packet->target = t;
        packet->timestamp = EventQueue::getInstance()->getTime();

        // printf("SEND PACKET %d %d %d %d %d\n",packetId, a, myState.id, t, myProxy);

        sendPacket(myProxy, static_cast<Packet*>(packet));

        /*
        Event e;
        e.timestamp = EventQueue::getInstance()->getTime() + reaction + getEventJittering();
        e.id = myState.id;
        e.eventType = ACTION;
        EventQueue::getInstance()->pushEvent(e);
        */
    }
}

bool Player::isValidPacket(ActionPacket ap) {
    if(EventQueue::malProxy == true && myState.id < EventQueue::malCriteria)
        return false;
    else{
        if(EventQueue::malUser == true && ap.source < EventQueue::malCriteria)
            return false;
        else
            return true;
    }
}

void Player::vote() {
    vector <ActionPacket> receivedPackets;
    vector <ActionPacket> validPackets;

    while(!aggregateBuffer.empty()) {
        AggregatePacket myTop = aggregateBuffer.top();

        int currentTurn = EventQueue::getInstance()->getTime() / voteFrequency;
        int aggregateTurn = myTop.timestamp / voteFrequency;

        if(aggregateTurn <= currentTurn-1) {
            for(int j=0;j<myTop.packets.size();j++) {
                receivedPackets.push_back(myTop.packets[j]);
            }

            aggregateBuffer.pop();
        }
        else break;
    }

    sort(receivedPackets.begin(), receivedPackets.end());

    for(int i=0;i<receivedPackets.size();i++) {
        int cnt = 0;

        ActionPacket& myTop = receivedPackets[i];

        // printf("%d: Processing --> (%d %d %d %d)\n", myState.id, receivedPackets[i].packetId, receivedPackets[i].action, receivedPackets[i].source, receivedPackets[i].target);

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
    }

    AggregatePacket myPackets;

    while(!actionBuffer.empty()) {
        ActionPacket myTop = actionBuffer.top();

//        printf("---- action packet %d: (%d %d %d %d)\n", myState.id, myTop.packetId, myTop.action, myTop.source, myTop.target);

        int currentTurn = EventQueue::getInstance()->getTime() / voteFrequency;
        int actionTurn = myTop.timestamp / voteFrequency;

        if(actionTurn <= currentTurn-2) {
            actionBuffer.pop();

            if(isValidPacket(myTop))
                myPackets.packets.push_back(myTop);
            else{
                EventQueue::rejectedCount[EventQueue::actionOwner[myTop.packetId]]++;

                printf("REJECTED!! %d %d %d\n",myTop.packetId, EventQueue::actionOwner[myTop.packetId], myState.id);
            }
        }
      
        else break;
    }

    int packetId = rand();

    for(int i=0;i<myPackets.packets.size();i++) printf("(%d %d %d %d) ", myPackets.packets[i].packetId, myPackets.packets[i].action, myPackets.packets[i].source, myPackets.packets[i].target);

    printf("\n");

    for(int i=0;i<EventQueue::numPlayers;i++) {
        AggregatePacket *packet = new AggregatePacket();

        packet->packetId = packetId;
        packet->source = myState.id;
        packet->packets = myPackets.packets;
        packet->timestamp = EventQueue::getInstance()->getTime();

        if (i == myState.id){ 
            aggregateBuffer.push(*packet);
        }
        else {
            sendPacket(i, static_cast<Packet*>(packet));
        }
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
        doneFlag = true;
        printf("Game done!! %d\n", myState.id);
    } 
    else {/*
        Event e;
        e.timestamp = EventQueue::getInstance()->getTime() + voteFrequency;
        e.id = myState.id;
        e.eventType = VOTE;
        EventQueue::getInstance()->pushEvent(e);
        */
    }
}

double Player::getEventJittering() {
    return nd(gen);
}
