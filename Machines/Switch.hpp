#ifndef _SWITCH_
#define _SWITCH_

#include "Machine.hpp"

class Switch : public Machine {
    CAM_Table _cam_table;
    size_t _number_of_interfaces;
    std::vector<MAC> _waiting; // Paquet dont on n'a pas d'association MAC <-> interface, on avait donc envoyé le paquet vers toutes les interfaces et maintenant on attend un paquet de retour depuis une interface pour savoir laquelle était la bonne.
public:
    Switch(size_t number_of_interfaces);
    void send(Packet& P) override;
    void receipt(Packet& p, interface_t from_interface) override;
    ~Switch() = default;

    friend std::ostream& operator << (std::ostream& o, const Switch& S);

};

#endif // _SWITCH_