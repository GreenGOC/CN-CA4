#ifndef LINK_H
#define LINK_H

#include "Packet.h"

class Node;

/*
 * This object is for connecting two nodes, not multiple nodes. BE CAREFUL!!!!!!!!!!!!
 */

class Link
{
private:
    Node* A;
    Node* B;
public:
    Link(Node* A, Node* B) : A(A), B(B) {}

    Node* getOtherEnd(Node* current);
    void send(const Packet& p, Node* source);
};

#endif // LINK_H
