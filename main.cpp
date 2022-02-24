#include <iostream>
#include <vector>
#include <bitset>
#include "IP_Machine.hpp"
#include "Interface.hpp"
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
                               (eth0)    (eth1)
    */

    IPv4 subnet_1 = IP_Machine::char2IPv4("10.0.0.0");
    CIDR subnet_1_cidr = {24};

    IPv4 subnet_2 = IP_Machine::char2IPv4("10.0.1.0"); 
    CIDR subnet_2_cidr = {24}; 
    
    MAC A_MAC1 = IP_Machine::char2MAC("00:00:00:00:0A:01");
    IPv4 A_IP1 = IP_Machine::char2IPv4("10.0.0.1");
    MAC A_MAC2 = IP_Machine::char2MAC("00:00:00:00:0A:02");
    IPv4 A_IP2 = IP_Machine::char2IPv4("10.0.1.1");
    IP_Machine *A = new IP_Machine(); // Crée la machine A
    Interface* A_eth0 = new Interface(*A, 0, A_MAC1, A_IP1, CIDR{24}); // Crée une interface eth0 ayant l'adresse MAC 00:00:00:00:0A:01 et l'ip 10.0.0.1/24
    Interface* A_eth1 = new Interface(*A, 1, A_MAC2, A_IP2, CIDR{24}); // Idem mais avec d'autres valeurs
    A->set_interfaces({A_eth0, A_eth1}); // Assigne les interface à la machine A

    MAC B_MAC = IP_Machine::char2MAC("00:00:00:00:0B:01");
    IPv4 B_IP = IP_Machine::char2IPv4("10.0.0.2");
    IP_Machine *B = new IP_Machine();
    Interface* B_eth0 = new Interface(*B, 0, B_MAC, B_IP, CIDR{24});
    B->set_interface(*B_eth0);

    MAC C_MAC = IP_Machine::char2MAC("00:00:00:00:0C:1F");
    IPv4 C_IP = IP_Machine::char2IPv4("10.0.1.4");
    IP_Machine *C = new IP_Machine();
    Interface* C_eth0 = new Interface(*C, 0, C_MAC, C_IP, CIDR{24});

    C->set_interface(*C_eth0);
    IP_Machine::connect(*A, *B, 1, 0);
    IP_Machine::connect(*A, *C, 0, 0);
    
    // Normalement ça doit être fait par les requêtes DHCP (le protocole donne un pack d'info de bienvenue)
    A->get_routing_table().add_in_table(subnet_1, subnet_1_cidr, 0, A_eth0->get_interface_number(), 0);
    A->get_routing_table().add_in_table(subnet_2, subnet_2_cidr, 0, A_eth1->get_interface_number(), 0);
    B->get_routing_table().add_in_table(IP_DEFAULT, {0}, 1, B_eth0->get_interface_number(), A_IP2);
    C->get_routing_table().add_in_table(IP_DEFAULT, {0}, 1, C_eth0->get_interface_number(), A_IP1);
    
    // Crée un paquet ICMP pour effectuer une requête de ping de C vers B
    // Packet* p = Packet_Factory::ICMP(*C_eth0, B_IP, Packet::ICMP::ICMP_Type::ECHO_req); // 
    Packet* p = Packet_Factory::DHCP(*C_eth0, Packet::DHCP::DHCP_Message_Type::Discover, {}, {}, {}, {}, C_MAC); // 
    C->send(*p); // Envoie le paquet sur le réseau

    return 0;
}