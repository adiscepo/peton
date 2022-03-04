#ifndef _APPLICATION_DNS_APPLICATION_
#define _APPLICATION_DNS_APPLICATION_
#include "Application.hpp"

typedef struct DNS_content {
    std::string hostname; // or char[63]
    std::string url;      // or char[254]
    Packet::DNS::DNS_Type type;
    DNS_content(std::string hostname, std::string url, Packet::DNS::DNS_Type type) : hostname(hostname), url(url), type(type) {};
} DNS_content;

class DNS_Application : public Application {
    std::vector<DNS_content> _content;
    std::string _domain_name;
    IPv4 _next_dns_server;
public:
    DNS_Application(IP_Machine& machine) : Application(machine) { Application::_type = "DNS Application"; }
    DNS_Application(IP_Machine& machine, std::string domain_name, IPv4 next_dns_server) : Application(machine), _domain_name(domain_name), _next_dns_server(next_dns_server) 
        { Application::_type = "DNS Application"; }
    void add_entry(DNS_content entry) { _content.push_back(entry); }
    void configure(std::string domain_name, IPv4 _next_dns_server) { _domain_name = domain_name; _next_dns_server = _next_dns_server; }
    void socket(Packet& P, interface_t from_interface) override { std::cout << "[DNS Application] Paquet reçu" << std::endl; };
    void resolve(std::string hostname);
};

#endif // _APPLICATION_DNS_APPLICATION_

/*
Format BIND
Domain Name space
Nom donné à l'arbre,  il a une racine unique, profondeur maximale de 127 niveaux (x.y.za.b.xc.qsd.... (127 fois))
Chaque noeud peut avoir seulement 64 char

DOMAINE = SOUS-ARBRE DU DOMAINE NAME SPACE
TLD (Top level domain) = first-level domain
Resource Record (RR) = Enregistrement DNS = données associées à un domain name
Mapping hostname - IPv4 A
Serveur de messagerie pour un domaine ou une machine - MX
mappings IP -> hostname "CNAME"
Gestion par l'ICANN
Zone != domaine
Découpe d'un domaine en zone, chaque zone correspond à un ou plusieurs serveurs
Serveur primaire = serveur principal, lit les données de la zone dans un fichier local
secondaire = Jumeau du serveur principal
esclaves = cache des valeurs du serveur primaire

Il y'a 13 serveurs racine qui savent où se trouvent tous les TLDs
Serveur de nom du provider va effectuer les requêtes DNS aux différents serveurs Authoritative du DNS
*/