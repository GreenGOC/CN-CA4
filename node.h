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
public:
    Node(int id) : id(id){}
    virtual ~Node() {};

    void connectLink(Link* link);
    int getId() {return this->id;}
    virtual void sendPacket(double currentTime) = 0;
    virtual void receivePacket(const Packet& p, Link* incomingLink) = 0;
};

#endif // NODE_H
