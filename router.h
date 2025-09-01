#ifndef ROUTER_H
#define ROUTER_H

#include <queue>
#include <vector>

#include "node.h"
#include "Packet.h"

class Link;

class Router :  public Node
{
private:
    const static int MAX_BUFFER_SIZE = 6;
    const static int MIN_THRESHOLD = 2;
    const static int MAX_THRESHOLD = 4;
    std::queue<Packet> buffer;
    void setARPTable(int id, int linkIndex);
    bool runRED(); // Just to say drop it or not, that's why it's boolean
    void sendCongestionSignal();
public:
    Router(int id) :Node(id) {}
    double getNextSendTime();
    void receivePacket(const Packet& p, Link* source) override;
    void sendPacket(double currentTime, int destId = -1) override;  // Honestly, I didn't know how to handle this, because PC need destId, but router doesn't, so I set it to -1.
};

#endif // ROUTER_H
