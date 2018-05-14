
#ifndef __PACKET_HPP__
#define __PACKET_HPP__
enum PacketType {
    ACTION_PACKET, VOTE_PACKET
};
enum ActionType {
    GATHER, BOLT, SHIELD
};
class Packet {
public:
    PacketType packetType;
    int sender;
    virtual ~Packet() {

    }
};

class ActionPacket : public Packet {
public:
    ActionPacket() {
        packetType = ACTION_PACKET;
    }
    ActionType action;
    int target;
    int targetHp;
    int myHp;
    int myMp;
    ~ActionPacket() {

    }
};

class VotePacket : public Packet {
public:
    VotePacket() {
        packetType = VOTE_PACKET;
    }
    ~VotePacket() {

    }
};
#endif