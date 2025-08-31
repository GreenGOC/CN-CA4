#include "link.h"
#include "node.h"

void Link::send(const Packet& p, Node* source){
    if(source == A){
        this->B->receivePacket(p, this);
    }else if(source == B){
        this->A->receivePacket(p, this);
    }
}

Node* Link::getOtherEnd(Node* source){
    return (this->A == source ? this->B : this->A);
}
