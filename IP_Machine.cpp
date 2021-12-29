#include "IP_Machine.hpp"
#include "Packet.hpp"
#include <bitset>
#include "Link.hpp"
#include "Interface.hpp"
// #include "Interface.cpp" // Besoin pour avoir accès à la méthode statique

// Méthodes statiques

int IP_Machine::total = 0;

bool IP_Machine::connect(IP_Machine& machine1, IP_Machine& machine2, interface_t interface1, interface_t interface2){
    if (machine1.as_interface(interface1) && machine2.as_interface(interface2)) {
        Basic_Interface::connect(*(machine1._interfaces[interface1]), *(machine2._interfaces[interface2]));
        return true;
    }
    return false;
}

IPv4 IP_Machine::char2IPv4(std::string ip) {
    IPv4 res = 0;
    IPv4 part[4];
    std::string tmp = ip;
    size_t i = 0;
    size_t pos = 0;
    while (i < 4) {
        pos = ip.find(".");
        part[i] = static_cast<IPv4>(atoi(ip.substr(0, pos).c_str())); 
        ip.erase(0, pos + 1);
        i += 1;
    }
    res = (part[0] << 24) + (part[1] << 16) + (part[2] << 8) + part[3];
    return res;
};

std::string IP_Machine::IPv42char(IPv4 ip) {
    std::string res;
    for (size_t i = 0; i < 4; i++) {
        res += std::to_string((ip >> ((3-i)*8)) & ((0b11111111)));
        if (i != 3) res += ".";
    }
    return res;
}

MAC IP_Machine::char2MAC(std::string mac) {
    if (mac.length() == 17) {
        MAC res = {};
        long unsigned part[6];
        size_t i = 0;
        size_t pos = 0;
        while (i < 6) {
            pos = mac.find(":");
            part[i] = static_cast<long unsigned int>(strtol(mac.substr(0, pos).c_str(), NULL, 16)); 
            mac.erase(0, pos + 1);
            i += 1;
        }
        for (size_t i = 0; i < 6; i++) res.x += part[i] << (5-i)*8; 
        return res;
    }else{
        throw std::invalid_argument("Eh ! Tu m'as pas mis une adresse MAC correcte !");
    }
}

std::string IP_Machine::MAC2char(MAC mac) {
    std::string res;
    char* r = new char;
    for (size_t i = 0; i < 12; i++) {
        // std::cout << std::bitset<4>((mac.x >> ((3-i)*4))) << std::endl;
        sprintf(r, "%lX", (mac.x >> ((11-i)*4)) & ((0b1111)));
        res += r;
        if (i != 11 && i%2 == 1) res += ":";
    }
    return res;
}

IPv4 IP_Machine::get_net_part(IPv4 ip, CIDR mask){
    IPv4 subnet_mask = 0b11111111111111111111111111111111;
    if (mask.x == 0) return subnet_mask; // Pcq quand mask = 32 : overflow et mask = 0
    for (size_t i = 1; i <= 32; i++) if (i > mask.x) subnet_mask ^= 1 << (32-i);
    return subnet_mask;
};

// Méthodes

IP_Machine::IP_Machine(std::vector<Interface*> interfaces) : _label(static_cast<char>(65 + total)) 
    {for (auto elem: interfaces) _interfaces.insert({elem->get_interface_number(), elem}); IP_Machine::total += 1; };

void IP_Machine::set_interface(Interface& interface) {
    _interfaces[interface.get_interface_number()] = &interface;
}

void IP_Machine::set_interfaces(std::vector<Interface*> interfaces) {
    for (auto elem: interfaces) _interfaces.insert({elem->get_interface_number(), elem});
}

void IP_Machine::physical_layer(Packet& P, interface_t from_interface) {
    // On passe la paquet directement à la couche liaison de données, il n'y a rien que l'on puisse faire ici (en tant qu'émulateur ce n'est pas nécéssaire)
    // DEBUG(P);
    action(P, from_interface);
}

void IP_Machine::datalink_layer(Packet& P, interface_t from_interface) {
    // Lorsqu'un paquet arrive sur le réseau, on se limite au contenu direct (trame ethernet)
    // Est-ce que le paquet nous est destiné ?
    // Si oui, je l'ouvre et passe à la couche suivante (internet)
    // Sinon est-ce que nous pouvons le transférer quelque part ? 
    if (P.type == Packet::Type::ETHERNET) { // Il s'agit d'un paquet Ethernet (y'a que ça ici)
        LOG(_label, "arrivé avec le protocole Ethernet")
        if (P.data.ethernet.dest == _interfaces[from_interface]->get_mac() || (P.data.ethernet.dest == MAC_BROADCAST && P.data.ethernet.src != _interfaces[from_interface]->get_mac())) { // Le paquet m'est destiné: soit directement, soit par broadcast (on check que le broadcast ne provienne pas de nous)
            LOG(_label, "ce paquet est pour moi");
            if (P.data.ethernet.payload != nullptr) {
                action(*P.data.ethernet.payload, from_interface);
            }
        }else{
            LOG(_label, "ce paquet n'est pas pour moi");
            if (_arp_table.is_MAC_in(P.data.ethernet.dest)) {
                LOG(_label, "J'ai l'adresse MAC dans ma table ARP");
            }else{
                LOG(_label, "J'ai pas l'adresse MAC dans ma table ARP, je forge un paquet de requête ARP pour savoir à qui l'adresse IP appartient")
                // Packet* req_arp = Packet_Factory::ARP(Packet::ARP::ARP_Opcode::REQUEST, _interfaces[from_interface]->get_mac(), _interfaces[from_interface]->get_ip(), MAC_BROADCAST, )
            }
        }
    }
}

