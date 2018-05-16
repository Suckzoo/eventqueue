#include "event.hpp"
#include <queue>
#include <random>
using namespace std;

#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

const int INITIAL_HP = 3;
const int INITIAL_MP = 0;

class playerState {
    public:
        int id;
        int hp;
        int mp;
        ActionType lastAction;
        double lastTimestamp;

        playerState() {
            id = -1;
            hp = -1;
            mp = -1;

            lastAction = GATHER;
            lastTimestamp = -987987987;
        }

        playerState(int i, int h, int m) {
            id = i;
            hp = h;
            mp = m;

            lastAction = GATHER;
            lastTimestamp = -987987987;
        }
};

class Player : public Instance {
private:
    playerState myState;
    double reaction = 1500;
    double voteFrequency = 3000;
    double deviation = 50;
    random_device rd;
    mt19937_64 gen;
    normal_distribution<double> nd;
    priority_queue <ActionPacket, vector<ActionPacket>, greater<ActionPacket> > actionBuffer; 
    queue <VotePacket> voteBuffer;
    vector <playerState> globalState;

public:
    void initialize(int);
    void initializeGlobalState();
    void packetArrived(Event);
    void action();
    void vote();
    double getEventJittering();
};

#endif
