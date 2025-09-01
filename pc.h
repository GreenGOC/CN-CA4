#ifndef PC_H
#define PC_H

#include "node.h"

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
    double getNextSendTime();
    void receivePacket(const Packet& p, Link* incomingLink) override;
    void sendPacket(double currentTime, int destId) override;
    void scheduleNext(double currentTime);
};

#endif // PC_H
