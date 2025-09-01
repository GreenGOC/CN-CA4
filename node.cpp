#include "node.h"
#include "link.h"


void Node::connectLink(Link* link){
    links.push_back(link);
    ARPTable.push_back({link->getOtherEnd(this)->getId(), links.size()-1});
}
