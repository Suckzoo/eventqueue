#include "eventqueue.hpp"
#include "instance.hpp"
#include "player.hpp"

int main() {
    Player* instance = new Player();
    EventQueue::getInstance()->registerInstance(static_cast<Instance*>(instance));
    EventQueue::getInstance()->run();
    return 0;
}