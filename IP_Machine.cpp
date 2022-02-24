#include "IP_Machine.hpp"
#include "Packet.hpp"
#include <bitset>
#include "Link.hpp"
#include "Interface.hpp"
// #include "Interface.cpp" // Besoin pour avoir accès à la méthode statique

// Méthodes statiques

int IP_Machine::total = 0;

// Eblas konekti inter du maŝinoj per iliajn interfacojn 
bool IP_Machine::connect(IP_Machine& machine1, IP_Machine& machine2, interface_t interface1, interface_t interface2){
    if (machine1.as_interface(interface1) && machine2.as_interface(interface2)) {
        Basic_Interface::connect(*(machine1._interfaces[interface1]), *(machine2._interfaces[interface2]));
        return true;
    }
    return false;
}

// Transformas la ĉenkaraktero el IP adreso
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


// Transformas la IP adreso en ĉenkaraktero
std::string IP_Machine::IPv42char(IPv4 ip) {
    std::string res;
    for (size_t i = 0; i < 4; i++) {
        res += std::to_string((ip >> ((3-i)*8)) & ((0b11111111)));
        if (i != 3) res += ".";
    }
    return res;
}

// Transformas la ĉenkaraktero en MAC adreso
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

// Transformas la MAC adreso en ĉenkaraktero
std::string IP_Machine::MAC2char(MAC mac) {
    std::string res;
    char* r = new char;
    for (size_t i = 0; i < 12; i++) {
        sprintf(r, "%lX", (mac.x >> ((11-i)*4)) & ((0b1111)));
        res += r;
        if (i != 11 && i%2 == 1) res += ":";
    }
    return res;
}

// Méthodes

// Konstruemo
IP_Machine::IP_Machine(std::vector<Interface*> interfaces) : _label(static_cast<char>(65 + total)) 
    {for (auto elem: interfaces) _interfaces.insert({elem->get_interface_number(), elem}); IP_Machine::total += 1; };

void IP_Machine::set_interface(Interface& interface) {
    _interfaces[interface.get_interface_number()] = &interface;
}

void IP_Machine::set_interfaces(std::vector<Interface*> interfaces) {
    for (auto elem: interfaces) _interfaces.insert({elem->get_interface_number(), elem});
}

bool IP_Machine::as_ip(IPv4 ip) { 
    for (auto i: _interfaces) if (i.second->get_ip() == ip) return true; 
    return false;
}

bool IP_Machine::as_mac(MAC mac) { 
    for (auto i: _interfaces) if (i.second->get_mac() == mac) return true; 
    return false;
}

// Couche physique
void IP_Machine::physical_layer(Packet& P, interface_t from_interface) {
    // On passe la paquet directement à la couche liaison de données, il n'y a rien que l'on puisse faire ici (en tant qu'émulateur ce n'est pas nécéssaire)
    // DEBUG(P);
    std::cout << P << std::endl;
    action(P, from_interface);
}

