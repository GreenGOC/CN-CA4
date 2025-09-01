#include <QCoreApplication>
#include "network.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Network net;

    net.addSenderPC(2.0);
    net.addSenderPC(2.0);
    net.addReceiverPC();
    net.addRouter();

    PC* pc1 = net.getPC(1);
    PC* pc2 = net.getPC(2);
    PC* pc3 = net.getPC(3);
    Router* r1 = net.getRouter(1000);

    net.connect(pc1, r1);
    net.connect(pc2, r1);
    net.connect(r1, pc3);

    net.run(10.0);

    return a.exec();
}
