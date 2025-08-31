#ifndef PACKET_H
#define PACKET_H
#include <string>

struct Packet{
    int id;
    int sourceId;
    int destId;
    int size;
    double timestamp;
    std::string data;

    Packet(int id, int sourceId, int destId, int size, double timestamp, std::string data);
};

Packet::Packet(int id, int sourceId, int destId, int size, double timestamp, std::string data)
    : id(id), sourceId(sourceId), destId(destId), size(size), timestamp(timestamp), data(data){}


#endif // PACKET_H