// Couche liaison des données
void IP_Machine::datalink_layer(Packet& P, interface_t from_interface) {
    // Lorsqu'un paquet arrive sur le réseau, on se limite au contenu direct (trame ethernet)
    // Est-ce que le paquet nous est destiné ?
    // Si oui, je l'ouvre et passe à la couche suivante (internet)
    // Sinon est-ce que nous pouvons le transférer quelque part ? 
    if (P.type == Packet::Type::ETHERNET) { // Il s'agit d'un paquet Ethernet (y'a que ça ici)
        LOG(_label, "arrivé avec le protocole Ethernet")
        if (as_mac(P.data.ethernet.dest) || (P.data.ethernet.dest == MAC_BROADCAST && !as_mac(P.data.ethernet.src))) { // Le paquet m'est destiné: soit directement, soit par broadcast (on check que le broadcast ne provienne pas de nous)
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



// Envoie le paquet sur le lien, en fonction de l'adresse MAC de destination 
void IP_Machine::send(Packet& P) {
    MAC dest = P.data.ethernet.dest;
    if (_arp_table.is_MAC_in(dest)){
        LOG(_label, "envoie un paquet");
        _interfaces[_arp_table.get_interface(dest)]->send(P);
    }else if (dest == MAC_BROADCAST){
        for (auto i: _interfaces) i.second->send(P); // On envoie le paquet par l'interface
    }else{
        LOG(_label, "n'envoie pas un paquet");
    }
};

void IP_Machine::receipt(Packet& P, interface_t from_interface) {
    LOG(_label, "reçoit un paquet."); 
    physical_layer(P, from_interface);
};

void IP_Machine::ARP_action(Packet& P, interface_t from_interface) {
    LOG(_label, "le paquet contient un header ARP");
    // Est-ce que c'est mon adresse MAC que l'on veut ? On le sait en regardant l'IP de destination
    if (as_ip(P.data.arp.ip_target)){
        if (P.data.arp.opcode == Packet::ARP::ARP_Opcode::REQUEST){
            LOG(_label, "Oui c'est bien moi qu'on cherche !");
            // Répond à la requête par un nouveau paquet ARP
            // Ajoute les infos du paquet que l'on vient de recevoir dans la table ARP de la machine
            _arp_table.add_in_table(P.data.arp.mac_sender, P.data.arp.ip_sender, from_interface);
            Packet* res = Packet_Factory::ARP(*this, Packet::ARP::ARP_Opcode::RESPONSE, interface(from_interface).get_mac(), interface(from_interface).get_ip(), P.data.arp.mac_sender, P.data.arp.ip_sender); 
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
}

void IP_Machine::IP_action(Packet& P, interface_t from_interface) {
    std::cout << IP_Machine::IPv42char(P.data.ip.src) << " a envoyé un paquet IP. On l'a reçu depuis l'interface eth" << from_interface << std::endl;
    if (as_ip(P.data.ip.dest)) { // Si le paquet est destiné à la machine
        if (P.data.ip.protocol == Packet::IP::IP_Protocol::ICMP) ICMP_action(P, from_interface);
        if (P.data.ip.protocol == Packet::IP::IP_Protocol::UDP) UDP_action(P, from_interface);
    }else{
        LOG(_label, "Ce paquet n'est pas pour moi, je le transfère")
        // TODO: Rempaqueter le paquet reçu et le transférer au prochain routeur
        // Forward du paquet
        IPv4 ip_dest = P.data.ip.dest;
        auto entries = _routing_table.longest_prefix(ip_dest);
        if (entries._metric == 0){ // Si l'hôte de destination que nous cherchons se trouve sur un sous-réseau auquel nous avons accès
            Interface* interface = _interfaces.at(entries._interface); 
            ip_route();
            std::cout << "La destination du paquet se trouve sur le sous-réseau " << IPv42char(entries._subnet) << " auquel je sais accéder depuis l'interface " << entries._interface << std::endl;
            // Forge le nouveau paquet ETHERNET
            if (!_arp_table.is_IP_in(ip_dest)) {
                auto req_arp = Packet_Factory::ARP(*this, Packet::ARP::ARP_Opcode::REQUEST, interface->get_mac(), interface->get_ip(), {}, ip_dest);
                send(*req_arp);
                DEBUG("Normalement on a l'adresse MAC")
                arp();
            }
            DEBUG(IPv42char(ip_dest))
            DEBUG(MAC2char(_arp_table.from_IP(ip_dest)))
            auto new_packet = Packet_Factory::ETHERNET(interface->get_mac(), _arp_table.from_IP(ip_dest), Packet::ETHERNET::EtherType::IP, P);
            send(*new_packet);
        }else{
            DEBUG("La destination ne se trouve pas dans un de mes sous-réseau, par contre je peux le transférer à l'interface qui elle pourra transmettre au sous-réseau " + IPv42char(entries._subnet))
        }
    }
}

void IP_Machine::ICMP_action(Packet& P, interface_t from_interface) {
    LOG(_label, "c'est un paquet ICMP")
    Packet* icmp = P.data.ip.payload;
    if (icmp->data.icmp.type == Packet::ICMP::ICMP_Type::ECHO_req) {
        Packet* res = Packet_Factory::ICMP(interface(from_interface), P.data.ip.src, Packet::ICMP::ICMP_Type::ECHO_res);
        send(*res);
    }else if (icmp->data.icmp.type == Packet::ICMP::ICMP_Type::ECHO_res){
        LOG(_label, "Pong");
    }
}

void IP_Machine::UDP_action(Packet& P, interface_t from_interface) {
    LOG(_label, "c'est un paquet UDP")
    Packet* udp = P.data.ip.payload;
    Packet* content = udp->data.udp.payload;
}

void IP_Machine::DHCP_action(Packet& P, interface_t from_interface) {
    LOG(_label, "c'est un paquet DHCP")
    // Packet* dhcp = P.data.
}

void IP_Machine::add_in_routing_table(IPv4 ip, CIDR mask, uint metric, interface_t interface){
    _routing_table.add_in_table(get_subnet(ip, mask), mask, metric, interface, IP_DEFAULT);
};

void IP_Machine::action(Packet& P, interface_t from_interface) {
    switch (P.type) {
    case Packet::Type::ETHERNET:
        datalink_layer(P, from_interface);
        break;
    case Packet::Type::ARP:
        ARP_action(P, from_interface);
        break;
    case Packet::Type::IP:
        IP_action(P, from_interface);
        break;
    case Packet::Type::UDP:
        UDP_action(P, from_interface);
        break;
    case Packet::Type::DHCP:
        DHCP_action(P, from_interface);
        break;
    default:
        break;
    }
}

void IP_Machine::arp() { 
    std::cout << "+--------------------------------------------------+" << std::endl;
    std::cout << "|                   TABLE ARP de " << _label << "                 |" << std::endl;
    std::cout << _arp_table << std::endl; 
};

void IP_Machine::ip_route(){
    std::cout << "+-----------------------------------------------------------------+" << std::endl;
    std::cout << "|                     TABLE DE ROUTAGE de " << _label << "                       |" << std::endl;
    std::cout << _routing_table << std::endl; 
}

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
}
