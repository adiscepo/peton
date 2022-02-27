cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(ROGGEMAN_FLAGS "-fcoroutines -fconcepts -pthread -fopenacc -fopenmp -ggdb3 -Wpedantic -Wall -Wextra -Wconversion -Wsign-conversion -Weffc++ -Wstrict-null-sentinel -Wold-style-cast -Wnoexcept -Wctor-dtor-privacy -Woverloaded-virtual -Wsign-promo -Wzero-as-null-pointer-constant -Wsuggest-override")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROGGEMAN_FLAGS}")

# Nom du projet
project(Peton)

# Fichiers utilisés
add_executable(
    peton 
    main.cpp
    Machine.hpp
    Machine.cpp
    Applications/Application.cpp
    Applications/Application.hpp
    Applications/DHCP_Application.cpp
    Applications/DHCP_Application.hpp
    IP_Machine.hpp
    IP_Machine.cpp
    Application_Machine.hpp
    Application_Machine.cpp
    Packet.hpp
    Packet.cpp
    Link.hpp
    Link.cpp
    Interface.hpp
    Interface.cpp
    types.hpp
    types.cpp
)

# set(ignoreMe "${DEBUG}")

if (DEFINED DEBUG)
    add_definitions(-DDEBUG)
    # sets the name of the executable to "ltype_game_server"
endif ()

# set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "peton")
