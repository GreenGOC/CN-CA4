#include "pc.h"
#include "link.h"

#include <iostream>

PC::PC(int id, double lambda): Node(id), packetCounter(0), interArrivalDist(lambda) {
    generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

Packet PC::generatePacket(double currentTime){
    this->packetCounter++;
    std::string text;
    //TODO: Get text from user
    int size = text.length();
    int id;
    //TODO: Get the user to enter the Id of the wanted Device
    return Packet(packetCounter, this->id, id, size, currentTime, text);
}

double PC::getNextPacketDelay(){
    return this->interArrivalDist(this->generator);
}

void PC::receivePacket(const Packet& p, Link* incomingLink){
    std::cout << "PC " << id << " received Packet " << p.id
              << " from Node " << incomingLink->getOtherEnd(this)->getId()
              << std::endl;
}

void PC::sendPacket(double currentTime) {
    if(!this->links.empty()){
        std::string text;
        Packet p = this->generatePacket(currentTime);

    } else{
        //TODO:  Display a message that there is no router
    }
}
