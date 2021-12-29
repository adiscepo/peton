#ifndef _INTERFACE_
#define _INTERFACE_

#include "types.hpp"
#include "Packet.hpp"
class IP_Machine;
class Link;

class Basic_Interface {
protected:
    IP_Machine* _connected = nullptr;
    interface_t _interface;
    MAC _mac;
    Link* _linked_to = nullptr;
public:
    inline constexpr Basic_Interface(IP_Machine& M, interface_t interface, MAC mac) : _connected(&M), _interface(interface), _mac(mac) {};
    inline constexpr Basic_Interface(IP_Machine& M, interface_t interface, MAC mac, Link* l) : _connected(&M), _interface(interface), _mac(mac), _linked_to(l) {};
    
    inline Basic_Interface(const Basic_Interface&) = delete;
    inline Basic_Interface& operator=(const Basic_Interface&) = delete;
    
    inline bool is_linked() { return !(_linked_to == nullptr); }
    inline void set_link(Link* l) { _linked_to = l; }
    inline IP_Machine* get_machine() { return _connected; }
    inline MAC get_mac() { return _mac; }
    inline Link* get_link() { return _linked_to; }
    inline interface_t get_interface_number() { return _interface; }
    
    bool send(Packet& P);
    bool receipt(Packet& P);

    static void connect(Basic_Interface& i1, Basic_Interface& i2) noexcept;

    virtual ~Basic_Interface() noexcept = default;
    
    friend std::ostream& operator << (std::ostream& o, const Basic_Interface& I) {return o;};
};

// bool Basic_Interface::connect(Interface& i1, Interface& i2) noexcept {};

class Interface : public Basic_Interface{
private:
    IPv4 _ip;
    CIDR _cidr;
public:
    Interface(IP_Machine& M, interface_t interface, MAC mac, IPv4 ip, CIDR cidr) : Basic_Interface(M, interface, mac), _ip(ip), _cidr(cidr) {};
    Interface(IP_Machine& M, interface_t interface, MAC mac, Link* l, IPv4 ip, CIDR cidr) : Basic_Interface(M, interface, mac, l), _ip(ip), _cidr(cidr) {};
    inline IPv4 get_ip() { return _ip; }
    inline CIDR get_cidr() { return _cidr; }
    ~Interface() override = default;

    friend std::ostream& operator << (std::ostream& o, const Interface& I);
};

#endif