#include "DHCP_Application.hpp"
#include "../Packet.hpp"
#include "../IP_Machine.hpp"
#include "../Interface.hpp"
#include <math.h>

// Exception
struct DHCPAllIPsAssigned : public std::exception { const char * what () const throw () { return "All addressables IP of the subnet were attribued. There is no more IP available."; } };

void DHCP_Application::socket(Packet& P, interface_t from_interface) {
    std::cout << "[DHCP Server]" << std::endl;
    switch (P.data.dhcp.options.message_type) {
    case Packet::DHCP::DHCP_Message_Type::Discover:
        discover(P, from_interface);
        break;
    case Packet::DHCP::DHCP_Message_Type::Request:
        request(P, from_interface);
        break;
    case Packet::DHCP::DHCP_Message_Type::Offer:
        offer(P, from_interface);
        break;
    case Packet::DHCP::DHCP_Message_Type::ACK:
        ack(P, from_interface);
        break;
    default:
        break;
    }
}

IPv4 DHCP_Application::generate_ip() {
    int i = 2;
    IPv4 res;
    while (i < pow(2, 32 - _subnet_mask.x)) { // L'ip doit être dans la partie hôte du réseau (32-CIDR)
        res = get_subnet_part(_router_ip, _subnet_mask) + i;
        if (!_assigned_ip.count(res)) return res; 
        i+=1;
    }
    throw DHCPAllIPsAssigned();
}

void DHCP_Application::discover(Packet& P, interface_t from_interface) { 
    LOG("DHCP Server", "Requête de découverte reçue.")
    IPv4 offer;
    // Si la machine a demandé une IP spécifique et que celle-ci est disponible il l'attribue
    if (P.data.dhcp.options.address_request != 0 && !_assigned_ip.count(P.data.dhcp.options.address_request)) offer = P.data.dhcp.options.address_request;
    else offer = generate_ip();
    Packet* res = Packet_Factory::DHCP(_connected.interface(from_interface), Packet::DHCP::DHCP_Message_Type::Offer, {}, offer, _router_ip, {}, P.data.dhcp.CHADDR);
    res->data.dhcp.options.router = _router_ip;
    res->data.dhcp.options.subnet_mask = _subnet_mask;
    send(*res);
    delete res;
};

void DHCP_Application::offer(Packet& P, interface_t from_interface) {
    LOG("DHCP Client", "Requête d'offre reçue.")
    LOG("DHCP Client", "IP " + IP_Machine::IPv42char(P.data.dhcp.YIADDR) + " proposée")
    Packet* res = Packet_Factory::DHCP(_connected.interface(from_interface), Packet::DHCP::DHCP_Message_Type::Request, P.data.dhcp.YIADDR, {}, _router_ip, {}, P.data.dhcp.CHADDR);
    send(*res);
    delete res;
};

void DHCP_Application::request(Packet& P, interface_t from_interface) {
    LOG("DHCP Server", "Requête de requête reçue.")
    _assigned_ip.insert({P.data.dhcp.CIADDR, P.data.dhcp.CHADDR});
    Packet* res = Packet_Factory::DHCP(_connected.interface(from_interface), Packet::DHCP::DHCP_Message_Type::ACK, {}, P.data.dhcp.CIADDR, _router_ip, {}, P.data.dhcp.CHADDR);
    res->data.dhcp.options.router = _router_ip;
    res->data.dhcp.options.subnet_mask = _subnet_mask;
    send(*res);
    delete res;
};

void DHCP_Application::decline(Packet& P, interface_t from_interface) {

};

void DHCP_Application::ack(Packet& P, interface_t from_interface) {
    LOG("DHCP Client", "Requête d'acquittement reçue.")
    Interface* new_interface = new Interface(_connected, from_interface, P.data.dhcp.CHADDR, P.data.dhcp.YIADDR, P.data.dhcp.options.subnet_mask);
    _connected.get_routing_table().add_in_table(get_subnet_part(P.data.dhcp.options.router, P.data.dhcp.options.subnet_mask), P.data.dhcp.options.subnet_mask, 0, from_interface, P.data.dhcp.options.router);
    _connected.set_interface(*new_interface);
    _connected.ip_route();
};

void DHCP_Application::nak(Packet& P, interface_t from_interface) {

};

void DHCP_Application::release(Packet& P, interface_t from_interface) {

};

void DHCP_Application::inform(Packet& P, interface_t from_interface) {

};
