#ifndef _APPLICATIONS_APPLICATION_
#define _APPLICATIONS_APPLICATION_

#include "../Packet.hpp"
#include "../IP_Machine.hpp"

class Application {
protected:
    // port_t _port;
    IP_Machine& _connected;
public:
    // Application() noexcept = default;
    Application(IP_Machine& machine) : _connected(machine) {};
    virtual void socket(Packet& P, interface_t from_interface) = 0;
    void send(Packet& P) { _connected.send(P); }
    ~Application();
};

#endif // _APPLICATIONS_APPLICATION_