#include "Switch.hpp"
#include "IP_Machine.hpp"

// TODO: Spanning tree protocol (BDPU)

Switch::Switch(size_t number_of_interfaces) : _number_of_interfaces(number_of_interfaces) {
    std::vector<Basic_Interface*> res = {};
    for (size_t i = 0; i < number_of_interfaces; i += 1) {
        Basic_Interface* n = new Basic_Interface(*this, i, MAC_DEFAULT);
        res.push_back(n);
    }
    set_interfaces(res);
}

void Switch::send(Packet& P) {
    LOG(_label, "Renvoi d'un paquet")
    MAC dest = P.data.ethernet.dest;
    if (_cam_table.is_mac_in(dest)){
        LOG(_label, "J'ai une entrée pour cette adresse MAC dans ma table MAC")
        _interfaces[_cam_table.to_mac(dest)]->send(P);
    }else{
        LOG(_label, "Je sais pas où aller, dans le doute j'envoi à toutes les directions")
        for (auto i: _interfaces) i.second->send(P);
        _waiting.push_back(dest);
    }
}

void Switch::receipt(Packet& p, interface_t from_interface) {
    LOG(_label, "Réception d'un paquet")
    MAC src = p.data.ethernet.src;
    MAC dest = p.data.ethernet.dest;
    auto it = find(_waiting.begin(), _waiting.end(), src);
    if (it != _waiting.end()) {
        std::cout << IP_Machine::MAC2char(*it) << std::endl;
        _cam_table.add_in_table(*it, from_interface);
        _waiting.erase(it);
    }
    send(p);
}


std::ostream& operator << (std::ostream& o, const Switch &S) {
    o << "+---------------[ Switch " << S._label << "]-------------------+" << std::endl;
     for (auto interface: S._interfaces) {
        o << "| eth";
        std::cout.setf(std::ios::left, std::ios::adjustfield);
        std::cout.width(7);
        o << interface.second->get_interface_number();
        o << "| ";
        std::cout.setf(std::ios::left, std::ios::adjustfield);
        std::cout.width(18);
        o << IP_Machine::MAC2char(interface.second->get_mac());
        o << "| ";
        std::cout.setf(std::ios::left, std::ios::adjustfield);
        std::cout.width(12);
        if (interface.second->is_linked()) {
            o << interface.second->get_link()->get_interface_connected_to(*interface.second)->get_machine()->get_label();
            o << "|" << std::endl;
        }else{
            o << " NON";
            o << "|" << std::endl;
        }
    }
    o << "+-----------+-------------------+-------------+";
    return o;
}