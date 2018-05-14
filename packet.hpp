
#ifndef __PACKET_HPP__
#define __PACKET_HPP__
enum PacketType {
    ACTION, VOTE
};
class Packet {
public:
    PacketType packetType;
    virtual ~Packet() {

    }
};

class ActionPacket : public Packet {
public:
    PacketType packetType = ACTION;
    int action;
    int target;
    int targetHp;
    int myHp;
    int myMp;
    ~ActionPacket() {

    }
};

class VotePacket : public Packet {
public:
    PacketType packetType = VOTE;
    ~VotePacket() {

    }
};
#endif