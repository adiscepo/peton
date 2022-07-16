#ifndef _IP_MACHINE_
#define _IP_MACHINE_

#include "Machine.hpp"
#include "../types.hpp"
#include <string>
#include <map>

class Interface;
class Packet;

class IP_Machine : public Machine {
protected:
    ARP_Table _arp_table = {};
    Routing_Table _routing_table = {};
    bool _forward = true; // Machine can forward packet ?

    virtual void action(Packet& p, interface_t from_interface);
    void ARP_action(Packet& P, interface_t from_interface);
    void IP_action(Packet& P, interface_t from_interface);
    void ICMP_action(Packet& P, interface_t from_interface);
    void add_in_routing_table(IPv4 ip, CIDR mask, uint metric, interface_t interface);
    bool as_ip(IPv4 ip);
    bool as_mac(MAC mac);
public:
    IP_Machine() : Machine() {};
    IP_Machine(bool forward) : Machine(), _forward(forward) {};
    IP_Machine(bool forward, std::vector<Interface*> interfaces) : Machine(interfaces.begin(), interfaces.end()), _forward(forward) {};

    void set_IP(interface_t interface, IPv4 ip);

    bool as_interface(interface_t interface) 
        { return (interface < _interfaces.size()) ? true : false; }
    Interface& interface(interface_t i) { return dynamic_cast<Interface&>(*_interfaces[i]); }

    void send(Packet& P) override;
    void receipt(Packet& p, interface_t from_interface) override;

    bool can_forward() { return _forward; }

    ARP_Table& get_arp_table() { return _arp_table; };
    Routing_Table& get_routing_table() { return _routing_table; };
    // // POUR TEST UNIQUEMENT
    // Routing_Table& get_routing_table() { return _routing_table; };
    // // -----
    void physical_layer(Packet& P, interface_t from_interface);
    void datalink_layer(Packet& P, interface_t from_interface);
    void internet_layer(Packet& P, interface_t from_interface);

    // Affiche les tables en ASCII
    void arp();
    void ip_route();

    virtual ~IP_Machine() noexcept = default;

    static const IPv4 IP_DEFAULT;
    static IPv4 char2IPv4(std::string ip);
    static std::string IPv42char(IPv4 ip);
    static MAC char2MAC(std::string mac);
    static std::string MAC2char(MAC mac);
    friend std::ostream& operator << (std::ostream& o, const IP_Machine& I);
};



#endif