// #ifndef _MACHINE_
// #define _MACHINE_

// #include <iostream>
// #include <vector>
// #include "Packet.hpp"
// #include "Interface.hpp"
// #include "Link.hpp"

// class Machine {
// protected:
//     char _label;
//     interface_t _nb_interface = 0;
//     std::vector<Basic_Interface*> _interfaces = {};

// public:
//     static int total;
//     static bool connect(Machine& machine1, Machine& machine2, interface_t interface1, interface_t interface2);

//     Machine(std::vector<Basic_Interface*> interfaces);
    
//     bool as_interface(interface_t interface) 
//         { return (interface < _nb_interface) ? true : false; }
//     interface_t nb_interface() { return _nb_interface; }
//     virtual Link* interface(interface_t interface);
//     virtual void forward(Packet&) {};
//     virtual void receipt(Packet&, interface_t) {};
//     bool is_interface_connected(interface_t interface) 
//         { return (this->interface(interface) == nullptr) ? false : true; }
//     std::vector<Link*> connected_interfaces();
//     // DEBUG
//     // Link* link_at_interface(interface_t interface) { return as_interface(interface) ? _linked_to[interface]->get_link() : NULL; }
//     char get_label() { return _label; }
//     void set_label(char s) { _label = s; }

//     Link* operator[](interface_t interface){ return this->interface(interface); }

//     virtual ~Machine() noexcept = default;

//     friend std::ostream& operator << (std::ostream& o, const Machine& M);
// };

// #endif