void IP_Machine::send(Packet& P) {
    MAC dest = P.data.ethernet.dest;
    if (_arp_table.is_MAC_in(dest)){
        LOG(_label, "envoie un paquet");
        _interfaces[_arp_table.get_interface(dest)]->send(P);
    }else{
        // LOG(_label, "n'envoie pas un paquet");
    }
};

void IP_Machine::receipt(Packet& P, interface_t from_interface) {
    LOG(_label, "reçoit un paquet."); 
    physical_layer(P, from_interface);
};

void IP_Machine::ARP_action(Packet& P, interface_t from_interface) {
    LOG(_label, "le paquet contient un header ARP");
    // Est-ce que c'est mon adresse MAC que l'on veut ? On le sait en regardant l'IP de destination
    if (P.data.arp.ip_target == interface(from_interface)->get_ip()){
        if (P.data.arp.opcode == Packet::ARP::ARP_Opcode::REQUEST){
            LOG(_label, "Oui c'est bien moi qu'on cherche !");
            // Répond à la requête par un nouveau paquet ARP
            // Ajoute les infos du paquet que l'on vient de recevoir dans la table ARP de la machine
            _arp_table.add_in_table(P.data.arp.mac_sender, P.data.arp.ip_sender, from_interface);
            Packet* res = Packet_Factory::ARP(*this, Packet::ARP::ARP_Opcode::RESPONSE, interface(from_interface)->get_mac(), interface(from_interface)->get_ip(), P.data.arp.mac_sender, P.data.arp.ip_sender); 
            // interface(from_interface);
            send(*res);
        }else if (P.data.arp.opcode == Packet::ARP::ARP_Opcode::RESPONSE){
            LOG(_label, "On a répondu à ma requête ARP !");
            // Répond à la requête par un nouveau paquet ARP
            // Ajoute les infos du paquet que l'on vient de recevoir dans la table ARP de la machine
            _arp_table.add_in_table(P.data.arp.mac_sender, P.data.arp.ip_sender, from_interface);
        }
    }else{
        LOG(_label, "Ah bah non c'était pas pour moi...");
        // Ignore
    }
    // Payload* p = P.get_content();
    // if (p->get_type() == ARP_OPCODE::REQUEST) { // C'est une requête pour savoir si l'adresse IP dans le paquet m'appartient
    //     LOG(_label, "c'est une requête");
    //     if (p->get_ip_dest() == _ip){ // Oui c'est la mienne
    //         LOG(_label, "c'est une requête qui m'est destinée");
    //         // Je forge un paquet pour répondre
    //         ARP_Payload *arp_res = new ARP_Payload(_mac, p->get_mac_src(), _ip, p->get_ip_src(), ARP_OPCODE::RESPONSE);
    //         Packet response(_mac, p->get_mac_src(), Packet::ETHERTHYPE::ARP, arp_res);
    //         // J'ajoute l'entrée dans ma table ARP pour pouvoir répondre
    //         if (!_arp_table.is_MAC_in(p->get_mac_src())) {
    //             LOG(_label, "j'ajoute l'entrée dans ma table ARP");
    //             _arp_table.add_in_table(p->get_mac_src(), p->get_ip_src(), from_interface);
    //         }
    //         LOG(_label, "je répond par une requête ARP de réponse");
    //         forward(response, from_interface); // On renvoit la réponse par la même interface par laquelle il est arrivé
    //     }else{ // Non c'est pas à moi
    //         LOG(_label, "c'est une requête qui ne m'est pas destinée");
    //         // Osef
    //     }
    // }else if (p->get_type() == ARP_OPCODE::RESPONSE) {
    //     LOG(_label, "j'ai reçu une réponse ARP");
    //     if (p->get_ip_dest() == _ip){ // Oui c'est bien moi le destinataire
    //         LOG(_label, "j'ajoute l'entrée dans ma table ARP");
    //         _arp_table.add_in_table(p->get_mac_src(), p->get_ip_src(), from_interface);
    //     }
    // }
}

void IP_Machine::IP_action(Packet& P, interface_t from_interface) {
    std::cout << IP_Machine::IPv42char(P.data.ip.src) << " a envoyé un paquet IP. On l'a reçu depuis l'interface eth";
    std::cout << from_interface << std::endl;
}


void IP_Machine::action(Packet& P, interface_t from_interface) {
    switch (P.type) {
    case Packet::Type::ETHERNET:
        datalink_layer(P, from_interface);
        break;
    case Packet::Type::ARP:
        ARP_action(P, from_interface);
        break;
    case  Packet::Type::IP:
        IP_action(P, from_interface);
    default:
        break;
    }
}

void IP_Machine::arp() { 
    std::cout << "+--------------------------------------------------+" << std::endl;
    std::cout << "|                   TABLE ARP de " << _label << "                 |" << std::endl;
    std::cout << _arp_table << std::endl; 
};

std::ostream& operator << (std::ostream& o, const IP_Machine &I) {
    o << "La machine " << I._label << " a " << I._interfaces.size() << " interfaces:"; 
    for (auto interface: I._interfaces) {
        if (interface.second != nullptr) {
            o << "\n - eth" << interface.second->get_interface_number() << " connectée à " << interface.second->get_link()->get_interface_connected_to(*interface.second)->get_machine()->get_label();
        }else{
            o << "\n - eth" << interface.second->get_interface_number() << " n'est pas connectée";
        }
    }
    return o;
    return o;
}
