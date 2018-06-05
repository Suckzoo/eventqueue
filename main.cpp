#include "eventqueue.hpp"
#include "instance.hpp"
#include "player.hpp"

int main() {
    int n;

    scanf("%d",&n);

    for(int i=0;i<n;i++) {
        Player* instance = new Player();
        EventQueue::getInstance()->registerInstance(static_cast<Instance*>(instance));
    }

    /*
    Player* instance2 = new Player();
    EventQueue::getInstance()->registerInstance(static_cast<Instance*>(instance2));
    Player* instance3 = new Player();
    EventQueue::getInstance()->registerInstance(static_cast<Instance*>(instance3));*/

    /*
    Player* instance4 = new Player();
    EventQueue::getInstance()->registerInstance(static_cast<Instance*>(instance4));*/

    EventQueue::getInstance()->run();
    return 0;
}
