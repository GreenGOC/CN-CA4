#ifndef NETWORK_H
#define NETWORK_H

#include "pc.h"
#include "router.h"

class Network
{
private:
    std::vector<PC*> endNodes;
    std::vector<Router*> routers;
public:
    Network();
    void addSenderPC();
    void addReceiverPC();
    void sendPacket();
};

#endif // NETWORK_H
