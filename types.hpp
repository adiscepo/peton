#ifndef _TYPES_
#define _TYPES_

#include <iostream>
#include <tuple>
#include <vector>
class IP_Machine;

#ifdef DEBUG
#define DEBUG(log) std::cout << log << std::endl;
#define LOG(machine, log) std::cout <<"[" << machine << "] " << log << std::endl;
#else
#define DEBUG(log);
#define LOG(machine, log);
#endif

typedef uint32_t IPv4;
typedef __int128_t IPv6;
typedef uint interface_t;

typedef struct CIDR {
    interface_t x:5;
    friend std::ostream& operator<<(std::ostream& o, const CIDR& C) { 
        o << C.x;
        return o;
    }
} __attribute__((packed)) CIDR;

IPv4 get_mask(CIDR mask);

typedef struct MAC {
    uint64_t x:48;
    friend std::ostream& operator<<(std::ostream& o, const MAC& M) { 
        o << M.x;
        return o;
    }
    bool operator==(const MAC& M) { if (x == M.x) return true; return false;}
    bool operator<(const MAC& M) { if (x < M.x) return true; return false;}
    bool operator>(const MAC& M) { if (x > M.x) return true; return false;}    
    bool operator!=(const MAC& M) { if (x != M.x) return true; return false;}
} __attribute__((packed)) MAC;

typedef interface_t TTL;

class ARP_Table {
private:
    typedef struct Entries {
        IPv4 ip;
        MAC mac;
        interface_t interface;
    } Entries;
    std::vector<Entries> _content = {};
public:
    inline ARP_Table() = default;
    bool is_MAC_in(MAC mac);
    bool is_IP_in(IPv4 ip);
    MAC from_IP(IPv4 ip);
    IPv4 from_MAC(MAC mac);
    size_t size() { return _content.size(); }
    bool add_in_table(MAC mac, IPv4 ip, interface_t interface);
    std::vector<Entries> get_content() { return _content; }
    
    interface_t get_interface(MAC mac) { return get_content(mac).interface; };
    interface_t get_interface(IPv4 ip) { return get_content(ip).interface; };
    Entries get_content(MAC mac);
    Entries get_content(IPv4 ip);
    ~ARP_Table() noexcept = default;
    friend std::ostream& operator<<(std::ostream& o, const ARP_Table& R);
};

class Routing_Table {
    typedef struct Entries {
        IPv4 _subnet;
        CIDR _mask;
        uint _metric;
        interface_t _interface;
        IP_Machine* _next_hop_routeur;
    } Entries;
    std::vector<Entries> _content = {};
public:
    inline Routing_Table() = default;
    bool add_in_table(IPv4 subnet, CIDR mask, uint metric, interface_t interface, IP_Machine* next_hop_router);
    bool is_IP_in(IPv4 ip);
    Entries longest_prefix(IPv4 ip);
    friend std::ostream& operator<<(std::ostream& o, const Routing_Table& R);
};

enum class ARP_OPCODE {
    REQUEST = 1,
    RESPONSE = 2
};

class Payload {
public:
    virtual MAC get_mac_src() { return {}; }
    virtual MAC get_mac_dest() { return {}; }
    virtual IPv4 get_ip_src() { return {}; }
    virtual IPv4 get_ip_dest() { return {}; }
    virtual Payload* get_payload() { return {}; }
    virtual ARP_OPCODE get_type() { return {}; }
    virtual ~Payload() noexcept = default;
};

class ARP_Payload : public Payload {
    MAC _mac_src;
    MAC _mac_dest;
    IPv4 _ip_src;
    IPv4 _ip_dest;
    ARP_OPCODE _type;
public:
    ARP_Payload(MAC mac_src, MAC mac_dest, IPv4 ip_src, IPv4 ip_dest, ARP_OPCODE type) :
        _mac_src(mac_src), _mac_dest(mac_dest), _ip_src(ip_src), _ip_dest(ip_dest), _type(type) {}
    inline MAC get_mac_src() override { return _mac_src; };
    inline MAC get_mac_dest() override { return _mac_dest; };
    inline IPv4 get_ip_src() override { return _ip_src; };
    inline IPv4 get_ip_dest() override { return _ip_dest; };
    inline ARP_OPCODE get_type() override { return _type; };

};

class IP_Header : public Payload {
    IPv4 _ip_src;
    IPv4 _ip_dest;
    Payload* _payload;
public:
    IP_Header(IPv4 ip_src, IPv4 ip_dest, Payload* payload) :
        _ip_src(ip_src), _ip_dest(ip_dest), _payload(payload) {}
    inline IPv4 get_ip_src() override { return _ip_src; };
    inline IPv4 get_ip_dest() override { return _ip_dest; };
    inline Payload* get_payload() override { return _payload; };
    inline constexpr IP_Header(const IP_Header&) = delete;
    inline constexpr IP_Header& operator=(const IP_Header&) = delete;
};

class ICMP_Header : public Payload {

};

const MAC MAC_BROADCAST = {0xffffffffffff};
const MAC MAC_ARP_REQUEST = {0x000000000000};
const MAC MAC_ERROR = {};

#endif