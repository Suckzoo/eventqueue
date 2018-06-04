#ifndef __PACKET_HPP__
#define __PACKET_HPP__

#include <vector>

using namespace std;

enum PacketType {
    ACTION_PACKET, AGGREGATE_PACKET, VOTE_PACKET
};
enum ActionType {
    GATHER, BOLT, SHIELD
};
class Packet {
public:
    PacketType packetType;
    int packetId;
    int source;
    int target;
    virtual ~Packet() {

    }
};

class ActionPacket : public Packet {
public:
    ActionPacket() {
        packetType = ACTION_PACKET;
    }
    ActionType action;
    double timestamp;

    ~ActionPacket() {

    }

    bool operator > (const ActionPacket& t) const {
        return timestamp > t.timestamp;
    }
};

class VotePacket : public Packet {
public:
    VotePacket() {
        packetType = VOTE_PACKET;
    }
    double timestamp;
    ~VotePacket() {

    }
};

class AggregatePacket : public Packet {
public:
    AggregatePacket() {
        packetType = AGGREGATE_PACKET;
    }
    double timestamp;
    vector <ActionPacket> packets;
    ~AggregatePacket() {

    }

    bool operator > (const AggregatePacket& t) const {
        return timestamp > t.timestamp;
    }
};
#endif
