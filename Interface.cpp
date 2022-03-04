#include "Interface.hpp"
#include "Link.hpp"
#include "Machines/IP_Machine.hpp"

// Méthode statique

void Basic_Interface::connect(Basic_Interface& i1, Basic_Interface& i2) noexcept {
    Link* link = new Link(i1, i2);
    i1.set_link(link);
    i2.set_link(link);
};

bool Basic_Interface::send(Packet& P){
    // Ne peut envoyer que des paquets Ethernet (couche 1)
    if (is_linked()) {
        if (P.type == Packet::Type::ETHERNET) {
            _linked_to->forward(P, *this);
            return true;
        }
        DEBUG("Impossible d'envoyer un paquet incomplet sur le réseau")
        return false;
    }
    return false;
};

bool Basic_Interface::receipt(Packet& P) {
    // DEBUG("L'interface recoit un paquet depuis le lien")
    // Ne peut recevoir que des paquets Ethernet (couche 1)
    if (is_linked()) {
        if (P.type == Packet::Type::ETHERNET) {
            _connected->receipt(P, _interface);
            return true;
        }
        DEBUG("Impossible d'envoyer un paquet incomplet sur le réseau")
        return false;
    }
    return false;
};

// Méthodes

IP_Machine* Interface::get_machine() { 
    return dynamic_cast<IP_Machine*>(_connected); 
}
    

std::ostream& operator<< (std::ostream& o, const Interface& I){
    o << IP_Machine::IPv42char(I._ip) << "/" << I._cidr << IP_Machine::MAC2char(I._mac) << "connecte à " << I._connected->get_label();
    return o;
};