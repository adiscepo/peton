#ifndef _MACHINE_FACTORY_
#define _MACHINE_FACTORY_

#include "IP_Machine.hpp"
#include "Application_Machine.hpp"
#include "Switch.hpp"

class Machine_Factory {
public:
    static IP_Machine* IP(uint number_of_interfaces, std::vector<MAC> list_of_mac);
    static IP_Machine* IP(uint number_of_interfaces, std::vector<MAC> list_of_mac, std::vector<IPv4> list_of_ip, std::vector<CIDR> list_of_mask);
    static Application_Machine* Application(uint number_of_interfaces, std::vector<MAC> list_of_mac);
    static Application_Machine* Application(uint number_of_interfaces, std::vector<MAC> list_of_mac, std::vector<IPv4> list_of_ip, std::vector<CIDR> list_of_mask);
    static Switch* S(uint number_of_interfaces) {Switch* res = new Switch(number_of_interfaces); return res;};
};  

#endif