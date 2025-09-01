#include "network.h"
#include "Link.h"
#include <QThread>
#include <thread>

Network::Network() {}

Network::~Network() {
    for (auto pc : endNodes) delete pc;
    for (auto r : routers) delete r;
    for (auto l : links) delete l;
}

void Network::addSenderPC(double lambda) {
    int id = endNodes.size() + 1;
    PC* pc = new PC(id, lambda);
    endNodes.push_back(pc);
}

void Network::addReceiverPC() {
    int id = endNodes.size() + 1;
    PC* pc = new PC(id, 0.0);
    endNodes.push_back(pc);
}

void Network::addRouter() {
    int id = routers.size() + 1000; // router IDs offset
    Router* r = new Router(id);
    routers.push_back(r);
}

void Network::connect(Node* a, Node* b) {
    Link* l = new Link(a, b);
    a->connectLink(l);
    b->connectLink(l);
    links.push_back(l);
}

void Network::run(double endTime) {
    double slowDown = 1000.0;
    double currentTime = 0.0;
    bool routerWait = false;
    while (currentTime < endTime) {
        double nextEventTime = endTime;
        PC* nextPC = nullptr;
        Router* nextRouter = nullptr;

        for (auto pc : this->endNodes) {
            if (pc->getNextSendTime() < nextEventTime) {
                nextEventTime = pc->getNextSendTime();
                nextPC = pc;
            }
        }
        for (auto router : this->routers){
            if(router->getNextSendTime() < nextEventTime){
                nextEventTime = router->getNextSendTime();
                nextRouter = router;
            }
        }

        if (!nextPC && !nextRouter) break;
        QThread::msleep((nextEventTime - currentTime) * slowDown);
        currentTime = nextEventTime;

        if(nextRouter){
            nextRouter->sendPacket(currentTime);
        }else if(nextPC){
            nextPC->sendPacket(currentTime, 3); // HARDCODED FELLAS
        }
    }
}

PC* Network::getPC(int id){
    for(auto& pc : this->endNodes){
        if(pc->getId() == id){
            return pc;
        }
    }
    return nullptr;
}

Router* Network::getRouter(int id){
    for(auto& router : this->routers){
        if(router->getId() == id){
            return router;
        }
    }
    return nullptr;
}
