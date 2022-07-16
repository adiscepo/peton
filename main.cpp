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
    /*
        +--------+                +--------+                +--------+
        |        |   10.0.0.0/24  |        |   10.0.1.0/24  |        |
        |    B   |----------------|    A   |----------------|    C   |
        |        |                |        |                |        |
        +--------+                +--------+                +--------+
         10.0.0.2             10.0.0.1  10.0.1.1             10.0.1.4 
               (eth0)          (eth0)    (eth1)           (eth0)  
    */

    // IPv4 subnet_1 = IP_Machine::char2IPv4("10.0.0.0");
    // CIDR subnet_1_cidr = {24};

    // IPv4 subnet_2 = IP_Machine::char2IPv4("10.0.1.0"); 
    // CIDR subnet_2_cidr = {24};

    // MAC A_MAC1 = IP_Machine::char2MAC("00:00:00:00:0A:01");
    // IPv4 A_IP1 = IP_Machine::char2IPv4("10.0.0.1");
    // MAC A_MAC2 = IP_Machine::char2MAC("00:00:00:00:0A:02");
    // IPv4 A_IP2 = IP_Machine::char2IPv4("10.0.1.1");

    auto router = Machine_Factory::Application(2, {MAC(1),MAC(2)});
    auto ordi = Machine_Factory::Application(1, {MAC(3)});
    auto laptop = Machine_Factory::Application(1, {MAC(4)});
    auto tel = Machine_Factory::Application(1, {MAC(5)});
    auto server = Machine_Factory::Application(1, {MAC(6)});
    
    auto router_switch = Machine_Factory::S(5);

    router->set_IP(0, IP_Machine::char2IPv4("192.168.1.1"));
    laptop->set_IP(0, IP_Machine::char2IPv4("192.168.1.45"));
    tel->set_IP(0, IP_Machine::char2IPv4("192.168.1.40"));
    server->set_IP(0, IP_Machine::char2IPv4("192.168.1.3"));

    Machine::connect(*router, *router_switch, 0, 0);
    Machine::connect(*router_switch, *ordi, 1, 0);
    Machine::connect(*router_switch, *laptop, 2, 0);
    Machine::connect(*router_switch, *tel, 3, 0);
    Machine::connect(*router_switch, *server, 4, 0);

    std::cout << *router << std::endl;
    std::cout << *ordi << std::endl;
    std::cout << *laptop << std::endl;
    std::cout << *tel << std::endl;
    std::cout << *server << std::endl;
    std::cout << *router_switch << std::endl;
    
    IPv4 subnet = IP_Machine::char2IPv4("192.168.1.0");
    CIDR subnet_cidr = {24};

    router->get_routing_table().add_in_table(subnet, {24}, 0, 0, IP_Machine::IP_DEFAULT);
    tel->get_routing_table().add_in_table(subnet, {24}, 0, 0, IP_Machine::IP_DEFAULT);

    Packet* packet = Packet_Factory::ICMP(tel->interface(0), IP_Machine::char2IPv4("192.168.1.1"), Packet::ICMP::ICMP_Type::ECHO_req);
    tel->send(*packet);
    // auto router = Machine_Factory::Application(2, {A_MAC1, A_MAC2}, {A_IP1, A_IP2}, {{24}, {24}});
    // auto router_switch = Machine_Factory::S(5);
    // DHCP_Application* dhcp = new DHCP_Application(*router, 1);
    // dhcp->configure(A_IP2, subnet_2_cidr);
    // router->add_application(dhcp, 68);
    // router->get_routing_table().add_in_table(subnet_1, {24}, 0, 0, IP_Machine::IP_DEFAULT);
    // router->get_routing_table().add_in_table(subnet_2, {24}, 0, 1, IP_Machine::IP_DEFAULT);
    
    // auto machine_1 = Machine_Factory::Application(1, {IP_Machine::char2MAC("00:00:DE:AD:BE:EF")}, {IP_Machine::char2IPv4("10.0.0.3")}, {{24}});
    // DNS_Application* dns = new DNS_Application(*machine_1);
    // dns->configure("zandies.net", IP_Machine::IP_DEFAULT);
    // machine_1->get_routing_table().add_in_table(subnet_1, {24}, 0, 0, A_IP1);
    // machine_1->add_application(dns, 53);

    // auto machine_2 = Machine_Factory::Application(1, {IP_Machine::char2MAC("00:00:00:00:13:37")});
    
    // std::cout << *router << std::endl;
    // std::cout << *machine_1 << std::endl;
    // std::cout << *machine_2 << std::endl;

    // Machine::connect(*router, *machine_1, 0, 0);
    // Machine::connect(*router, *router_switch, 1, 0);
    // Machine::connect(*router_switch, *machine_2, 1, 0);

    // machine_2->runDHCP(0);

    // std::cout << *router << std::endl;
    // std::cout << *machine_1 << std::endl;
    // std::cout << *machine_2 << std::endl;

    // router->ip_route();
    // machine_2->ip_route();

    // router->arp();
    // machine_2->arp();
    
    // // // Crée un paquet ICMP pour effectuer une requête de ping de C vers B
    // Packet* p = Packet_Factory::ICMP(machine_2->interface(0), IP_Machine::char2IPv4("10.0.0.3"), Packet::ICMP::ICMP_Type::ECHO_req); // 
    // // // Packet* p = Packet_Factory::DHCP(*C_eth0, Packet::DHCP::DHCP_Message_Type::Discover, {}, {}, {}, {}, C_MAC); // 
    // machine_2->send(*p); // Envoie le paquet sur le réseau

    // return 0;
}