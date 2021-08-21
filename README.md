# webserver

[![Doriane's 42Project Score](https://badge42.herokuapp.com/api/project/dmontgen/webserv)](https://github.com/JaeSeoKim/badge42)

42 project - Recoder un server HTTP en C++

Team : [dmontgen](https://github.com/ejawe/) [nmbabazi](https://github.com/nmbabazi/) 

**Methodes implémentées** GET PUT POST DELETE

Utilisation de Ngninx pour comparer les en-tête, comportement de réponse et fichier de configuration.

**HTTP** Hypertext Transfert Protocol : Protocole d'application pour les système d'information distribués, collaboratif et hypermédia.
Fondement de la communication de données pour le WWW. Sa fonction principale est de servir et recevoir du contenu des clients.

**RFC** Request For Comment : Série numérotée de documents décrivant les aspect et spécifications technique d'internet.

**HEADERS** Les champs d'en-tête HTTP sont des composants de la section d'en-tête des requêtes et réponses dans le protocole HTTP.
Ils définisent les paramètres de fonctionnement d'une transaction. 

**TCP** Transmission Control Protocol : protocole de transport des fux de données sur une connexion réseau.

**SOKET** Flux de communication entre deux objets client/server. 
Mécanisme fourni par la plupart des système d'exploitation pour permettre aux programmes d'accéder au réseau.
Créé pour être indépendant à tous types de réseaux. 
Par défaut, les opération sur les sockets TCP sont bloquante. La fonction *accept* bloquera le flux d'exécution tant qu'un client ne s'est pas connecté. Il va attendre que l'opération de lecture sur la socket soit complétée.
*fcntl* dit au noyau de ne pas mettre le process en sleep si l'opération E/S n'est pas terminé, mais de renvoyer l'erreur EWOULDBLOCK à la place.

**POLL** permet à l'application de multiplexer les E/S sur un enssemble de fd. Permet au processus d'attendre qu'un événement se produise et de réveiller le processus lorsque l'événement se produit. poll va gérer un set de fd et attendre un évènement sur un fd. Débloque le processus quand le fd est prêt à effectuer des E/S.

**CGI** Common Gateway Interface : Interface de passerelle commune utilisée par les serveur HTTP. Au lieu d'envoyer le contenu d'un fichier (html, image...) le serveur HTTP exécute un programme puis retourne le contenu généré. CGI est un protocole qui indique comment transmettre la requête du server HTTP au programme et récupérer la réponse générée. Il faut configurer le serveur HTTP pour associer l'exécution du programme CGI à certaine URL.

**run** 
---
`make && ./webserv configfiles/simple_server.conf`

http://localhost:8085/

### Ressources
* https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa
* https://www.tutorialspoint.com/http/http_overview.htm
* https://web.maths.unsw.edu.au/~lafaye/CCM/sockets/sockfonc.htm
* https://www.ibm.com/docs/en/i/7.2?topic=designs-using-poll-instead-select
* https://stackoverflow.com/questions/5106674/error-address-already-in-use-while-binding-socket-with-address-but-the-port-num
* https://www.scottklement.com/rpg/socktut/nonblocking.html
* https://broux.developpez.com/articles/c/sockets/