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
IPv4 get_subnet(IPv4 ip, CIDR mask);

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
        IPv4 _via;
    } Entries;
    std::vector<Entries> _content = {};
public:
    inline Routing_Table() = default;
    bool add_in_table(IPv4 subnet, CIDR mask, uint metric, interface_t interface, IPv4 via);
    bool is_IP_in(IPv4 ip);
    Entries longest_prefix(IPv4 ip);
    friend std::ostream& operator<<(std::ostream& o, const Routing_Table& R);
};

enum class ARP_OPCODE {
    REQUEST = 1,
    RESPONSE = 2
};

const MAC MAC_BROADCAST = {0xffffffffffff};
const MAC MAC_ARP_REQUEST = {0x000000000000};
const MAC MAC_ERROR = {};

const IPv4 IP_DEFAULT = 0x0;

#endif