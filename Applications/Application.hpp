#ifndef _APPLICATIONS_APPLICATION_
#define _APPLICATIONS_APPLICATION_

#include "../Packet.hpp"
#include "../Machines/IP_Machine.hpp"

class Application {
protected:
    std::string _type = "Application";
    // port_t _port;
    IP_Machine& _connected;
public:
    // Application() noexcept = default;
    Application(IP_Machine& machine) : _connected(machine) {};
    virtual void socket(Packet& P, interface_t from_interface) = 0;
    void send(Packet& P) { _connected.send(P); }
    std::string get_type() { return _type; }
    virtual ~Application() noexcept = default;
};

#endif // _APPLICATIONS_APPLICATION_