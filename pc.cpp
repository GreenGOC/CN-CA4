#include "pc.h"
#include "link.h"

#include <iostream>

PC::PC(int id, double lambda): Node(id), packetCounter(0), interArrivalDist(lambda), congestionWaitTime(0.0) {
    generator.seed(std::chrono::system_clock::now().time_since_epoch().count() + this->id);
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

void PC::receivePacket(const Packet& p, Link* incomingLink){
    // Check if the packet is a congestion signal
    if (p.destId == -1) {
        // We received a congestion signal from the router
        // Set a random wait time. The problem asks for a random time.
        // Let's use a random value between 0.5 and 2.0.
        // The time unit can be set by you. Here, let's assume it's seconds or similar.
        double randomDelay = 0.5 + static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (1.5)));
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

    // If there is a congestion wait time, decrease it and don't send
    if (this->congestionWaitTime > 0.0) {
        std::cout << "PC " << this->id << " is in congestion wait mode. " << congestionWaitTime << " units remaining." << std::endl;
        this->congestionWaitTime -= 0.5; // Assuming time steps are 0.5 as in main.cpp
        if (this->congestionWaitTime < 0.0) {
            this->congestionWaitTime = 0.0;
        }
        return;
    }

    if(!this->links.empty()){
        Packet p = this->generatePacket(currentTime, destId);
        std::cout << "PC " << this->id << " has sent Packet " << p.id << " to " << p.destId << std::endl;

        links[0]->send(p, this);
    } else{
        std::cout << "PC " << this->id << " is isolated! " << std::endl;
    }
}
