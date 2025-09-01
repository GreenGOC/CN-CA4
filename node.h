#ifndef NODE_H
#define NODE_H

#include <vector>

#include "Packet.h"

class Link;

class Node
{
protected:
    int id;
    std::vector<Link*> links;
    std::vector<std::pair<int, int>> ARPTable;
public:
    Node(int id) : id(id){}
    virtual ~Node() {};

    void connectLink(Link* link);
    int getId() {return this->id;}
    virtual void sendPacket(double currentTime, int destId) = 0;
    virtual void receivePacket(const Packet& p, Link* incomingLink) = 0;
};

#endif // NODE_H
