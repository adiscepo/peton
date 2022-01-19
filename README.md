# Peton - Simulateur de réseau informatique

Permet l'émulation d'un réseau informatique.

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
 10.0.0.2             10.0.0.1  10.0.1.1             10.0.1.4 
                       (eth0)    (eth1)
```
Ping de B depuis C.
/!\ Les connexions et les tables de routages sont effectuées manuellement /!\

Faronti
 * UDP
 * DHCP (Permet une connexion plus Plug&Play entre les machines)
 * DNS
 * TCP
 * HTTP