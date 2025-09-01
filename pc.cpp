#include "pc.h"
#include "link.h"

#include <iostream>
#include <chrono>

PC::PC(int id, double lambda): Node(id), packetCounter(0), interArrivalDist(lambda), congestionWaitTime(0.0) {
    generator.seed(std::chrono::system_clock::now().time_since_epoch().count() + this->id);
    nextSendTime = getNextPacketDelay();
}

Packet PC::generatePacket(double currentTime, int destId){
    this->packetCounter++;
    std::string text = "THIS IS FROM PC " + std::to_string(this->id) + ". OVER\n";
    int size = text.length();
    int id = destId;
    return Packet(packetCounter, this->id, id, size, currentTime, text);
}

double PC::getNextPacketDelay(){
    return this->interArrivalDist(this->generator);
}

double PC::getNextSendTime(){
    return this->nextSendTime;
}

void PC::receivePacket(const Packet& p, Link* incomingLink){
    if (p.destId == -1) {
        // We received a congestion signal from the router
        // Set a random wait time. The problem asks for a random time.
        double randomDelay = 1.0 + static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (1.0)));
        this->congestionWaitTime = randomDelay;
        std::cout << "PC " << id << " received a congestion signal! Waiting for " << congestionWaitTime << " units." << std::endl;
    } else {
        std::cout << "PC " << id << " has received Packet " << p.id
                  << " from Node " << incomingLink->getOtherEnd(this)->getId()
                  << std::endl;
    }
}

void PC::sendPacket(double currentTime, int destId) {
    std::cout << "Current Time: " << currentTime << ":\n";

    if(!this->links.empty()){
        Packet p = this->generatePacket(currentTime, destId);
        std::cout << "PC " << this->id << " has sent Packet " << p.id << " to " << p.destId << std::endl;
        this->scheduleNext(currentTime);

        links[0]->send(p, this);
    } else{
        std::cout << "PC " << this->id << " is isolated! " << std::endl;
    }
}

void PC::scheduleNext(double currentTime) {
    double baseDelay = getNextPacketDelay();

    if (congestionWaitTime > 0.0) {
        nextSendTime = currentTime + baseDelay + congestionWaitTime;
        std::cout << "PC " << id << " applied congestion wait of "
                  << congestionWaitTime << ", nextSendTime = "
                  << nextSendTime << std::endl;
        congestionWaitTime = 0.0;
    } else {
        nextSendTime = currentTime + baseDelay;
    }
}
