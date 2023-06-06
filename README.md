# TCP-IP_Socket_windows
TCP/IP socket using C for Windows platform. Implemented communication functionality using socket programming for seamless data transmission.
Ce mini-projet porte sur le langage C avancé et met en œuvre les concepts de sockets et de structures de données.

Côté Client :
Se connecter au serveur socket en entrant votre nom d'utilisateur et en demandant un mot de passe.

Si le mot de passe est incorrect, vous avez jusqu'à 3 tentatives maximum, sinon le système sera arrêté.
Si le mot de passe est correct, le profil de l'utilisateur (Admin ou invité) sera récupéré.
Les noms d'utilisateur sont stockés dans un fichier texte côté serveur.

Côté Serveur :
Être prêt à recevoir les connexions des clients.

Vérifier l'authentification des clients (nom d'utilisateur, mot de passe).

Si l'utilisateur est l'administrateur, le menu de l'administrateur sera affiché.
Sinon, l'invité ne peut que consulter et rechercher les contacts. Le menu est personnalisé en fonction du profil de l'utilisateur.
Afficher l'historique instantané des clients.

Gérer les contacts : (ajouter, modifier, supprimer, rechercher, etc.).

Le serveur affiche toutes les requêtes reçues et effectue le traitement demandé par le client.


