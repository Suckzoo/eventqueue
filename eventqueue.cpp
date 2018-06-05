#include "event.hpp"
#include "eventqueue.hpp"
#include "player.hpp"
#include <vector>
#include <map>
#include <unistd.h>

int EventQueue::numPlayers = 0;
int EventQueue::malCriteria = 0;
bool EventQueue::malProxy = false;
bool EventQueue::malUser = false;
int EventQueue::rejectedCount[100];

EventQueue* EventQueue::singleton = nullptr;
vector <playerState> EventQueue::truth;

map <int, int> EventQueue::actionOwner;


double EventQueue::getTime() {
    return time;
}

double EventQueue::getRTT() {
    double rtt = nd(gen);
    return rtt;
}

void EventQueue::registerInstance(Instance *instance) {
    EventQueue::numPlayers++;

    instance->initialize(instances.size());
    instances.push_back(instance);

    truth.push_back(playerState(truth.size(), INITIAL_HP, INITIAL_MP));
}

void EventQueue::run() {
    for(int i=0;i<EventQueue::numPlayers;i++) {
        instances[i]->initializeGlobalState();
    }

    Event event;
    double curTime;
    int actionVote;
    int actionType;
    int source;
    int target;

    scanf("%lf %d",&curTime, &actionVote);
    
    if(actionVote == 1) {
        scanf("%d",&actionType);

        if(actionType == 1)
            scanf("%d %d",&source, &target);
        else
            scanf("%d",&source);
    }
    else {
        scanf("%d",&source);
    }

    Event initial;
    initial.timestamp = 987987987; 
    initial.id = -1;
    initial.eventType = PACKET;
    initial.p = nullptr;
    EventQueue::getInstance()->pushEvent(initial);

    while(!que.empty()) {
        bool df = false;
        for(int i=0;i<EventQueue::numPlayers;i++) {
            Player *p = dynamic_cast <Player *>(instances[i]);

            if(p->doneFlag == false) {
                df = true;
                break;
            }
        }

        if(!df) break;

        event = que.top();
        int _target;
        int _actionType;

        // printf("%lf %lf\n",event.timestamp, curTime);
        if(event.timestamp <= curTime) {
            que.pop();
        }
        else {
            if(actionVote == 1) {
                event.timestamp = curTime;
                event.id = source;
                event.eventType = ACTION;
                _target = target;
                _actionType = actionType;
            }
            else {
                event.timestamp = curTime;
                event.id = source;
                event.eventType = VOTE;
            }

            scanf("%lf %d",&curTime, &actionVote);

            if(actionVote == 1) {
                scanf("%d",&actionType);

                if(actionType == 1)
                    scanf("%d %d",&source, &target);
                else {
                    scanf("%d",&source);
                    target = source;
                }
            }
            else {
                scanf("%d",&source);
                target = source;
            }
        }

        // printf("[EVENT] %lf %d %d\n", event.timestamp, event.id, event.eventType);

        time = event.timestamp;
        Instance *instance = instances[event.id];

        switch(event.eventType) {
            case PACKET:
                instance->packetArrived(event);
                break;
            case VOTE:
                instance->vote();
                break;
            case ACTION:
                instance->action(_actionType, _target);
                break;
        }

        /*
        event = que.top();
        que.pop();
        time = event.timestamp;
        pprint(event);
        Instance *instance = instances[event.id];
        switch(event.eventType) {
            case PACKET:
                instance->packetArrived(event);
                break;
            case VOTE:
                instance->vote();
                break;
            case ACTION:
                instance->action();
                break;
        }
        sleep(0.8);
        */
    }
}

void EventQueue::pushEvent(Event &e) {
    que.push(e);
}
