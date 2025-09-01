#include "pc.h"
#include "link.h"

#include <iostream>

PC::PC(int id, double lambda): Node(id), packetCounter(0), interArrivalDist(lambda) {
    generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

Packet PC::generatePacket(double currentTime, int destId){
    this->packetCounter++;
    std::string text;
    int size = text.length();
    int id = destId;
    return Packet(packetCounter, this->id, id, size, currentTime, text);
}

double PC::getNextPacketDelay(){
    return this->interArrivalDist(this->generator);
}

void PC::receivePacket(const Packet& p, Link* incomingLink){
    std::cout << "PC " << id << " has received Packet " << p.id
              << " from Node " << incomingLink->getOtherEnd(this)->getId()
              << std::endl;
}

void PC::sendPacket(double currentTime, int destId) {
    if(!this->links.empty()){
        Packet p = this->generatePacket(currentTime, destId);
        links[0]->send(p, this); // TODO: This is hardcoded as well.
        std::cout << "PC " << this->id << " has sent Packet " << p.id << " to " << p.destId << std::endl;
    } else{
        std::cout << "PC " << this->id << " is isolated! " << std::endl;
    }
}
