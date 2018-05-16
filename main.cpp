#include "eventqueue.hpp"
#include "instance.hpp"
#include "player.hpp"

int main() {
    Player* instance1 = new Player();
    EventQueue::getInstance()->registerInstance(static_cast<Instance*>(instance1));
    Player* instance2 = new Player();
    EventQueue::getInstance()->registerInstance(static_cast<Instance*>(instance2));
    EventQueue::getInstance()->run();
    return 0;
}
