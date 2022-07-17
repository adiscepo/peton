#include <iostream>
#include <vector>
#include <bitset>
#include "Machines/Machine_Factory.hpp"
#include "Machines/IP_Machine.hpp"
#include "Machines/Switch.hpp"
#include "Machines/Application_Machine.hpp"
#include "Applications/DHCP_Application.hpp"
#include "Applications/DNS_Application.hpp"
#include "Interface.hpp"
#include "Link.hpp"
#include "Packet.hpp"

/* Farontu:
 * DNS
 * TCP
 * HTTP
 */

int main() {

    auto router = Machine_Factory::Application(2, {MAC(1),MAC(2)});
    auto ordi = Machine_Factory::Application(1, {MAC(3)});
    auto laptop = Machine_Factory::Application(1, {MAC(4)});
    auto tel = Machine_Factory::Application(1, {MAC(5)});
    auto server = Machine_Factory::Application(1, {MAC(6)});
    
    auto router_switch = Machine_Factory::S(5);

    router->set_IP(0, IP_Machine::char2IPv4("192.168.1.1"));

    Machine::connect(*router, *router_switch, 0, 0);
    Machine::connect(*router_switch, *ordi, 1, 0);
    Machine::connect(*router_switch, *laptop, 2, 0);
    Machine::connect(*router_switch, *tel, 3, 0);
    Machine::connect(*router_switch, *server, 4, 0);
    
    IPv4 subnet = IP_Machine::char2IPv4("192.168.1.0");
    CIDR subnet_cidr = {24};

    router->get_routing_table().add_in_table(subnet, {24}, 0, 0, IP_Machine::IP_DEFAULT);
    
    DHCP_Application* dhcp = new DHCP_Application(*router, 0);
    dhcp->configure(IP_Machine::char2IPv4("192.168.1.1"), subnet_cidr);

    router->add_application(dhcp, 68);

    tel->runDHCP(0, IP_Machine::char2IPv4("192.168.1.40"));
    laptop->runDHCP(0, IP_Machine::char2IPv4("192.168.1.41"));
    ordi->runDHCP(0);
    server->runDHCP(0, IP_Machine::char2IPv4("192.168.1.3"));

    std::cout << *router << std::endl;
    std::cout << *ordi << std::endl;
    std::cout << *laptop << std::endl;
    std::cout << *tel << std::endl;
    std::cout << *server << std::endl;

    Packet* packet = Packet_Factory::ICMP(tel->interface(0), IP_Machine::char2IPv4("192.168.1.1"), Packet::ICMP::ICMP_Type::ECHO_req);
    Packet* packet2 = Packet_Factory::ICMP(laptop->interface(0), IP_Machine::char2IPv4("192.168.1.3"), Packet::ICMP::ICMP_Type::ECHO_req);
    Packet* packet3 = Packet_Factory::ICMP(server->interface(0), IP_Machine::char2IPv4("192.168.1.40"), Packet::ICMP::ICMP_Type::ECHO_req);
    
    tel->send(*packet);
    laptop->send(*packet2);
    server->send(*packet3);
    
    std::cout << *router_switch << std::endl;
    
    return 0;
}