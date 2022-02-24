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

Packet* Packet_Factory::UDP(Interface& from, IPv4 ip_dest, port_t src, port_t dest, Packet* payload) {
    Packet* udp = new Packet();
    udp->type = Packet::Type::UDP;
    udp->data.udp = Packet::UDP(src, dest, payload);
    return Packet_Factory::IP(from, Packet::IP::IP_Protocol::UDP, ip_dest, udp);
}

Packet* Packet_Factory::ICMP(Interface& from, IPv4 ip_dest, Packet::ICMP::ICMP_Type type) {
    Packet* icmp = new Packet();
    icmp->type = Packet::Type::ICMP;
    icmp->data.icmp = Packet::ICMP(type);
    if (type == Packet::ICMP::ICMP_Type::ECHO_req) LOG(from.get_machine()->get_label(), "Ping");
    return Packet_Factory::IP(from, Packet::IP::IP_Protocol::ICMP, ip_dest, icmp);
};

Packet* Packet_Factory::DHCP(Interface& from, Packet::DHCP::DHCP_Message_Type type, IPv4 C, IPv4 Y, IPv4 S, IPv4 G, MAC M) {
    // Informations du paquet
    port_t p_src, p_dest;
    IPv4 ip_src = IP_Machine::char2IPv4("0.0.0.0");
    IPv4 ip_dest = IP_Machine::char2IPv4("255.255.255.255");
    MAC mac_src = from.get_mac();
    MAC mac_dest = MAC_BROADCAST;
    if (type == Packet::DHCP::DHCP_Message_Type::Discover || type == Packet::DHCP::DHCP_Message_Type::Request) { p_src = 67; p_dest = 68; } 
    if (type == Packet::DHCP::DHCP_Message_Type::Offer || type == Packet::DHCP::DHCP_Message_Type::ACK) { p_src = 67; p_dest = 68; mac_dest = M; ip_src = from.get_ip(); } 
    // Création du paquet 
    Packet* dhcp = new Packet();
    dhcp->type = Packet::Type::DHCP;
    dhcp->data.dhcp = Packet::DHCP(type, C, Y, S, G, M);
    Packet* udp = new Packet();
    udp->type = Packet::Type::UDP;
    udp->data.udp.payload = dhcp;
    udp->data.udp.src = p_src;
    udp->data.udp.dest = p_dest;
    Packet* ip = new Packet();
    ip->type = Packet::Type::IP;
    ip->data.ip.src  = ip_src;
    ip->data.ip.dest = ip_dest;
    ip->data.ip.protocol = Packet::IP::IP_Protocol::UDP;
    ip->data.ip.payload = udp;
    Packet* ethernet = new Packet();
    ethernet->type = Packet::Type::ETHERNET;
    ethernet->data.ethernet.type = Packet::ETHERNET::EtherType::IP; 
    ethernet->data.ethernet.src = mac_src; 
    ethernet->data.ethernet.dest = mac_dest;
    ethernet->data.ethernet.payload = ip;
    return ethernet;
};


std::ostream& operator<< (std::ostream& o, const Packet::ETHERNET& P) {
    o << "+----------+-------Ethernet--------+" << std::endl
      << "| MAC src  |  " << IP_Machine::MAC2char(P.src)  << "    |" << std::endl
      << "| MAC dest |  " << IP_Machine::MAC2char(P.dest) << "    |" << std::endl
      << "| EtherType|  " << ((P.type == Packet::ETHERNET::EtherType::IP) ? "IP  " : "ARP ") << "                 |" << std::endl;
      if (P.type == Packet::ETHERNET::EtherType::ARP) o << P.payload->data.arp;
      else if (P.type == Packet::ETHERNET::EtherType::IP) o << P.payload->data.ip;
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
    o << IP_Machine::IPv42char(P.ip_target) << "|" << std::endl;
    return o;
}


std::ostream& operator<< (std::ostream& o, const Packet::ICMP& I) {
    o << "+----------+---------ICMP----------+" << std::endl
      << "|  Type    |  ";
      std::cout.setf(std::ios::left, std::ios::adjustfield);
      std::cout.width(23);
    if (I.type == Packet::ICMP::ICMP_Type::ECHO_req) o << "Requête d'écho";
    if (I.type == Packet::ICMP::ICMP_Type::ECHO_res) o << "Réponse d'écho";
    o << "|" << std::endl;
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
    else if (P.protocol == Packet::IP::IP_Protocol::UDP) o << P.payload->data.udp;
    return o;
}

std::ostream& operator<< (std::ostream& o, const Packet::UDP& P) {
    o << "+----------+-----------UDP---------+" << std::endl;
    o << "| Port src |  ";
    std::cout.setf(std::ios::left, std::ios::adjustfield);
    std::cout.width(21);
    o << P.src;
    o << "|" << std::endl
      << "| Port dst |  ";
    std::cout.setf(std::ios::left, std::ios::adjustfield);
    std::cout.width(21);
    o << P.dest;
    o << "|" << std::endl;
    if (P.payload->type == Packet::Type::DHCP) o << P.payload->data.dhcp;
    return o;
}

std::ostream& operator<< (std::ostream& o, const Packet::DHCP& P) {
    o << "+----------+----------DHCP---------+" << std::endl;
    o << "|   Type   |  ";
    std::cout.setf(std::ios::left, std::ios::adjustfield);
    std::cout.width(21);
    if (P.message_type == Packet::DHCP::DHCP_Message_Type::Discover) o <<  "Discover";
    else if (P.message_type == Packet::DHCP::DHCP_Message_Type::Request) o <<  "Request";
    else if (P.message_type == Packet::DHCP::DHCP_Message_Type::Offer) o <<  "Offer";
    else if (P.message_type == Packet::DHCP::DHCP_Message_Type::Decline) o <<  "Decline";
    else if (P.message_type == Packet::DHCP::DHCP_Message_Type::Release) o <<  "Release";
    else if (P.message_type == Packet::DHCP::DHCP_Message_Type::ACK) o <<  "ACK";
    else if (P.message_type == Packet::DHCP::DHCP_Message_Type::NAK) o <<  "NAK";
    else if (P.message_type == Packet::DHCP::DHCP_Message_Type::Inform) o <<  "Inform";
    o << "|" << std::endl;
    o << "|  CiAddr  |  ";
    std::cout.setf(std::ios::left, std::ios::adjustfield);
    std::cout.width(21);
    o << P.CIADDR;
    o << "|" << std::endl;
    o << "|  YiAddr  |  ";
    std::cout.setf(std::ios::left, std::ios::adjustfield);
    std::cout.width(21);
    o << P.YIADDR;
    o << "|" << std::endl;
    o << "|  SiAddr  |  ";
    std::cout.setf(std::ios::left, std::ios::adjustfield);
    std::cout.width(21);
    o << P.SIADDR;
    o << "|" << std::endl;
    o << "|  GiAddr  |  ";
    std::cout.setf(std::ios::left, std::ios::adjustfield);
    std::cout.width(21);
    o << P.GIADDR;
    o << "|" << std::endl;
    o << "|  ChAddr  |  ";
    std::cout.setf(std::ios::left, std::ios::adjustfield);
    std::cout.width(21);
    o << IP_Machine::MAC2char(P.CHADDR);
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