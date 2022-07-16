#include "Machine.hpp"

struct MachineDoNotHaveInterface : public std::exception { const char * what () const throw () { return "The interface you try to attain is not set on this Machine"; } };

int Machine::total = 0;

bool Machine::connect(Machine& machine1, Machine& machine2, interface_t interface1, interface_t interface2){
    if (machine1.as_interface(interface1) && machine2.as_interface(interface2)) {
        Basic_Interface::connect(*(machine1._interfaces[interface1]), *(machine2._interfaces[interface2]));
        return true;
    }
    throw MachineDoNotHaveInterface();
    return false;
}

void Machine::set_interfaces(std::vector<Basic_Interface*> interfaces) {
    for (auto elem: interfaces) _interfaces.insert({elem->get_interface_number(), elem});
}


void Machine::set_interfaces(std::vector<Interface*> interfaces) {
    for (auto elem: interfaces) _interfaces.insert({elem->get_interface_number(), elem});
}

void Machine::set_interface(Basic_Interface& interface) {
    _interfaces[interface.get_interface_number()] = &interface;
}

void Machine::set_interface(Interface& interface) {
    _interfaces[interface.get_interface_number()] = &interface;
}
