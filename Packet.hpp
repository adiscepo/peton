#ifndef _PACKET_
#define _PACKET_

#include "types.hpp"
class IP_Machine;
class Interface;

// TODO: Réfléchir à réprésenter les paquets sous la forme Composite des design patterns
// ou chaque element peut-être une classe dérivée de la classe paquet -> Risque de poser
// pb de problèmes de try_catch/casts pour pouvoir appeler les méthodes des classes dérivées 
// alors que la référence voulue est celle de la classe de base (Peut-être essayer en définissant 
// les références vers les classes dérivées dans les paramètres des fonctions)

struct Packet {
    enum class Type {
        ETHERNET,
        ARP,
        IP,
        ICMP,
        UDP,
        DHCP,
        DNS
    } type = Type::ETHERNET;

    // Réprésente une trame Ethernet
    struct ETHERNET {
        enum class EtherType { IP = 0x0800, ARP = 0x0806 };
        MAC src;
        MAC dest;
        EtherType type;
        Packet* payload;
        ETHERNET(MAC src, MAC dest, EtherType type, Packet* p) : src(src), dest(dest), type{type}, payload(p) {}
        ETHERNET& operator=(const ETHERNET& E) {
            if (this != &E) {
                src = E.src;
                dest = E.dest;
                type = E.type;
                payload = E.payload;
            }
            return *this;
        }
        ~ETHERNET() {};
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
        ~ARP() {};
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
        IP& operator=(const IP& I) {
            if (this != &I) {
                version = I.version;
                ttl = I.ttl;
                src = I.src;
                dest = I.dest;
                protocol = I.protocol;
                payload = I.payload;
            }
            return *this;
        }
        ~IP() {};
        friend std::ostream& operator<< (std::ostream& o, const IP& P);
    };

    struct ICMP {
        enum class ICMP_Type { ECHO_res = 0, ECHO_req = 8 };
        ICMP_Type type;
        ICMP(ICMP_Type type): type(type) {};
        ~ICMP() {};
        friend std::ostream& operator<< (std::ostream& o, const IP& P);
    };
    
    struct UDP {
        port_t src;
        port_t dest;
        uint16_t length = 0;
        uint16_t checksum = 0;
        Packet* payload;
        UDP(port_t src, port_t dest, Packet* payload) : src(src), dest(dest), payload(payload) {};
        UDP& operator=(const UDP& U) {
            if (this != &U) {
                src = U.src;
                dest = U.dest;
                length = U.length;
                checksum = U.checksum;
                payload = U.payload;
            }
            return *this;
        }
        ~UDP() {};
        friend std::ostream& operator<< (std::ostream& o, const UDP& P);
    };

    struct DHCP {
        enum class DHCP_Message_Type { Discover = 1, Offer = 2, Request = 3, Decline = 4, ACK = 5, NAK = 6, Release = 7, Inform = 8 };
        // uint8_t OP; // Operation Code: Specifies the general type of message. A value of 1 indicates a request message, while a value of 2 is a reply message.
        uint8_t HTYPE;
        uint8_t HLEN;
        uint8_t HOPS;
        uint32_t XID = 0x3903F326;
        IPv4 CIADDR; // Client IP
        IPv4 YIADDR; // Your IP
        IPv4 SIADDR; // Server IP
        IPv4 GIADDR; // Gateway IP
        MAC CHADDR;  // Client MAC
        // std::string SNAME; // Nom du serveur
        struct DHCP_Options {
            DHCP_Message_Type message_type;
            IPv4 address_request = 0; // Par défaut est égale à 0, si il y'a une autre valeur c'est que la machine veut cette IP spécifiquement
            CIDR subnet_mask;
            IPv4 router;
            IPv4 dhcp_server;
            void set_message_type (DHCP_Message_Type msg) { message_type = msg; }
        };
        DHCP_Options options;
        DHCP(DHCP_Message_Type type, IPv4 C, IPv4 Y, IPv4 S, IPv4 G, MAC M) 
            : CIADDR(C), YIADDR(Y), SIADDR(S), GIADDR(G), CHADDR(M) {options.message_type = type;};
        ~DHCP() {};
        friend std::ostream& operator<< (std::ostream& o, const DHCP& P);
    };

