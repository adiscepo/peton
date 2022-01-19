# Peton - Simulateur de réseau informatique

Permet l'émulation d'un réseau informatique.

L'exemple dans le fichier main est le suivant

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
[] DHCP