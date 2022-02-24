#include "Link.hpp"
#include "Interface.hpp"
// #include "Interface.cpp"

void Link::forward(Packet& packet, Basic_Interface& from) {
    Basic_Interface* to = get_interface_connected_to(from);
    // DEBUG(packet)
    // if (packet.get_type() == Packet::ETHERTHYPE::IP) std::cout << "Envoi d'un paquet IP de " << from->get_label() << " à " << to->get_label() << std::endl;
    // if (packet.get_type() == Packet::ETHERTHYPE::ARP) std::cout << "Envoi d'un paquet ARP de " << from->get_label() << " à " << to->get_label() << std::endl;
    // std::cout << "+-----------------------------+" << std::endl;
    // std::cout << "|   From: " << from.get_label() << "    |    To: " << to->get_label() << "     |" << std::endl;
    // std::cout << packet << std::endl;
    to->receipt(packet);
};

interface_t Link::get_interface_of(const Basic_Interface& m) { 
    return (_connected[0] == &m) ? _connected[0]->get_interface_number() : _connected[1]->get_interface_number(); 
}

Basic_Interface* Link::get_interface_connected_to(const Basic_Interface& m) {
    return (_connected[0] != &m) ? _connected[0] : _connected[1];
}

void Link::modifyMachine(interface_t i, char s) {
    // _connected[i]->set_label(s);
}