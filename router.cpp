#include "router.h"
#include "Link.h"
#include "pc.h"
#include <iostream>
#include <cstdlib>   // rand()

bool Router::runRED() {
    int qlen = buffer.size();

    if (qlen < MIN_THRESHOLD) return false;
    if (qlen >= MAX_THRESHOLD) return true;
    double dropProb = 0.3 * this->buffer.size() - 0.4;
    double rnd = (double) rand() / RAND_MAX; // random [0,1]
    return rnd < dropProb;
}

void Router::receivePacket(const Packet& p, Link* source) {
    if (runRED()) {
        std::cout << "[Router] RED dropped Packet " << p.id << " From " << p.sourceId << std::endl;
        
        // Check for congestion signal
        if (buffer.size() >= MAX_THRESHOLD) {
            sendCongestionSignal();
        }
    } else {
        buffer.push(p);
        std::cout << "[Router] Enqueued Packet " << p.id << " From " << p.sourceId << " to " << p.destId
                  << " (queue size=" << buffer.size() << ")" << std::endl;
    }
}

void Router::sendPacket(double currentTime, int destId) {
    std::cout << "Current Time: " << currentTime << ":\n";

    if (buffer.empty()) return;

    Packet p = buffer.front();
    buffer.pop();

    if (!links.empty()) {
        int linkIndex = -1;
        for(int i = 0; i < this->ARPTable.size(); i++){
            if(this->ARPTable[i].first == p.destId){
                linkIndex = i;
                break;
            }
        }
        if(linkIndex == -1){
            std::cerr << "[Router] No ARP entry for dest " << p.destId << std::endl;
            return;
        }
        std::cout << "[Router] Forwarded Packet " << p.id << " From router to " << p.destId
                  << " at time " << currentTime << std::endl;


        links[linkIndex]->send(p, this);
    }
}

void Router::setARPTable(int id, int linkIndex) {
    if (linkIndex >= 0 && linkIndex < (int)links.size()) {
        ARPTable.push_back({id, linkIndex});
    }
}

void Router::sendCongestionSignal() {
    // Create a special congestion signal packet
    // We'll use destId = -1 to mark it as a signal
    Packet signalPacket(0, this->id, -1, 0, 0, "CONGESTION SIGNAL");
    
    // Loop through all connected links and send the signal to source PCs (1 and 2)
    // You have PC1 (ID 1) and PC2 (ID 2) connected to the router
    // This part assumes you know the IDs of the sender PCs
    for (auto& link : this->links) {
        if (link->getOtherEnd(this)->getId() == 1 || link->getOtherEnd(this)->getId() == 2) {
            link->send(signalPacket, this);
        }
    }
    std::cout << "[Router] Sent a congestion signal to the sender PCs." << std::endl;
}
