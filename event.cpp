#include "event.hpp"
#include "eventqueue.hpp"


void pprint(Event e) {
    printf("[%.2lf] ",EventQueue::getInstance()->getTime());
    switch(e.eventType) {
        case PACKET:
            printf("Packet arrived to %d\n",e.id);
            break;
        case VOTE:
            printf("%d is voting\n",e.id);
            break;
        case ACTION:
            printf("%d is taking action\n",e.id);
            break;
    };
}