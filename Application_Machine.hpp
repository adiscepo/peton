#ifndef _APPLICATION_MACHINE_
#define _APPLICATION_MACHINE_

#include "IP_Machine.hpp"
#include "Applications/Application.hpp"

class Application_Machine : public IP_Machine {
    std::map<port_t, Application*> _applications = {};
protected:
    void action(Packet& p, interface_t from_interface) override;
    void UDP_action(Packet& P, interface_t from_interface);
public:
    Application_Machine() : IP_Machine() {};
    Application_Machine(bool forward) : IP_Machine(forward) {};
    Application_Machine(bool forward, std::vector<Interface*> interfaces) : IP_Machine(forward, interfaces) {};
    
    void add_application(Application* app, port_t port) {_applications.insert({port, app});}
    void kill_application(port_t port) {_applications.erase(port);}
    
    void runDHCP(interface_t interface); // Permet d'obtenir toutes les infos sur notre réseau et d'obtenir une IP
    void runDHCP(interface_t interface, IPv4 ip); // Permet d'obtenir toutes les infos sur notre réseau et d'obtenir une IP

    void transport_layer(Packet& P, interface_t from_interface);
    void application_layer(Packet& P, interface_t from_interface, port_t port);

    virtual ~Application_Machine() override = default;


    friend std::ostream& operator << (std::ostream& o, const Application_Machine& A);
};

#endif