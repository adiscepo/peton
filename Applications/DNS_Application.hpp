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
};

#endif // _APPLICATION_DNS_APPLICATION_