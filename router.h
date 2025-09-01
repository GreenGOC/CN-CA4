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
    bool runRED();
    void sendCongestionSignal(); // New method declaration
public:
    Router(int id) :Node(id) {}
    void receivePacket(const Packet& p, Link* source) override;
    void sendPacket(double currentTime, int destId = -1) override;
};

#endif // ROUTER_H