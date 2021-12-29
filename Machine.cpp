// #include "Machine.hpp"

// // Static elements

// int Machine::total = 0;

// bool Machine::connect(Machine& machine1, Machine& machine2, interface_t interface1, interface_t interface2){
//     if (machine1.as_interface(interface1) && machine2.as_interface(interface2)) {
//         Link *link = new Link(machine1, machine2, interface1, interface2);
//         machine1._linked_to[interface1]->set_link(link);
//         machine2._linked_to[interface2]->set_link(link);
//         return true;
//     }
//     return false;
// }

// // Methods

// Machine::Machine(std::vector<Basic_Interface*> interfaces) :  _label(static_cast<char>(65 + total)), 
//                                                               _nb_interface(interfaces.size()), 
//                                                               _interfaces(interfaces) {
//     Machine::total += 1;
// }

// Link* Machine::interface(interface_t interface) {
//     if (interface < _nb_interface){
//         if (_linked_to[interface] == nullptr) return nullptr;
//         else return _linked_to[interface]->get_link();
//     }
//     return nullptr;
// }

// std::vector<Link*> Machine::connected_interfaces() {
//     std::vector<Link*> res;
//     for (interface_t i = 0; i < _nb_interface; i++) {
//         if (is_interface_connected(i)) res.push_back(_linked_to[i]->get_link());
//     }
//     return res;
// };

// // Friend
// std::ostream& operator << (std::ostream& o, const Machine& M){
//     o << "La machine " << M._label << " a " << M._nb_interface << " interfaces:"; 
//     for (std::size_t i = 0; i < M._nb_interface; i++) {
//         if (M._linked_to.at(i) != nullptr) {
//             o << "\n - eth" << i << " connectée à " << M._linked_to.at(i)->get_link()->get_machine_connected_to(M)->_label;
//         }else{
//             o << "\n - eth" << i << " n'est pas connectée";
//         }
//     }
//     return o;
// }