    // https://www.iana.org/assignments/dns-parameters/dns-parameters.xhtml
    // http://www.networksorcery.com/enp/protocol/dns.htm
    // http://www.tcpipguide.com/free/t_DNSMessageHeaderandQuestionSectionFormat.htm
    struct DNS {
        enum class DNS_QR { Query = 0, Reply = 1 };
        enum class DNS_Opcode { Query = 0, IQuery = 1, Status = 2, Notify = 4, Update = 5, DSO = 6 };  
        enum class DNS_Rcode { NoError = 0, FormErr = 1, ServFail = 2, NXDomain = 3, NotImp = 4, Refused = 5 };  
        enum class DNS_Type { A = 1, NS = 2, CNAME = 5, NUL = 10, MX = 15, AAAA = 28 };  
        enum class DNS_Class { IN = 1, CH = 3, HS = 4 };  
        struct DNS_Query {
            char* query_name;
            DNS_Type type;
            DNS_Class classe;
        };  
        struct DNS_RR {
            char* answer_name;
            DNS_Type type;
            DNS_Class classe;
            uint32_t ttl;
            uint16_t rlength;
            std::string payload;
        };  
        uint16_t transaction_id; // Permet d'identifier une paire réquête-réponse
        DNS_QR type;
        DNS_Opcode opcode;
        bool AA;    // Serveur authoritaire ?
        bool TC;    // Résultat tronqué ? (Seulement les 512 premiers bits de la réponse sont là) 
        bool RD;    // Récursion désirée ?
        bool RA;    // Récursion disponible ?
        DNS_Rcode rcode;
        uint16_t QDCount; // Nombre de questions qui ont été posées
        uint16_t ANCount; // Nombre d'entrées dans les réponses RR retournées
        uint16_t NSCount; // Nombres d'entrées dans les réponses authoritaire retournées
        uint16_t ARCount; // Nombres d'entrées dans les réponses additionnelles retournées
        std::vector<DNS_Query> query;
        std::vector<DNS_RR> rr;
        std::vector<DNS_RR> rr_authoritative;
        std::vector<DNS_RR> rr_additional;
        uint16_t length;
        uint16_t checksum;
        DNS(DNS_QR type, DNS_Opcode opcode, bool AA, bool TC, bool RD, bool RA, DNS_Rcode rcode, std::vector<DNS_Query> query, std::vector<DNS_RR> rr, std::vector<DNS_RR> rr_authoritative, std::vector<DNS_RR> rr_additional) :
        type(type), opcode(opcode), AA(AA), TC(TC), RD(RD), RA(RA), rcode(rcode), QDCount(QDCount), ANCount(ANCount), NSCount(NSCount), ARCount(ARCount), query(query), rr(rr), rr_authoritative(rr_authoritative), rr_additional(rr_additional) {
            QDCount = query.size(); ANCount = rr.size(); NSCount = rr_authoritative.size(); ARCount = rr_additional.size();
        };
        ~DNS() {};
        friend std::ostream& operator<< (std::ostream& o, const DNS& D);
    };

    union Data {
        ETHERNET ethernet;
        ARP arp;
        IP  ip;
        ICMP icmp;
        UDP udp;
        DNS dns;
        DHCP dhcp;
        ~Data() {};
    } data{};

    Packet() {};
    Packet& operator=(const Packet& P) { 
        std::cout << "Opérateur de copie du Packet" << std::endl;
        // TODO: Le faire mieux que ça
        if (this != &P) {        
            data.ethernet = P.data.ethernet;
            data.arp = P.data.arp;
            data.ip = P.data.ip;
            data.icmp = P.data.icmp;
            data.udp = P.data.udp;
            data.dns = P.data.dns;
            data.dhcp = P.data.dhcp;
        }
        return *this;
     };
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
    static Packet* DNS(Interface& from, Packet::DNS::DNS_QR type, Packet::DNS::DNS_Opcode opcode);

};

#endif