#include "types.hpp"
#include "IP_Machine.hpp"

#include <bitset>

bool ARP_Table::is_MAC_in(MAC mac) {
    for (auto elem: _content){
        if (elem.mac == mac) return true;
    }
    return false;
}

bool ARP_Table::is_IP_in(IPv4 ip) {
    for (auto elem: _content){
        if (elem.ip == ip) return true;
    }
    return false;
}

MAC ARP_Table::from_IP(IPv4 ip) {
    for (auto elem: _content){
        if (elem.ip == ip) return elem.mac;
    }
    return MAC_ERROR;
}

IPv4 ARP_Table::from_MAC(MAC mac) {
    for (auto elem: _content){
        if (elem.mac == mac) return elem.ip;
    }
    return IPv4{};
}

bool ARP_Table::add_in_table(MAC mac, IPv4 ip, interface_t interface) {
    if (!is_MAC_in(mac)){
        _content.push_back(Entries{ip, mac, interface});
        return true;
    }
    return false; // Il y'a déjà une entrée dans la table ARPs
}

ARP_Table::Entries ARP_Table::get_content(MAC mac) {
    if (is_MAC_in(mac)){
        for (auto elem: _content) {
            if (elem.mac == mac) return elem;
        }
    }
    return {};
};

ARP_Table::Entries ARP_Table::get_content(IPv4 ip) {
    if (is_IP_in(ip)){
        for (auto elem: _content) {
            if (elem.ip == ip) return elem;
        }
    }
    return {};
};

std::ostream& operator<<(std::ostream& o, const ARP_Table& R){
    o << "+--------------------------------------------------+" << std::endl;
    o << "|                     TABLE ARP                    |" << std::endl;
    o << "+-------------------+------------------+-----------+" << std::endl;
    o << "|        MAC        |        IP        | interface |" << std::endl;
    o << "+-------------------+------------------+-----------+" << std::endl;
    auto table = R._content;
    for (size_t i = 0; i < R._content.size(); i++){
        o << "|";
        std::cout.setf(std::ios::right, std::ios::adjustfield);
        std::cout.width(18);
        o << IP_Machine::MAC2char(table[i].mac);
        o << " |";
        std::cout.setf(std::ios::right, std::ios::adjustfield);
        std::cout.width(17);
        o << IP_Machine::IPv42char(table[i].ip);
        o << " |";
        std::cout.setf(std::ios::right, std::ios::adjustfield);
        std::cout.width(10);
        o << table[i].interface;
        o << " |" << std::endl;
    }
    if(R._content.size() == 0) {
        o << "|                   Aucune entrée                  |" << std::endl;
    }
    o << "+--------------------------------------------------+" << std::endl;
    return o;
}


bool Routing_Table::add_in_table(IPv4 subnet, CIDR mask, uint metric, interface_t interface, IP_Machine* next_hop_router){
    if (!is_IP_in(subnet)){
        _content.push_back({subnet, mask, metric, interface, next_hop_router});
        return true;
    }
    return false;
}

bool Routing_Table::is_IP_in(IPv4 ip) {
    for (auto elem: _content) if (elem._subnet == ip) return true;
    return false;
}

Routing_Table::Entries Routing_Table::longest_prefix(IPv4 ip) {
    /*  1. prendre chaque entrée e de la table de routage 
     *  2. mettre tous les bits de la partie host à 0 de e grâce au masque de e,
     *  3. faire un & entre l'ip qu'on cherche et l'ip où vient de retirer les bits de la partie host (ip trouvée = x)
     *  4. compter les bits à 1 de x. 
     *  L'ip de longest prefix est celle qui donne le plus grand nombre de bit à 1
     */
    Entries res;
    // __builtin_popcount(x) donne le nombre de bit à 1 dans x
    for (auto elem : _content) if (__builtin_popcount(elem._subnet&ip) > __builtin_popcount(res._subnet)) res = elem; 
    return res;
}

std::ostream& operator<<(std::ostream& o, const Routing_Table& R){
    o << "+-----------------------------------------------------------------+" << std::endl;
    o << "|                         TABLE DE ROUTAGE                        |" << std::endl;
    o << "+-----------------+------+--------+-----------+-------------------+" << std::endl;
    o << "|       SUBNET    | CIDR | Metric | Interface |  Next Hop Router  |" << std::endl;
    o << "+-----------------+------+--------+-----------+-------------------+" << std::endl;
    for (auto entries: R._content) {
        o << "|";
        std::cout.setf(std::ios::right, std::ios::adjustfield);
        std::cout.width(16);
        o << IP_Machine::IPv42char(entries._subnet); 
        o << " |";
        std::cout.setf(std::ios::right, std::ios::adjustfield);
        std::cout.width(5);
        o << entries._mask.x;
        o << " |"; 
        std::cout.setf(std::ios::right, std::ios::adjustfield);
        std::cout.width(7);
        o << entries._metric;
        o << " |"; 
        std::cout.setf(std::ios::right, std::ios::adjustfield);
        std::cout.width(10);
        o << entries._interface;
        o << " |";
        std::cout.setf(std::ios::right, std::ios::adjustfield);
        std::cout.width(18);
        o << "TODO";
        o <<  " |" << std::endl;
    }
    if(R._content.size() == 0) o << "|                             Aucune entrée                           |" << std::endl;
    o << "+-----------------------------------------------------------------+" << std::endl;
    return o;
};