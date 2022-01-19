#ifndef _IP_MACHINE_
#define _IP_MACHINE_

// #include "Machine.hpp"
#include "types.hpp"
#include <string>
#include <map>
class Interface;
class Packet;

class IP_Machine {
private:
    char _label;
    ARP_Table _arp_table = {};
    Routing_Table _routing_table = {};
    std::map<interface_t, Interface*> _interfaces = {};

    void action(Packet& p, interface_t from_interface);
    void ARP_action(Packet& P, interface_t from_interface);
    void IP_action(Packet& P, interface_t from_interface);
    void ICMP_action(Packet& P, interface_t from_interface);
    void add_in_routing_table(IPv4 ip, CIDR mask, uint metric, interface_t interface);
    bool as_ip(IPv4 ip);
    bool as_mac(MAC mac);
public:
    IP_Machine() : _label(static_cast<char>(65 + total)) { IP_Machine::total += 1; };
    IP_Machine(std::vector<Interface*> interfaces);

    void set_interface(Interface& interface);
    void set_interfaces(std::vector<Interface*> interface);

    bool as_interface(interface_t interface) 
        { return (interface < _interfaces.size()) ? true : false; }
    Interface& interface(interface_t i) { return *_interfaces[i]; }

    void send(Packet& P);
    void receipt(Packet& p, interface_t from_interface);

    ARP_Table& get_arp_table() { return _arp_table; };
    Routing_Table& get_routing_table() { return _routing_table; };
    // // POUR TEST UNIQUEMENT
    // Routing_Table& get_routing_table() { return _routing_table; };
    // // -----
    void physical_layer(Packet& P, interface_t from_interface);
    void datalink_layer(Packet& P, interface_t from_interface);

    void arp();
    void ip_route();

    char get_label() { return _label; }
    void set_label(char s) { _label = s; }

    static int total;
    static bool connect(IP_Machine& machine1, IP_Machine& machine2, interface_t interface1, interface_t interface2);
    static IPv4 char2IPv4(std::string ip);
    static std::string IPv42char(IPv4 ip);
    static MAC char2MAC(std::string mac);
    static std::string MAC2char(MAC mac);
    friend std::ostream& operator << (std::ostream& o, const IP_Machine& I);
};



#endif