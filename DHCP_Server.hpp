#include "IP_Machine.hpp"

class DHCP_Server: public IP_Machine {
	// void action(Packet& P, interface_t from_interface) override {std::cout << "[Serveur DHCP] Action" << std::endl;};
public:
	DHCP_Server() : IP_Machine() {};
	DHCP_Server(bool forward, std::vector<Interface*> interfaces) : IP_Machine(forward, interfaces) {};
	~DHCP_Server() {};
};
