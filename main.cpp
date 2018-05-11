#include "eventqueue.hpp"
#include "instance.hpp"
#include "emptyinstance.hpp"

int main() {
    EmptyInstance* instance = new EmptyInstance();
    EventQueue::getInstance()->registerInstance(static_cast<Instance*>(instance));
    EventQueue::getInstance()->run();
    return 0;
}