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
    std::queue<Packet> buffer;
    std::vector<Link*> links;
    std::vector<std::pair<int, int>> ARPTable;
    void setARPTable(int id, int linkIndex);
    bool runRED(); // Just to say drop it or not, that's why it's boolean
public:
    Router();
    void receivePacket(const Packet& p, Link* source) override;
    void sendPacket(double currentTime) override;
};

#endif // ROUTER_H
