#include "Packet.hpp"
#include "Interface.hpp"
#include "Application_Machine.hpp"
#include "Applications/DHCP_Application.hpp"

void Application_Machine::action(Packet& P, interface_t from_interface) {
    IP_Machine::action(P, from_interface);
    P = *P.data.ip.payload;
    if (P.type == Packet::Type::UDP) transport_layer(P, from_interface);
}

void Application_Machine::transport_layer(Packet& P, interface_t from_interface) {
    switch (P.type) {
    case Packet::Type::UDP:
        UDP_action(P, from_interface);
        break;
    default:
        break;
    };
}

void Application_Machine::UDP_action(Packet& P, interface_t from_interface) {
    if (_applications.count(P.data.udp.dest)) application_layer(*P.data.udp.payload, from_interface, P.data.udp.dest);
}

void Application_Machine::application_layer(Packet& P, interface_t from_interface, port_t port) {
    if (_applications.count(port)) _applications[port]->socket(P, from_interface);
    else std::cout << "J'ai reçu un paquet mais aucune application ne tourne sur ce port là" << std::endl;
}

void Application_Machine::runDHCP(interface_t on_interface) {
    LOG(_label, "lance un client DHCP")
    Application* dhcp = new DHCP_Application(*this);
    add_application(dhcp, 67); // Lance un client DHCP sur le port 67
    Interface& inter = interface(on_interface);
    Packet* p = Packet_Factory::DHCP(inter, Packet::DHCP::DHCP_Message_Type::Discover, {}, {}, {}, {}, inter.get_mac());
    p->data.ethernet.payload->data.ip.payload->data.udp.payload->data.dhcp.options.address_request = IP_Machine::char2IPv4("10.0.1.166");
    inter.send(*p);
    LOG(_label, "éteint le client DHCP")
    kill_application(67);
    delete dhcp;
}
