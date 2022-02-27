#ifndef _APPLICATIONS_DHCP_APPLICATION_
#define _APPLICATIONS_DHCP_APPLICATION_

#include "Application.hpp"
#include <map>

class DHCP_Application : public Application {
    IPv4 _router_ip;
    CIDR _subnet_mask;
    IPv4 _domaine_server_name;
    IPv4 _broadcast_ip;
    std::map<IPv4, MAC> _assigned_ip = {};
    
    void discover(Packet& P, interface_t from_interface);
    void offer(Packet& P, interface_t from_interface);
    void request(Packet& P, interface_t from_interface);
    void decline(Packet& P, interface_t from_interface);
    void ack(Packet& P, interface_t from_interface);
    void nak(Packet& P, interface_t from_interface);
    void release(Packet& P, interface_t from_interface);
    void inform(Packet& P, interface_t from_interface);

    IPv4 generate_ip();
public:
    DHCP_Application() noexcept = default;
    DHCP_Application(IP_Machine& machine) : Application::Application(machine) {};
    
    void configure(IPv4 router_ip, CIDR subnet_mask) {_router_ip = router_ip; _subnet_mask = subnet_mask;}

    void socket(Packet& P, interface_t from_interface) override;

    ~DHCP_Application();
};

#endif // _APPLICATIONS_DHCP_APPLICATION_