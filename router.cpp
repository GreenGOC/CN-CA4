#include "router.h"
#include "Link.h"
#include "pc.h"
#include <iostream>
#include <cstdlib>   // rand()

bool Router::runRED() {
    int qlen = buffer.size();
    if (qlen < MIN_THRESHOLD) return false;
    if (qlen >= MAX_THRESHOLD) return true;

    double dropProb = 0.3 * qlen - 0.4;
    double rnd = static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (1.0)));
    return rnd < dropProb;
}

double Router::getNextSendTime(){
    return this->nextSendTime;
}

void Router::receivePacket(const Packet& p, Link* source) {
    if (runRED()) {
        std::cout << " [Router] RED dropped Packet " << p.id << " From " << p.sourceId << std::endl;
        sendCongestionSignal();

    } else {
        buffer.push(p);
        std::cout << "[Router] Enqueued Packet " << p.id << " From " << p.sourceId << " to " << p.destId
                  << " (queue size=" << buffer.size() << ")" << std::endl;
    }
}

void Router::sendPacket(double currentTime, int destId) {
    std::cout << " [Router] Current Time: " << currentTime << ":\n";

    if (buffer.empty()) {
        this->nextSendTime += 0.01;
        return;
    }
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
            std::cerr << " [Router] No ARP entry for dest " << p.destId << std::endl;
            return;
        }
        std::cout << " [Router] Forwarded Packet " << p.id << " From router to " << p.destId
                  << " at time " << currentTime << std::endl;

        this->nextSendTime += 1.0;
        links[linkIndex]->send(p, this);
    }
}

void Router::setARPTable(int id, int linkIndex) {
    if (linkIndex >= 0 && linkIndex < (int)links.size()) {
        ARPTable.push_back({id, linkIndex});
    }
}

void Router::sendCongestionSignal() {
    Packet signalPacket(0, this->id, -1, 0, 0, "CONGESTION SIGNAL");

    for (auto& link : this->links) {
        if (link->getOtherEnd(this)->getId() == 1 || link->getOtherEnd(this)->getId() == 2) {
            link->send(signalPacket, this);
        }
    }
    std::cout << " [Router] Sent a congestion signal to the sender PCs." << std::endl;
}
