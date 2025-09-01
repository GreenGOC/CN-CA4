#ifndef NETWORK_H
#define NETWORK_H

#include "pc.h"
#include "router.h"

class Link;

class Network
{
private:
    std::vector<Link*> links;
    std::vector<PC*> endNodes;
    std::vector<Router*> routers;
public:
    Network();
    ~Network();
    void addSenderPC(double lambda);
    void addReceiverPC();
    void addRouter();
    void run(double currentTime);
    void connect(Node* a, Node* b);
    PC* getPC(int id);
    Router* getRouter(int id);


};

#endif // NETWORK_H
