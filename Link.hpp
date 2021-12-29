#ifndef _LINK_
#define _LINK_

#include "Packet.hpp"
class Basic_Interface;

/* Connecte deux machines entre elles
 * Permet de monitorer le trafic sur la ligne
 */

class Link {
private:
    Basic_Interface* _connected[2];
public:
    // inline Link() = default;
    Link(Basic_Interface& m1, Basic_Interface& m2) : _connected() 
        { _connected[0] = &m1; _connected[1] = &m2; }
    
    void forward(Packet& packet, Basic_Interface& from);
    
    interface_t get_interface_of(const Basic_Interface& m);
    Basic_Interface* get_interface_connected_to(const Basic_Interface& m);
    
    ~Link() noexcept = default;

    friend std::ostream& operator << (std::ostream& o, const Link& L)
        {o << "lien entre " << L._connected[0] << " et " << L._connected[1]; return o;};
    // DEBUG    
    void modifyMachine(interface_t i, char s);
};

#endif