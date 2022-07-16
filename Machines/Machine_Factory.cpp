#include "Machine_Factory.hpp"
#include "IP_Machine.hpp"
#include "../Interface.hpp"

struct IncorrectInterfaceMACNumber : public std::exception { const char * what () const throw () { return "The number of interfaces and the number of MAC address are different."; } };

IP_Machine* Machine_Factory::IP(uint number_of_interfaces, std::vector<MAC> list_of_mac) {
    if (list_of_mac.size() != number_of_interfaces) throw IncorrectInterfaceMACNumber();
    IP_Machine *res = new IP_Machine();
    std::vector<Interface*> inter = {};
    for (interface_t i = 0; i < number_of_interfaces; i++) {
        Interface * in = new Interface(*res, i, list_of_mac[i], NO_IP, NO_CIDR);
        inter.push_back(in);
    }
    res->set_interfaces(inter);
    return res;
}

IP_Machine* Machine_Factory::IP(uint number_of_interfaces, std::vector<MAC> list_of_mac, std::vector<IPv4> list_of_ip, std::vector<CIDR> list_of_mask) {
    if (list_of_mac.size() != number_of_interfaces || list_of_ip.size() != number_of_interfaces || list_of_mask.size() != number_of_interfaces) throw IncorrectInterfaceMACNumber();
    IP_Machine *res = new IP_Machine();
    std::vector<Interface*> inter = {};
    for (interface_t i = 0; i < number_of_interfaces; i++) {
        Interface * in = new Interface(*res, i, list_of_mac[i], list_of_ip[i], list_of_mask[i]);
        inter.push_back(in);
    }
    res->set_interfaces(inter);
    return res;
}

Application_Machine* Machine_Factory::Application(uint number_of_interfaces, std::vector<MAC> list_of_mac) {
    if (list_of_mac.size() != number_of_interfaces) throw IncorrectInterfaceMACNumber();
    Application_Machine *res = new Application_Machine();
    std::vector<Interface*> inter = {};
    for (interface_t i = 0; i < number_of_interfaces; i++) {
        Interface * in = new Interface(*res, i, list_of_mac[i], NO_IP, NO_CIDR);
        inter.push_back(in);
    }
    res->set_interfaces(inter);
    return res;
}

Application_Machine* Machine_Factory::Application(uint number_of_interfaces, std::vector<MAC> list_of_mac, std::vector<IPv4> list_of_ip, std::vector<CIDR> list_of_mask) {
    if (list_of_mac.size() != number_of_interfaces || list_of_ip.size() != number_of_interfaces || list_of_mask.size() != number_of_interfaces) throw IncorrectInterfaceMACNumber();
    Application_Machine *res = new Application_Machine();
    std::vector<Interface*> inter = {};
    for (interface_t i = 0; i < number_of_interfaces; i++) {
        Interface * in = new Interface(*res, i, list_of_mac[i], list_of_ip[i], list_of_mask[i]);
        inter.push_back(in);
    }
    res->set_interfaces(inter);
    return res;
}