#include "eventqueue.hpp"
#include "instance.hpp"
#include "player.hpp"

int main() {
    int n, m;

    Player* instance[100];

    scanf("%d %d",&n, &m);

    EventQueue::malCriteria = m;
    EventQueue::malProxy = true;
    EventQueue::malUser = false;

    for(int i=0;i<n;i++) {
        instance[i] = new Player();
        EventQueue::getInstance()->registerInstance(static_cast<Instance*>(instance[i]));
    }

    for(int i=0;i<n;i++) EventQueue::rejectedCount[i] = 0;

    for(int i=0;i<m;i++) {
        instance[i]->maluser = true;
    }

    EventQueue::getInstance()->run();

    /*
    int statistics[5] = {0,};

    for(int i=0;i<n;i++) {
        statistics[0] += instance[i]->packetSendingCount;
        statistics[1] += instance[i]->packetSuccessfulCount;
        statistics[2] += instance[i]->packetRejectedCount;
        statistics[3] += instance[i]->packetReceivingCount;
        statistics[4] += instance[i]->invalidPacketReceivingCount;
    }

    printf("--- overall statistics --- \n");


    for(int i=0;i<5;i++) printf("%d ",statistics[i]);

    printf("\n\n");

    printf("--- honest user statistics --- \n");

    for(int i=0;i<5;i++) statistics[i] = 0;

    for(int i=EventQueue::malCriteria;i<n;i++) {
        statistics[0] += instance[i]->packetSendingCount;
        statistics[1] += instance[i]->packetSuccessfulCount;
        statistics[2] += instance[i]->packetRejectedCount;
        statistics[3] += instance[i]->packetReceivingCount;
        statistics[4] += instance[i]->invalidPacketReceivingCount;
    }

    for(int i=0;i<5;i++) printf("%d ",statistics[i]);

    printf("\n\n");
 */   
    int sum1 = 0;
    int sum2 = 0;

    for(int i=0;i<n;i++){
        printf("%d ",instance[i]->actionSendingCount);
        sum1 += instance[i]->actionSendingCount;
    }

    printf("\n\n");


    for(int i=0;i<n;i++){
        printf("%d ",EventQueue::rejectedCount[i]);
        sum2 += EventQueue::rejectedCount[i];
    }

    printf("\n");

    printf("%d\t%d\t%lf\n",sum1, sum2, 100.0 - (double)sum2 / sum1 * 100);

    return 0;
}
