#ifndef PC_H
#define PC_H

#include "node.h"

#include <chrono>
#include <random>

class PC : public Node
{
private:
    int packetCounter;
    std::default_random_engine generator;
    std::exponential_distribution<double> interArrivalDist;
    double congestionWaitTime;
public:
    PC(int id, double lambda);
    Packet generatePacket(double currentTime, int destId);
    double getNextPacketDelay();
    void receivePacket(const Packet& p, Link* incomingLink) override;
    void sendPacket(double currentTime, int destId) override;

};

#endif // PC_H
