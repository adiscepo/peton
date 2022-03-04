#ifndef _MACHINE_
#define _MACHINE_

#include <iostream>
#include <map>
#include "Packet.hpp"
#include "Interface.hpp"
#include "Link.hpp"

class Machine {
protected:
    char _label;
    std::map<interface_t, Basic_Interface*> _interfaces = {};
public:
    inline Machine() : _label(static_cast<char>(65 + total)) { Machine::total += 1; };
    Machine(std::vector<Basic_Interface*>::iterator begin, std::vector<Basic_Interface*>::iterator end) {for (auto i = begin; i != end; ++i) _interfaces.insert({(*i)->get_interface_number(), *i}); };
    Machine(std::vector<Interface*>::iterator begin, std::vector<Interface*>::iterator end) {for (auto i = begin; i != end; ++i) _interfaces.insert({(*i)->get_interface_number(), *i}); };
    bool as_interface(interface_t interface) 
        { return (interface < _interfaces.size()) ? true : false; }
    void set_interface(Basic_Interface& interface);
    void set_interface(Interface& interface);
    void set_interfaces(std::vector<Basic_Interface*> interface);
    void set_interfaces(std::vector<Interface*> interface);

    virtual void send(Packet& P) = 0;
    virtual void receipt(Packet& p, interface_t from_interface) = 0;
    
    char get_label() { return _label; }
    void set_label(char s) { _label = s; }
    
    virtual ~Machine() noexcept = default;

    static int total;
    static bool connect(Machine& machine1, Machine& machine2, interface_t interface1, interface_t interface2);
};

#endif