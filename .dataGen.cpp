#include "event.hpp"
#include <cstdio>
#include <queue>
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;

const int MAX = 100;

mt19937_64 gen;
normal_distribution <double> nd;
priority_queue <Event, vector <Event>, greater<Event> > Queue;
double reaction = 1500;
double voteFrequency = 3000;
double deviation = 50;

int hp[MAX], mp[MAX], lastAction[MAX];
double lastTimestamp[MAX];

double getEventJittering() {
    return nd(gen);
}

int main() {
    int n;

    scanf("%d",&n);

    printf("%d\n",n);

    for(int i=0;i<n;i++) {
        hp[i] = 3;
        mp[i] = 0;
        lastAction[i] = -1;
        lastTimestamp[i] = 0;
    }

    for(int i=0;i<n;i++) {
        Event initial_action;
        initial_action.timestamp = reaction + getEventJittering();
        initial_action.id = i;
        initial_action.eventType = ACTION;
        initial_action.p = nullptr;
        Queue.push(initial_action);

        // Seed vote
        Event initial_vote;
        initial_vote.timestamp = voteFrequency; /*+ getEventJittering() */
        initial_vote.id = i;
        initial_vote.eventType = VOTE;
        initial_vote.p = nullptr;
        Queue.push(initial_vote);
    }

    double curTime = 0;
    int cnt = 0;

    srand(time(NULL));

    while(!Queue.empty()) {
        if(cnt >= 10000) break;

        cnt++;

        Event myTop = Queue.top();
        Queue.pop();

        curTime = myTop.timestamp;

        if(myTop.eventType == ACTION) {
            while(1) {
                int t = rand() % 3;

                if(t == 0) { // ATTACK
                    int target = -1;

                    while(1) {
                        target = rand() % n;

                        if(target != myTop.id)
                            break;
                    }

                    if(mp[myTop.id] > 0){
                        printf("%lf %d %d %d %d\n",myTop.timestamp, 1, 1, myTop.id, target);

                        if(hp[myTop.id] <= 0 || (lastTimestamp[target] >= curTime - 1000 && lastAction[target] == 2)) ;
                        else hp[target]--;

                        mp[myTop.id]--;

                        lastAction[myTop.id] = 0;
                        lastTimestamp[myTop.id] = curTime;

                        break;
                    }
                }
                else if(t == 1) { // GATHER
                    printf("%lf %d %d %d\n",myTop.timestamp, 1, 2, myTop.id);

                    mp[myTop.id]++;

                    lastAction[myTop.id] = 1;
                    lastTimestamp[myTop.id] = curTime;

                    break;
                }
                else { // SHIELD
                    printf("%lf %d %d %d\n",myTop.timestamp, 1, 3, myTop.id);

                    lastAction[myTop.id] = 2;
                    lastTimestamp[myTop.id] = curTime;

                    break;
                }
            }
            /*
            printf("\n");

            for(int i=0;i<n;i++) {
                printf("(%d, %d, %d)   ",i, hp[i], mp[i]);
            }

            printf("\n");*/
        }
        else {
            printf("%lf %d %d\n",myTop.timestamp, 2, myTop.id);
        }

        if(myTop.eventType == ACTION) {
            Event e;
            e.timestamp = curTime + reaction + getEventJittering();
            e.id = myTop.id;
            e.eventType = ACTION;
            Queue.push(e);
        }
        else {
            Event e;
            e.timestamp = curTime + voteFrequency;
            e.id = myTop.id;
            e.eventType = VOTE;
            Queue.push(e);
        }
    }

    return 0;
}
