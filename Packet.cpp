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
    Packet* ethernet;
    Packet* ip = new Packet();
    auto m = from.get_machine();
    ip->type = Packet::Type::IP;
    ip->data.ip = Packet::IP(protocol, from.get_ip(), ip_dest, payload);
    if (m->get_arp_table().is_IP_in(ip_dest)){ // La machine a l'adresse
        LOG(from.get_machine()->get_label(), "J'ai l'entrée dans ma table ARP")
        ethernet = Packet_Factory::ETHERNET(from.get_mac(), 
                                            m->get_arp_table().from_IP(ip_dest), 
                                            Packet::ETHERNET::EtherType::IP, 
                                            *ip);
    }else{
        LOG(from.get_machine()->get_label(), "J'ai que dalle")
        Packet* p = Packet_Factory::ARP(*from.get_machine(), Packet::ARP::ARP_Opcode::REQUEST, from.get_mac(), from.get_ip(), {}, ip_dest);
        from.send(*p); // Va mettre à jour la table ARP de machine from
        if (m->get_arp_table().is_IP_in(ip_dest)){
            LOG(from.get_machine()->get_label(), "J'ai l'entrée dans ma table ARP II")
            ethernet = Packet_Factory::ETHERNET(from.get_mac(), 
                                                m->get_arp_table().from_IP(ip_dest), 
                                                Packet::ETHERNET::EtherType::IP, 
                                                *ip);
        }else{
            // On devrait pas se trouver ici normalement
            LOG(from.get_machine()->get_label(), "[PROBLEM] Ok là je viens d'envoier une req ARP et ma table ne s'est pas mise à jour, je sais pas")
        }
    }
    return ethernet;
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
        << "| IP dest  |  ";
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

std::ostream& operator<< (std::ostream& o, const Packet::IP& P) {
    o << "+----------+-----------IP----------+" << std::endl
        << "| Version  |  " << ((P.version == Packet::IP::IP_Version::IPv4) ? 4 : 6) << "                    |" << std::endl
        << "| IP src   |  ";
    std::cout.setf(std::ios::left, std::ios::adjustfield);
    std::cout.width(21);
    o << IP_Machine::IPv42char(P.src);
    o << "|" << std::endl
        << "| IP dest  |  ";
    std::cout.setf(std::ios::left, std::ios::adjustfield);
    std::cout.width(21);
    o << IP_Machine::IPv42char(P.dest);
    o << "|";
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