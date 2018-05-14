#include "event.hpp"
#include <random>
using namespace std;

#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__


class Player : public Instance {
private:
    int id;
    int hp = 3;
    int mp = 0;
    double reaction = 1600;
    double voteFrequency = 5000;
    double deviation = 50;
    random_device rd;
    mt19937_64 gen;
    normal_distribution<double> nd;
public:
    void initialize(int);
    void packetArrived(Event);
    void action();
    void vote();
    double getEventJittering();
};

#endif