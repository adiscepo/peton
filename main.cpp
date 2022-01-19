#include <iostream>
#include <vector>
#include <bitset>
#include "IP_Machine.hpp"
#include "Interface.hpp"
#include "Link.hpp"
#include "Packet.hpp"

int main() {
    /*          
              routeur
                |
        [B]----[A]-----[C]
         |              |
       host           host
    
    */

    IPv4 subnet_1 = IP_Machine::char2IPv4("10.0.0.0");
    CIDR subnet_1_cidr = {24}; 
    IPv4 subnet_2 = IP_Machine::char2IPv4("10.0.1.0"); 
    CIDR subnet_2_cidr = {24}; 
    
    MAC A_MAC1 = IP_Machine::char2MAC("00:00:00:00:0A:01");
    IPv4 A_IP1 = IP_Machine::char2IPv4("10.0.0.1");
    MAC A_MAC2 = IP_Machine::char2MAC("00:00:00:00:0A:02");
    IPv4 A_IP2 = IP_Machine::char2IPv4("10.0.1.1");
    IP_Machine *A = new IP_Machine();
    Interface* A_eth0 = new Interface(*A, 0, A_MAC1, A_IP1, CIDR{24});
    Interface* A_eth1 = new Interface(*A, 1, A_MAC2, A_IP2, CIDR{24});
    A->set_interfaces({A_eth0, A_eth1});

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
    
    // Routing_Table a;
    // a.add_in_table(IP_Machine::char2IPv4("10.0.0.0"), CIDR{24}, 1, 0, nullptr);
    // a.add_in_table(IP_Machine::char2IPv4("10.0.1.0"), CIDR{24}, 1, 0, nullptr);
    // a.add_in_table(IP_Machine::char2IPv4("10.1.0.0"), CIDR{16}, 1, 0, nullptr);
    // std::cout << a << std::endl;
    // std::cout << IP_Machine::IPv42char(A_IP2) << std::endl;
    // std::cout << IP_Machine::IPv42char(a.longest_prefix(A_IP2)._subnet) << std::endl;
    // A->arp();
    // Machine::connect(A, &C, 1, 0);
    // std::cout << *A << std::endl << std::endl;
    // std::cout << *B << std::endl << std::endl;
    
    // Normalement ça doit être fait par les requêtes DHCP (le protocole donne un pack d'info de bienvenue)
    A->get_routing_table().add_in_table(subnet_1, subnet_1_cidr, 0, A_eth0->get_interface_number(), 0);
    A->get_routing_table().add_in_table(subnet_2, subnet_2_cidr, 0, A_eth1->get_interface_number(), 0);
    C->get_routing_table().add_in_table(IP_DEFAULT, {0}, 1, C_eth0->get_interface_number(), A_IP1);
    B->get_routing_table().add_in_table(IP_DEFAULT, {0}, 1, B_eth0->get_interface_number(), A_IP2);
    
    // IP_Header* paquet = new IP_Header(A_IP, C_IP, {});
    // ARP_Payload arp_req(A_MAC, MAC_ARP_REQUEST, A_IP, C_IP, ARP_OPCODE::REQUEST);
    // Packet P_ARP(A_MAC, MAC_BROADCAST, Packet::ETHERTHYPE::ARP, &arp_req);
    // A->send(P_ARP);
    // A->get_arp_table()->add_in_table(C_MAC, C_IP, 0);
    // A->arp();

    Packet* p = Packet_Factory::ICMP(*C_eth0, B_IP, Packet::ICMP::ICMP_Type::ECHO_req);
    
    C->send(*p);

    // Packet* p = Packet_Factory::ARP(*A, Packet::ARP::ARP_Opcode::REQUEST, A_MAC1, A_IP1, MAC_ARP_REQUEST, C_IP);
    // Packet* p = Packet_Factory::ETHERNET(A_MAC1, MAC_BROADCAST, Packet::ETHERNET::EtherType::ARP, *Packet_Factory::ARP(*A, Packet::ARP::ARP_Opcode::REQUEST, A_MAC1, A_IP1, MAC_ARP_REQUEST, C_IP));
    // A->send(*p, 1);
    // A->arp();
    // B->arp();
    // C->arp();
    // A->send(*p); // Ne fonctionne que sur un même réseau local -> je dois ajouter les tables de routages dans chaque IP_Machine;
    // A->arp();
    // B->arp();
    // C->arp();
    // std::cout << p << std::endl;
    // std::cout << *p << std::endl;
    // Packet P(A_MAC, C_MAC, Packet::ETHERTHYPE::IP, paquet);
    // A->send(P);
    // B->arp();
    // C.arp();
    // std::cout << A->get_routing_table() << std::endl;
    // std::cout << C->get_routing_table() << std::endl;
    return 0;
}