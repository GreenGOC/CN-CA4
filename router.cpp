#include "router.h"
#include "Link.h"
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
    } else {
        buffer.push(p);
        std::cout << "[Router] Enqueued Packet " << p.id << " From " << p.sourceId << " to " << p.destId
                  << " (queue size=" << buffer.size() << ")" << std::endl;
    }
}

void Router::sendPacket(double currentTime, int destId) {
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
        links[linkIndex]->send(p, this);
        std::cout << "[Router] Forwarded Packet " << p.id << " From router to " << p.destId
                  << " at time " << currentTime << std::endl;
    }
}

void Router::setARPTable(int id, int linkIndex) {
    if (linkIndex >= 0 && linkIndex < (int)links.size()) {
        ARPTable.push_back({id, linkIndex});
    }
}
