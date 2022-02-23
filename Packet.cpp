#include "Packet.hpp"
#include "IP_Machine.hpp"
#include "Interface.hpp"

/* Décrémente le TTL du paquet et le détruit lorsqu'il arrive à 0 */
bool Packet::control_TTL(Packet& P) {
    if (P.type == Type::IP) {
        P.data.ip.ttl -= 1;
        if (P.data.ip.ttl == 0) {
            P.~Packet();
            DEBUG("Paquet a écoulé son TTL : on le détruit");
            return true;
        }
    }
    return false;
}

Packet* Packet_Factory::ETHERNET(MAC mac_sender, MAC mac_target, Packet::ETHERNET::EtherType type, Packet& payload) {
    Packet* ethernet = new Packet();
    ethernet->type = Packet::Type::ETHERNET;
    ethernet->data.ethernet = Packet::ETHERNET(mac_sender, mac_target, type, &payload);
    return ethernet;
};


Packet* Packet_Factory::ARP(IP_Machine& from, Packet::ARP::ARP_Opcode opcode, MAC mac_sender, IPv4 ip_sender, MAC mac_target, IPv4 ip_target) {
    Packet* ethernet;
    Packet* arp = new Packet();
    arp->type = Packet::Type::ARP;
    if (opcode == Packet::ARP::ARP_Opcode::REQUEST){
        LOG(from.get_label(), "crée un paquet de requête ARP")
        arp->data.arp = Packet::ARP(opcode, mac_sender, ip_sender, MAC_ARP_REQUEST, ip_target);
        ethernet = Packet_Factory::ETHERNET(mac_sender, MAC_BROADCAST, Packet::ETHERNET::EtherType::ARP, *arp);
    }else{
        LOG(from.get_label(), "crée un paquet de réponse ARP")
        arp->data.arp = Packet::ARP(opcode, mac_sender, ip_sender, mac_target, ip_target);
        ethernet = Packet_Factory::ETHERNET(mac_sender, mac_target, Packet::ETHERNET::EtherType::ARP, *arp);
    }
    return ethernet;
};

Packet* Packet_Factory::IP(Interface& from, Packet::IP::IP_Protocol protocol, IPv4 ip_dest, Packet* payload) {
    using P = Packet::IP;
    Packet* ethernet;
    Packet* ip = new Packet();
    auto m = from.get_machine();
    ip->type = Packet::Type::IP;
    ip->data.ip = Packet::IP(protocol, from.get_ip(), ip_dest, payload);

    switch (protocol) {
    case P::IP_Protocol::ICMP:
        DEBUG("[Packet_Factory.IP] Content ICMP")
        break;
    case P::IP_Protocol::TCP:
        DEBUG("[Packet_Factory.IP] Content TCP")
        break;
    case P::IP_Protocol::EGP:
        DEBUG("[Packet_Factory.IP] Content EGP")
        break;
    case P::IP_Protocol::IGP:
        DEBUG("[Packet_Factory.IP] Content IGP")
        break;
    case P::IP_Protocol::UDP:
        DEBUG("[Packet_Factory.IP] Content UDP")
        break;
    case P::IP_Protocol::OSPF:
        DEBUG("[Packet_Factory.IP] Content OSPF")
        break;
    default:
        DEBUG("[PAcket_Factory.IP] Content is undefined")
        break;
    }

    ip_dest = m->get_routing_table().longest_prefix(ip_dest)._via; // On récupère l'ip de la machine nous permettant d'accéder au sous-réseau voulu
    
    LOG(from.get_machine()->get_label(), "Est-ce que je connais l'adresse MAC de " + IP_Machine::IPv42char(ip_dest) + " ?")
    if (m->get_arp_table().is_IP_in(ip_dest)){ // La machine a l'adresse
        LOG(from.get_machine()->get_label(), "Oui, j'ai l'entrée dans ma table ARP, son adresse MAC est " + IP_Machine::MAC2char(m->get_arp_table().from_IP(ip_dest)))
        return Packet_Factory::ETHERNET(from.get_mac(), 
                                            m->get_arp_table().from_IP(ip_dest), 
                                            Packet::ETHERNET::EtherType::IP, 
                                            *ip);
    }else{
        LOG(from.get_machine()->get_label(), "J'ai que dalle, j'interroge mes voisins pour savoir si quelqu'un a cet adresse IP sur mon sous-réseau (requête ARP)")
        Packet* p = Packet_Factory::ARP(*from.get_machine(), Packet::ARP::ARP_Opcode::REQUEST, from.get_mac(), from.get_ip(), {}, ip_dest);
        from.send(*p); // Va mettre à jour la table ARP de machine from
        if (m->get_arp_table().is_IP_in(ip_dest)){
            LOG(from.get_machine()->get_label(), "J'ai récupéré l'adresse MAC de " + IP_Machine::IPv42char(ip_dest) + " ! C'est " + IP_Machine::MAC2char(m->get_arp_table().from_IP(ip_dest)))
            return Packet_Factory::ETHERNET(from.get_mac(), 
                                                m->get_arp_table().from_IP(ip_dest), 
                                                Packet::ETHERNET::EtherType::IP, 
                                                *ip);
        }else{
            // On devrait pas se trouver ici normalement
            LOG(from.get_machine()->get_label(), "[PROBLEM] Ok là je viens d'envoier une req ARP et ma table ne s'est pas mise à jour, je sais pas")
        }
    }
    return nullptr;
};

