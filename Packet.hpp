#ifndef _PACKET_
#define _PACKET_

#include "types.hpp"
class IP_Machine;
class Interface;

struct Packet {
    enum class Type {
        ETHERNET,
        ARP,
        IP,
        ICMP,
        UDP,
        DHCP
    } type = Type::ETHERNET;

    // Réprésente une trame Ethernet
    struct ETHERNET {
        enum class EtherType { IP = 0x0800, ARP = 0x0806 };
        MAC src;
        MAC dest;
        EtherType type;
        Packet* payload;
        ETHERNET(MAC src, MAC dest, EtherType type, Packet* p) : src(src), dest(dest), type{type}, payload(p) {}
        friend std::ostream& operator<< (std::ostream& o, const ETHERNET& P);
    };

    struct ARP {
        enum class ARP_Opcode { REQUEST = 1, RESPONSE = 2 };
        size_t HLEN = 6; // Taille d'une adresse MAC (Hardware)
        size_t PLEN = 4; // Taille d'une adresse IP  (Protocol)
        ARP_Opcode opcode;
        MAC mac_sender;
        IPv4 ip_sender;
        MAC mac_target;
        IPv4 ip_target;
        ARP(ARP_Opcode o, MAC ms, IPv4 is, MAC mt, IPv4 it) : opcode{o}, mac_sender(ms), ip_sender(is), mac_target(mt), ip_target(it) {}
        friend std::ostream& operator<< (std::ostream& o, const ARP& P);
    };

    struct IP {
        enum class IP_Version { IPv4 = 0b100, IPv6 = 0b110 };
        enum class IP_Protocol { ICMP = 1, TCP = 6, EGP = 8, IGP = 9, UDP = 17, OSPF = 89 };
        IP_Version version = IP_Version::IPv4;
        TTL ttl = 64;
        IP_Protocol protocol;
        IPv4 src;
        IPv4 dest;
        Packet* payload; 
        IP(IP_Protocol protocol, IPv4 s, IPv4 d, Packet* p) : protocol{protocol}, src(s), dest(d), payload(p) {}
        friend std::ostream& operator<< (std::ostream& o, const IP& P);
    };

    struct ICMP {
        enum class ICMP_Type { ECHO_res = 0, ECHO_req = 8 };
        ICMP_Type type;
        ICMP(ICMP_Type type): type(type) {};
        friend std::ostream& operator<< (std::ostream& o, const IP& P);
    };
    
    struct UDP {
        port_t src;
        port_t dest;
        uint16_t length = 0;
        uint16_t checksum = 0;
        Packet* payload;
        UDP(port_t src, port_t dest, Packet* payload) : src(src), dest(dest), payload(payload) {};
        friend std::ostream& operator<< (std::ostream& o, const UDP& P);
    };

    struct DHCP {
        enum class DHCP_Message_Type { Discover = 1, Offer = 2, Request = 3, Decline = 4, ACK = 5, NAK = 6, Release = 7, Inform = 8 };
        DHCP_Message_Type message_type;
        // uint8_t OP; // Operation Code: Specifies the general type of message. A value of 1 indicates a request message, while a value of 2 is a reply message.
        // uint8_t HTYPE;
        // uint8_t HLEN;
        // uint8_t HOPS;
        uint32_t XID = 0x3903F326;
        IPv4 CIADDR; // Client IP
        IPv4 YIADDR; // Your IP
        IPv4 SIADDR; // Server IP
        IPv4 GIADDR; // Gateway IP
        MAC CHADDR;  // Client MAC
        // std::string SNAME; // Nom du serveur
        DHCP(DHCP_Message_Type type, IPv4 C, IPv4 Y, IPv4 S, IPv4 G, MAC M) 
            : message_type(type), CIADDR(C), YIADDR(Y), SIADDR(S), GIADDR(G), CHADDR(M) {};
        friend std::ostream& operator<< (std::ostream& o, const DHCP& P);
    };

    struct DNS {
        enum class DNS_QR { Query = 0, Reply = 1 };
        enum class DNS_Opcode { Query = 0, IQuery = 1, Status = 2, Notify = 4, Update = 5, DSO = 6 };  
        enum class DNS_Rcode { NOERROR = 0, FORMERR = 1, SERVFAIL = 2, NXDOMAIN = 3 };  
        DNS_QR type;
        DNS_Opcode opcode;
        bool AA;
        bool TC;
        bool RD;
        bool RA;
        DNS_Rcode rcode;
        port_t dest;
        uint16_t length;
        uint16_t checksum;
        friend std::ostream& operator<< (std::ostream& o, const UDP& P);
    };

    union Data {
        ETHERNET ethernet;
        ARP arp;
        IP  ip;
        ICMP icmp;
        UDP udp;
        DNS dns;
        DHCP dhcp;
    } data{};

    static bool control_TTL(Packet& P);
    friend std::ostream& operator<< (std::ostream& o, const Packet& P) {
        if (P.type == Packet::Type::ETHERNET)   o << P.data.ethernet;
        if (P.type == Packet::Type::ARP)        o << P.data.arp;
        if (P.type == Packet::Type::IP)         o << P.data.ip;
        if (P.type == Packet::Type::DHCP)       o << P.data.dhcp;
        return o;
    }
};

class Packet_Factory { // Crée des paquets précis
public:
    static Packet* ETHERNET(MAC mac_sender, MAC mac_target, Packet::ETHERNET::EtherType type, Packet& payload);
    static Packet* ARP(IP_Machine& from, Packet::ARP::ARP_Opcode opcode, MAC mac_sender, IPv4 ip_sender, MAC mac_target, IPv4 ip_target);
    static Packet* IP(Interface& from, Packet::IP::IP_Protocol protocol, IPv4 ip_dest, Packet* payload);
    static Packet* ICMP(Interface& from, IPv4 ip_dest, Packet::ICMP::ICMP_Type type);
    static Packet* UDP(Interface& from, IPv4 ip_dest, port_t src, port_t dest, Packet* payload);
    static Packet* DHCP(Interface& from, Packet::DHCP::DHCP_Message_Type type, IPv4 C, IPv4 Y, IPv4 S, IPv4 G, MAC M);

};

#endif