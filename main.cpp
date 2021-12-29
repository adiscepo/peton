#include <iostream>
#include <vector>
#include <bitset>
#include "IP_Machine.hpp"
#include "Interface.hpp"
#include "Link.hpp"
#include "Packet.hpp"

int main() {
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
    IPv4 C_IP = IP_Machine::char2IPv4("10.0.0.4");
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


    // IP_Header* paquet = new IP_Header(A_IP, C_IP, {});
    // ARP_Payload arp_req(A_MAC, MAC_ARP_REQUEST, A_IP, C_IP, ARP_OPCODE::REQUEST);
    // Packet P_ARP(A_MAC, MAC_BROADCAST, Packet::ETHERTHYPE::ARP, &arp_req);
    // A->send(P_ARP);
    // A->get_arp_table()->add_in_table(C_MAC, C_IP, 0);
    // A->arp();
    // Remplacer (*A, 1) par (Interface&) ? Permet de pas devoir l'interface_t de la machine, mais uniquement l'interface
    Packet* p = Packet_Factory::IP(*A_eth1, Packet::IP::IP_Protocol::ICMP, B_IP, nullptr);
    // Packet* p = Packet_Factory::ARP(*A, Packet::ARP::ARP_Opcode::REQUEST, A_MAC1, A_IP1, MAC_ARP_REQUEST, C_IP);
    // Packet* p = Packet_Factory::ETHERNET(A_MAC1, MAC_BROADCAST, Packet::ETHERNET::EtherType::ARP, *Packet_Factory::ARP(*A, Packet::ARP::ARP_Opcode::REQUEST, A_MAC1, A_IP1, MAC_ARP_REQUEST, C_IP));
    // A->send(*p, 1);
    A->arp();
    A->send(*p);
    A->arp();
    // std::cout << p << std::endl;
    // std::cout << *p << std::endl;
    // Packet P(A_MAC, C_MAC, Packet::ETHERTHYPE::IP, paquet);
    // A->send(P);
    // B->arp();
    // C.arp();
    
    return 0;
}