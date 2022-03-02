# Peton - Simulateur de réseau informatique

Petite application permettant d'émuler un réseau informatique, de créer des paquets de différents type et de les envoyer depuis une machine vers une autre.

## Fonctionnalités principales

### Machine
Une machine est représentée par un *corps* (la machine en elle même qui peut exécuter des processus (cf. Applications)) et une ou plusieurs *interface* (qui permet la connexion avec d'autres machines (cf. Interfaces)).

La classe `Machine_Factory` permet la création de machine.
Il est possible d'assigner une IP manuellement à notre machine, mais en faisant cela il ne faut pas oublier d'ajouter une entrée *default* dans la table de routage.
```cpp
machine->get_routing_table().add_in_table(subnet, CIDR, metric, interface, gateway);
```
Sinon, si un serveur DHCP tourne sur une machine du sous-réseau il est possible d'obtenir une adresse IP et une entrée dans la table de routage en utilisant la méthode `runDHCP(interface)`.

### Interfaces
Les seules interfaces disponibles sont les interfaces *Ethernet* (ethX). Une interface est composée d'une adresse IP, d'un CIDR (masque sous-réseau) et d'une adresse MAC. Les interfaces sont connectées entre elles par des *liens* (cf. Liens).

### Liens
Un lien représente la couche physique du modèle TCP/IP, ils ne font qu'envoyer les paquets d'une interface à l'autre.

### Paquet
Le paquet contient l'information qui transite sur le réseau. Comme des matroschka, un paquet en contient un autre plus petit qui sera ouvert par la couche TCP/IP qui sait le lire.
Les protocoles disponibles sont les suivants
Couche | Protocole |
--- | --- |
Liaison de données | Ethernet
Liaison de données | ARP
Internet | ICMP
Internet | IP
Transport | UDP
Application | DHCP

La classe `Packet_Factory` permet la création de paquet.

### Application
Une machine peut faire tourner une ou plusieurs application.s qui utiliseront les paquets reçus depuis leurs socket pour effectuer des actions. Actuellement la seule application disponible est un serveur/client DHCP.
La méthode `add_application(Application, port)` permet d'assigner une application à une machine.

## Compilation
```sh
cmake .
make
```

Il est possible d'ajouter le flag -DDEBUG=1 pour avoir plus d'infos lors de l'éxécution du programme

## Exemple

L'exemple dans le fichier `./main.cpp` est le suivant

```
+--------+                +--------+                +--------+
|        |   10.0.0.0/24  |        |   10.0.1.0/24  |        |
|    B   |----------------|    A   |----------------|    C   |
|        |                |        |                |        |
+--------+                +--------+                +--------+
 10.0.0.3             10.0.0.1  10.0.1.1             10.0.1.2
                       (eth0)    (eth1)
```
La machine A exécute un serveur DHCP pour le sous-réseau 10.0.1.0/24.

L'IP de la machine B a été définie manuellement.

L'IP de la machine C a été obtenue grâce au protocole DHCP.

Dans l'exemple: C envoie un paquet ICMP (ping) vers B.

## Faronti
 - [x] UDP
 - [x] DHCP (Permet une connexion plus Plug&Play entre les machines)
 - [ ] DNS
 - [ ] TCP
 - [ ] HTTP
