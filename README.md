# RED Algorithm Simulation

## Project Overview
This project simulates the Random Early Detection (RED) algorithm to manage network congestion, featuring a router, two sender PCs (PC1, PC2), and one receiver PC (PC3). Developed for the Computer Networks course (CN_CA_4) using Qt Framework and C++, it implements early packet dropping based on buffer length.

- **Course**: Computer Networks
- **Instructor**: Dr. Naser Yazdani
- **Semester**: Second Semester, Academic Year 1403-1404
- **GitHub Repository**: [GreenGOC/CN-CA4](https://github.com/GreenGOC/CN-CA4/tree/master)

## Objectives
- Simulate a network with a RED router and three PCs.
- Implement RED with a 6-unit buffer and probabilistic packet dropping.
- Send congestion signals when the buffer exceeds the threshold.
- Use a store-and-forward mechanism.

## Implementation Details
- **Framework**: Qt Framework with C++
- **Topology**: Two sender PCs (PC1, PC2) and one receiver PC (PC3) connected via a RED router.
- **Packet Generation**: 2 units speed using Exponential Distribution.
- **Router Mechanism**: Store-and-forward with RED logic.

## Key Code Sections

### 1. Main Function (main.cpp)
**Purpose**: Entry point to initialize and run the simulation.

- **Block**: Includes and Main Function
  ```cpp
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

      net.run(100.0);

      return 0;
  }
  ```
  - **Explanation**: Initializes a `QCoreApplication`, creates a `Network` object, adds two sender PCs (lambda=2.0), one receiver PC (lambda=0.0), and a router (ID=1000). Connects nodes with links and runs the simulation for 100 units.

### 2. Network Class (network.h / network.cpp)
**Purpose**: Manages the network topology and simulation.

- **Constructor and Destructor (network.cpp)**:
  ```cpp
  Network::Network() {}

  Network::~Network() {
      for (auto pc : endNodes) delete pc;
      for (auto r : routers) delete r;
      for (auto l : links) delete l;
  }
  ```
  - **Explanation**: Default constructor initializes the network; destructor cleans up dynamically allocated PCs, routers, and links.

- **addSenderPC() (network.cpp)**:
  ```cpp
  void Network::addSenderPC(double lambda) {
      int id = endNodes.size() + 1;
      PC* pc = new PC(id, lambda);
      endNodes.push_back(pc);
  }
  ```
  - **Explanation**: Adds a sender PC with a given lambda (rate) for packet generation.

- **addReceiverPC() (network.cpp)**:
  ```cpp
  void Network::addReceiverPC() {
      int id = endNodes.size() + 1;
      PC* pc = new PC(id, 0.0);
      endNodes.push_back(pc);
  }
  ```
  - **Explanation**: Adds a receiver PC with lambda=0.0 (no packet generation).

- **addRouter() (network.cpp)**:
  ```cpp
  void Network::addRouter() {
      int id = routers.size() + 1000; // router IDs offset
      Router* r = new Router(id);
      routers.push_back(r);
  }
  ```
  - **Explanation**: Adds a router with an ID offset by 1000.

- **connect() (network.cpp)**:
  ```cpp
  void Network::connect(Node* a, Node* b) {
      Link* l = new Link(a, b);
      a->connectLink(l);
      b->connectLink(l);
      links.push_back(l);
  }
  ```
  - **Explanation**: Connects two nodes with a bidirectional link.

- **run() (network.cpp)**:
  ```cpp
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
          for (auto router : this->routers) {
              if (router->getNextSendTime() < nextEventTime) {
                  nextEventTime = router->getNextSendTime();
                  nextRouter = router;
              }
          }

          if (!nextPC && !nextRouter) break;
          QThread::msleep((nextEventTime - currentTime) * slowDown);
          currentTime = nextEventTime;

          if (nextRouter) {
              nextRouter->sendPacket(currentTime);
          } else if (nextPC) {
              nextPC->sendPacket(currentTime, 3); // HARDCODED FELLAS
          }
      }
  }
  ```
  - **Explanation**: Runs the simulation, determining the next event (PC or router) based on `nextSendTime`, updates time, and processes the event with a slowdown factor.

- **getPC() and getRouter() (network.cpp)**:
  ```cpp
  PC* Network::getPC(int id) {
      for (auto& pc : this->endNodes) {
          if (pc->getId() == id) {
              return pc;
          }
      }
      return nullptr;
  }

  Router* Network::getRouter(int id) {
      for (auto& router : this->routers) {
          if (router->getId() == id) {
              return router;
          }
      }
      return nullptr;
  }
  ```
  - **Explanation**: Retrieves a PC or router by ID, returning nullptr if not found.

### 3. Node Class (node.h / node.cpp)
**Purpose**: Base class for network nodes.

- **Constructor and connectLink() (node.cpp)**:
  ```cpp
  Node(int id) : id(id), nextSendTime(0.0) {}

  void Node::connectLink(Link* link) {
      links.push_back(link);
      ARPTable.push_back({link->getOtherEnd(this)->getId(), links.size() - 1});
  }
  ```
  - **Explanation**: Initializes node with ID and connects a link, updating the ARP table with the other node's ID and link index.

### 4. Packet Structure (Packet.h)
**Purpose**: Defines the packet data structure.

- **Definition**:
  ```cpp
  #ifndef PACKET_H
  #define PACKET_H
  #include <string>

  struct Packet {
      int id;
      int sourceId;
      int destId;
      int size;
      double timestamp;
      std::string data;

      Packet(int id, int sourceId, int destId, int size, double timestamp, std::string data)
          : id(id), sourceId(sourceId), destId(destId), size(size), timestamp(timestamp), data(data) {}
  };

  #endif // PACKET_H
  ```
  - **Explanation**: Stores packet details (ID, source, destination, size, timestamp, data).

### 5. PC Class (pc.h / pc.cpp)
**Purpose**: Handles packet generation and congestion for sender/receiver PCs.

- **Constructor (pc.cpp)**:
  ```cpp
  PC::PC(int id, double lambda) : Node(id), packetCounter(0), interArrivalDist(lambda), congestionWaitTime(0.0) {
      generator.seed(std::chrono::system_clock::now().time_since_epoch().count() + this->id);
      nextSendTime = getNextPacketDelay();
  }
  ```
  - **Explanation**: Initializes PC with ID and lambda, seeds the random generator, and sets the initial send time.

- **generatePacket() (pc.cpp)**:
  ```cpp
  Packet PC::generatePacket(double currentTime, int destId) {
      this->packetCounter++;
      std::string text = "THIS IS FROM PC " + std::to_string(this->id) + ". OVER\n";
      int size = text.length();
      int id = destId;
      return Packet(packetCounter, this->id, id, size, currentTime, text);
  }
  ```
  - **Explanation**: Generates a packet with a counter, source ID, destination ID, size, timestamp, and data.

- **getNextPacketDelay() (pc.cpp)**:
  ```cpp
  double PC::getNextPacketDelay() {
      return this->interArrivalDist(this->generator);
  }
  ```
  - **Explanation**: Returns the next packet delay using an exponential distribution.

- **getNextSendTime() (pc.cpp)**:
  ```cpp
  double PC::getNextSendTime() {
      return this->nextSendTime;
  }
  ```
  - **Explanation**: Returns the next scheduled send time.

- **receivePacket() (pc.cpp)**:
  ```cpp
  void PC::receivePacket(const Packet& p, Link* incomingLink) {
      if (p.destId == -1) {
          double randomDelay = 1.0 + static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (1.0)));
          this->congestionWaitTime = randomDelay;
          std::cout << "PC " << id << " received a congestion signal! Waiting for " << congestionWaitTime << " units." << std::endl;
      } else {
          std::cout << "PC " << id << " has received Packet " << p.id
                    << " from Node " << incomingLink->getOtherEnd(this)->getId()
                    << std::endl;
      }
  }
  ```
  - **Explanation**: Handles congestion signals (destId=-1) by setting a random wait time or processes regular packets.

- **sendPacket() (pc.cpp)**:
  ```cpp
  void PC::sendPacket(double currentTime, int destId) {
      std::cout << "Current Time: " << currentTime << ":\n";

      if (!this->links.empty()) {
          Packet p = this->generatePacket(currentTime, destId);
          std::cout << "PC " << this->id << " has sent Packet " << p.id << " to " << p.destId << std::endl;
          this->scheduleNext(currentTime);

          links[0]->send(p, this);
      } else {
          std::cout << "PC " << this->id << " is isolated! " << std::endl;
      }
  }
  ```
  - **Explanation**: Sends a packet if connected, schedules the next send, and uses the first link.

- **scheduleNext() (pc.cpp)**:
  ```cpp
  void PC::scheduleNext(double currentTime) {
      double baseDelay = getNextPacketDelay();

      if (congestionWaitTime > 0.0) {
          nextSendTime = currentTime + baseDelay + congestionWaitTime;
          std::cout << "PC " << id << " applied congestion wait of "
                    << congestionWaitTime << ", nextSendTime = "
                    << nextSendTime << std::endl;
          congestionWaitTime = 0.0;
      } else {
          nextSendTime = currentTime + baseDelay;
      }
  }
  ```
  - **Explanation**: Schedules the next send time, adding congestion wait if present.

### 6. Router Class (router.h / router.cpp)
**Purpose**: Implements the RED algorithm and store-and-forward logic.

- **Constructor and Constants (router.h)**:
  ```cpp
  class Router : public Node {
  private:
      const static int MAX_BUFFER_SIZE = 6;
      const static int MIN_THRESHOLD = 2;
      const static int MAX_THRESHOLD = 4;
      std::queue<Packet> buffer;
      void setARPTable(int id, int linkIndex);
      bool runRED(); // Just to say drop it or not
      void sendCongestionSignal();
  public:
      Router(int id) : Node(id) {}
      double getNextSendTime();
      void receivePacket(const Packet& p, Link* source) override;
      void sendPacket(double currentTime, int destId = -1) override;
  };
  ```
  - **Explanation**: Defines router with a 6-unit buffer, thresholds, and RED logic.

- **runRED() (router.cpp)**:
  ```cpp
  bool Router::runRED() {
      int qlen = buffer.size();
      if (qlen < MIN_THRESHOLD) return false;
      if (qlen >= MAX_THRESHOLD) return true;

      double dropProb = 0.3 * qlen - 0.4;
      double rnd = static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (1.0)));
      return rnd < dropProb;
  }
  ```
  - **Explanation**: Returns true if a packet should be dropped based on buffer size and probability.

- **getNextSendTime() (router.cpp)**:
  ```cpp
  double Router::getNextSendTime() {
      return this->nextSendTime;
  }
  ```
  - **Explanation**: Returns the next scheduled send time.

- **receivePacket() (router.cpp)**:
  ```cpp
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
  ```
  - **Explanation**: Applies RED to decide whether to drop or enqueue a packet, sending a congestion signal if dropped.

- **sendPacket() (router.cpp)**:
  ```cpp
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
          for (int i = 0; i < this->ARPTable.size(); i++) {
              if (this->ARPTable[i].first == p.destId) {
                  linkIndex = i;
                  break;
              }
          }
          if (linkIndex == -1) {
              std::cerr << " [Router] No ARP entry for dest " << p.destId << std::endl;
              return;
          }
          std::cout << " [Router] Forwarded Packet " << p.id << " From router to " << p.destId
                    << " at time " << currentTime << std::endl;

          this->nextSendTime += 1.0;
          links[linkIndex]->send(p, this);
      }
  }
  ```
  - **Explanation**: Forwards the next packet from the buffer using the ARP table, updating the send time by 1.0 unit.

- **setARPTable() (router.cpp)**:
  ```cpp
  void Router::setARPTable(int id, int linkIndex) {
      if (linkIndex >= 0 && linkIndex < (int)links.size()) {
          ARPTable.push_back({id, linkIndex});
      }
  }
  ```
  - **Explanation**: Adds an ARP entry for a destination ID and link index.

- **sendCongestionSignal() (router.cpp)**:
  ```cpp
  void Router::sendCongestionSignal() {
      Packet signalPacket(0, this->id, -1, 0, 0, "CONGESTION SIGNAL");

      for (auto& link : this->links) {
          if (link->getOtherEnd(this)->getId() == 1 || link->getOtherEnd(this)->getId() == 2) {
              link->send(signalPacket, this);
          }
      }
      std::cout << " [Router] Sent a congestion signal to the sender PCs." << std::endl;
  }
  ```
  - **Explanation**: Sends a congestion signal (destId=-1) to sender PCs (ID 1 and 2).

### 7. Link Class (link.h / link.cpp)
**Purpose**: Manages packet transmission between two nodes.

- **Definition and Constructor (link.h)**:
  ```cpp
  #ifndef LINK_H
  #define LINK_H

  #include "Packet.h"

  class Node;

  /*
   * This object is for connecting two nodes, not multiple nodes. BE CAREFUL!!!!!!!!!!!!
   */

  class Link
  {
  private:
      Node* A;
      Node* B;
  public:
      Link(Node* A, Node* B) : A(A), B(B) {}

      Node* getOtherEnd(Node* current);
      void send(const Packet& p, Node* source);
  };

  #endif // LINK_H
  ```
  - **Explanation**: Defines a `Link` class to connect exactly two nodes, with a warning against multiple connections.

- **send() (link.cpp)**:
  ```cpp
  void Link::send(const Packet& p, Node* source) {
      if (source == A) {
          this->B->receivePacket(p, this);
      } else if (source == B) {
          this->A->receivePacket(p, this);
      }
  }
  ```
  - **Explanation**: Sends a packet from the source node to the other end of the link.

- **getOtherEnd() (link.cpp)**:
  ```cpp
  Node* Link::getOtherEnd(Node* source) {
      return (this->A == source ? this->B : this->A);
  }
  ```
  - **Explanation**: Returns the node at the other end of the link from the given source.

## Project Structure
- `main.cpp`: Simulation entry point.
- `network.h` / `network.cpp`: Network management.
- `node.h` / `node.cpp`: Base node class.
- `Packet.h`: Packet structure.
- `pc.h` / `pc.cpp`: PC logic.
- `router.h` / `router.cpp`: RED router logic.
- `link.h` / `link.cpp`: Link management.

## How to Run
1. Clone: `git clone https://github.com/GreenGOC/CN-CA4.git`
2. Set up Qt Framework and C++11 compiler.
3. Build and run `main.cpp` with Qt Creator.

## Requirements
- C++11 or later compiler
- Qt Framework


## Notes
- Topology is static (defined in `main.cpp`).
- RED parameters are hardcoded in `router.h`.
- Refer to Piazza for rules.

## License
Educational use only; distribution requires permission.