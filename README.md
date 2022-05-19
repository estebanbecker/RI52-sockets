# Sockets

Ce TD a pour but de vous familiariser progressivement à la programmation d'applications reposant sur des communications réseau basées sur des sockets.

L'implémentation sera faite dans un premier temps en C, puis en C++ dans les derniers exercices.

Ce sujet évoluera au fil du temps pour couvrir les bases des sockets, puis le multiplexage, et finalement l'usage de Qt.

Les instructions et la base de code éventuelle seront partagées via des dépôts git. Si vous n'êtes pas à l'aise avec cet outil, vous pouvez vous référer au cours de GL41 disponible [ici](https://www.youtube.com/watch?v=jJBODtE_Rts&list=PLNgzB9uJ0Ss58ZdoPk1vueOMYYdfBkKoS)

Les vidéos du cours sur les sockets publiées sur Moodle sont également disponibles sur [Youtube](https://www.youtube.com/watch?v=4goPrs4NtOk&list=PLNgzB9uJ0Ss4nsIBJV8j8XJwYzwV43nk2)

## Paramétrisation et sécurisation d'un client TCP

Dans cet exercice, vous vous baserez sur l'exemple de client TCP vu en cours pour le rendre paramétrable :

- l'option `h` permet de définir l'hôte auquel se connectera le client.
- l'option `p` définira le port d'écoute du serveur auquel le client va se connecter.

Vous vérifierez les retours des fonctions utilisées dans le programme pour permettre de quitter proprement en cas de problème (serveur inexistant, etc.)

## Serveur UDP avec trace des réceptions

Dans cet exercice, vous vous baserez sur le serveur UDP du cours, qu'il s'agira dans un premier temps de rendre plus robuste (vérification des valeurs de retour des fonctions), avant de permettre de récupérer l'adresse et le port source des envois reçus. (il faut passer des paramètres non `NULL` au deux derniers paramètres de recvfrom)

## Serveur TCP full duplex

On considérera toujours un serveur TCP à un seul client. Celui ci devra ajouter un texte à celui envoyé par son client et lui répondra par ce texte complété. Là encore, il est nécessaire de mémoriser l'adresse et le port du client connecté.
