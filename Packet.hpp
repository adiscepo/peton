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
        ICMP
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
        uint16_t length;
        uint16_t checksum;
        friend std::ostream& operator<< (std::ostream& o, const UDP& P);
    };

    struct DHCP {
        // enum class DHCP_Options = {  };
        uint8_t OP;
        uint8_t HTYPE;
        uint8_t HLEN;
        uint8_t HOPS;
        uint32_t XID = 0x3903F326;
        IPv4 CIADDR;
        IPv4 YIADDR;
        IPv4 SIADDR;
        IPv4 GIADDR;    
        MAC CHADDR;
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
    } data{};

    static bool control_TTL(Packet& P);
    friend std::ostream& operator<< (std::ostream& o, const Packet& P) {
        if (P.type == Packet::Type::ETHERNET)   o << P.data.ethernet;
        if (P.type == Packet::Type::ARP)        o << P.data.arp;
        if (P.type == Packet::Type::IP)         o << P.data.ip;
        return o;
    }
};

class Packet_Factory { // Crée des paquets précis
public:
    static Packet* ETHERNET(MAC mac_sender, MAC mac_target, Packet::ETHERNET::EtherType type, Packet& payload);
    static Packet* ARP(IP_Machine& from, Packet::ARP::ARP_Opcode opcode, MAC mac_sender, IPv4 ip_sender, MAC mac_target, IPv4 ip_target);
    static Packet* IP(Interface& from, Packet::IP::IP_Protocol protocol, IPv4 ip_dest, Packet* payload);
    static Packet* ICMP(Interface& from, IPv4 ip_dest, Packet::ICMP::ICMP_Type type);
};

#endif