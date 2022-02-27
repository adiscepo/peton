#include "Packet.hpp"
#include "Application_Machine.hpp"

void Application_Machine::action(Packet& P, interface_t from_interface) {
    IP_Machine::action(P, from_interface);
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
