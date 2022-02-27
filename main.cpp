#include <iostream>
#include <vector>
#include <bitset>
#include "IP_Machine.hpp"
#include "Application_Machine.hpp"
#include "Applications/DHCP_Application.hpp"
#include "Interface.hpp"
#include "Machine_Factory.hpp"
#include "Link.hpp"
#include "Packet.hpp"

/* Farontu:
 * UDP  
 * DHCP (Permet une connexion plus Plug&Play entre les machines)
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

    IPv4 subnet_1 = IP_Machine::char2IPv4("10.0.0.0");
    CIDR subnet_1_cidr = {24};

    IPv4 subnet_2 = IP_Machine::char2IPv4("10.0.1.0"); 
    CIDR subnet_2_cidr = {24};

    MAC A_MAC1 = IP_Machine::char2MAC("00:00:00:00:0A:01");
    IPv4 A_IP1 = IP_Machine::char2IPv4("10.0.0.1");
    MAC A_MAC2 = IP_Machine::char2MAC("00:00:00:00:0A:02");
    IPv4 A_IP2 = IP_Machine::char2IPv4("10.0.1.1");
    
    auto router = Machine_Factory::Application(2, {A_MAC1, A_MAC2}, {A_IP1, A_IP2}, {{24}, {24}});
    DHCP_Application* dhcp = new DHCP_Application(*router, 1);
    dhcp->configure(A_IP2, subnet_2_cidr);
    router->add_application(dhcp, 68);

    auto machine_1 = Machine_Factory::Application(1, {IP_Machine::char2MAC("00:00:00:00:13:37")});
    
    auto machine_2 = Machine_Factory::Application(1, {IP_Machine::char2MAC("00:00:00:00:BE:EF")});
    
    std::cout << *router << std::endl;
    std::cout << *machine_1 << std::endl;
    std::cout << *machine_2 << std::endl;

    IP_Machine::connect(*router, *machine_1, 0, 0);
    IP_Machine::connect(*router, *machine_2, 1, 0);

    machine_1->runDHCP(0);
    machine_2->runDHCP(0);

    std::cout << *router << std::endl;
    std::cout << *machine_1 << std::endl;
    std::cout << *machine_2 << std::endl;

    // Application_Machine *A = new Application_Machine(); // Crée la machine A
    // Interface* A_eth0 = new Interface(*A, 0, A_MAC1, A_IP1, subnet_1_cidr); // Crée une interface eth0 ayant l'adresse MAC 00:00:00:00:0A:01 et l'ip 10.0.0.1/24
    // Interface* A_eth1 = new Interface(*A, 1, A_MAC2, A_IP2, subnet_2_cidr); // Idem mais avec d'autres valeurs
    // A->set_interfaces({A_eth0, A_eth1}); // Assigne les interface à la machine A

    // // Ajoute un serveur DHCP sur la machine A
    // DHCP_Application* dhcp = new DHCP_Application(*A);
    // dhcp->configure(A_IP2, subnet_2_cidr);
    
    // A->add_application(dhcp, 68);

    // MAC B_MAC = IP_Machine::char2MAC("00:00:00:00:0B:01");
    // IPv4 B_IP = IP_Machine::char2IPv4("10.0.0.2");
    // IP_Machine *B = new IP_Machine();
    // Interface* B_eth0 = new Interface(*B, 0, B_MAC, B_IP, subnet_1_cidr);
    // B->set_interface(*B_eth0);

    // MAC C_MAC = IP_Machine::char2MAC("00:00:00:00:0C:1F");
    // // IPv4 C_IP = IP_Machine::char2IPv4("10.0.1.4");
    // Application_Machine *C = new Application_Machine();
    // Interface* C_eth0 = new Interface(*C, 0, C_MAC, {}, subnet_2_cidr);
    // C->set_interface(*C_eth0);

    // IP_Machine::connect(*A, *B, 0, 0);
    // IP_Machine::connect(*A, *C, 1, 0);
    
    // C->runDHCP(0);
    // // Crée un paquet ICMP pour effectuer une requête de ping de C vers B
    // // Packet* p = Packet_Factory::ICMP(*C_eth0, B_IP, Packet::ICMP::ICMP_Type::ECHO_req); // 
    // // Packet* p = Packet_Factory::DHCP(*C_eth0, Packet::DHCP::DHCP_Message_Type::Discover, {}, {}, {}, {}, C_MAC); // 
    // // C->send(*p); // Envoie le paquet sur le réseau
    // std::cout << *C << std::endl;
    // std::cout << *A << std::endl;
    return 0;
}