// Packet* Packet_Factory::UDP(Interface& from, port_t src, port_t dest, P payload) {
    
// }

Packet* Packet_Factory::ICMP(Interface& from, IPv4 ip_dest, Packet::ICMP::ICMP_Type type) {
    Packet* icmp = new Packet();
    icmp->type = Packet::Type::ICMP;
    icmp->data.icmp = Packet::ICMP(type);
    if (type == Packet::ICMP::ICMP_Type::ECHO_req) LOG(from.get_machine()->get_label(), "Ping");
    return Packet_Factory::IP(from, Packet::IP::IP_Protocol::ICMP, ip_dest, icmp);
};


std::ostream& operator<< (std::ostream& o, const Packet::ETHERNET& P) {
    o << "+----------+-------Ethernet--------+" << std::endl
      << "| MAC src  |  " << IP_Machine::MAC2char(P.src)  << "    |" << std::endl
      << "| MAC dest |  " << IP_Machine::MAC2char(P.dest) << "    |" << std::endl
      << "| EtherType|  " << ((P.type == Packet::ETHERNET::EtherType::IP) ? "IP  " : "ARP ") << "                 |" << std::endl;
      if (P.type == Packet::ETHERNET::EtherType::ARP) o << P.payload->data.arp << std::endl;
      else if (P.type == Packet::ETHERNET::EtherType::IP) o << P.payload->data.ip << std::endl;
      o << "+----------+-----------------------+";
    return o;
}

std::ostream& operator<< (std::ostream& o, const Packet::ARP& P) {
    o << "+----------+----------ARP----------+" << std::endl
        << "| Opcode   |  " << ((P.opcode == Packet::ARP::ARP_Opcode::REQUEST) ? 1 : 2) << "                    |" << std::endl
        << "| MAC src  |  " << IP_Machine::MAC2char(P.mac_sender)  << "    |" << std::endl
        << "| IP src   |  ";
    std::cout.setf(std::ios::left, std::ios::adjustfield);
    std::cout.width(21);
    o << IP_Machine::IPv42char(P.ip_sender);
    o << "|" << std::endl
        << "| MAC dest |  " << IP_Machine::MAC2char(P.mac_target)  << "    |" << std::endl
        << "| IP dest  |  ";
    std::cout.setf(std::ios::left, std::ios::adjustfield);
    std::cout.width(21);
    o << IP_Machine::IPv42char(P.ip_target) << "|";
    return o;
}


std::ostream& operator<< (std::ostream& o, const Packet::ICMP& I) {
    o << "+----------+---------ICMP----------+" << std::endl
      << "|  Type    |  ";
      std::cout.setf(std::ios::left, std::ios::adjustfield);
      std::cout.width(23);
    if (I.type == Packet::ICMP::ICMP_Type::ECHO_req) o << "Requête d'écho";
    if (I.type == Packet::ICMP::ICMP_Type::ECHO_res) o << "Réponse d'écho";
    o << "|";
        return o;
}

std::ostream& operator<< (std::ostream& o, const Packet::IP& P) {
    o << "+----------+-----------IP----------+" << std::endl;
    o << "| Version  |  ";
    std::cout.setf(std::ios::left, std::ios::adjustfield);
    std::cout.width(21);
    o << ((P.version == Packet::IP::IP_Version::IPv4) ? 4 : 6) << "|" << std::endl;
    o << "| IP src   |  ";
    std::cout.setf(std::ios::left, std::ios::adjustfield);
    std::cout.width(21);
    o << IP_Machine::IPv42char(P.src);
    o << "|" << std::endl
        << "| IP dest  |  ";
    std::cout.setf(std::ios::left, std::ios::adjustfield);
    std::cout.width(21);
    o << IP_Machine::IPv42char(P.dest);
    o << "|" << std::endl;
    if (P.protocol == Packet::IP::IP_Protocol::ICMP) o << P.payload->data.icmp;
    return o;
}

std::ostream& operator<< (std::ostream& o, const Packet::UDP& P) {
    o << "+----------+-----------UDP---------+" << std::endl;
    o << "| Port src |  ";
    std::cout.setf(std::ios::left, std::ios::adjustfield);
    std::cout.width(21);
    o << IP_Machine::IPv42char(P.src);
    o << "|" << std::endl
      << "| Port dst |  ";
    std::cout.setf(std::ios::left, std::ios::adjustfield);
    std::cout.width(21);
    o << IP_Machine::IPv42char(P.dest);
    o << "|" << std::endl;
    return o;
}


// std::ostream& operator<< (std::ostream& o, const Packet& P) {
    // o << "+------------Paquet-----------+" << std::endl
    //   << "+---Type---+-------Value------+" << std::endl
    //   << "| MAC src  | " << IP_Machine::MAC2char(P._src) << "|"<< std::endl
    //   << "| MAC dest | " << IP_Machine::MAC2char(P._dest) << "|"<< std::endl
    //   << "| TTL      | " << P._ttl << "               |"<< std::endl
    //   << "| EtherType| " << ((P._type == Packet::ETHERTHYPE::IP) ? "IP  " : "ARP ") << "             |"<< std::endl
    //   << "+----------+------------------+";
    //   return o;
// };