# Tetris Multijoueur - Projet de POO

## 📝 Introduction

Ce projet est une implémentation complète du jeu Tetris en C++, réalisée dans le cadre d'un module de Programmation Orientée Objet (POO).

Au-delà de la reproduction des mécaniques classiques du jeu, ce projet se distingue par l'intégration d'une architecture **Multijoueur en temps réel**. Grâce à une gestion réseau personnalisée, deux joueurs peuvent s'affronter : les lignes complétées par l'un sont envoyées comme "malus" (lignes grises) à l'adversaire, ajoutant une dimension compétitive stratégique.

## 🛠️ Prérequis et Technologies

Le projet repose sur les bibliothèques suivantes :
* **Raylib** : Pour le rendu graphique et la gestion des entrées (fenêtrage, audio, textures).
* **ENet** : Pour la communication réseau UDP fiable (gestion des paquets, connexions).
* **C++17** (ou supérieur) : Le standard utilisé pour le développement.

## 📦 Installation des Dépendances (Linux/Ubuntu/Debian)

Pour compiler le projet, vous devez installer les bibliothèques nécessaires. Ouvrez un terminal et exécutez les commandes suivantes :

### 1. Raylib (Rendu Graphique)
Ces commandes installent les dépendances, téléchargent le code source et compilent la librairie en mode partagé (shared).

```bash
# Dépendances de build
sudo apt install build-essential git cmake

# Dépendances graphiques et audio
sudo apt install libasound2-dev mesa-common-dev libx11-dev libxrandr-dev libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev

# Cloner et compiler Raylib
git clone [https://github.com/raysan5/raylib.git](https://github.com/raysan5/raylib.git) raylib
cd raylib/src/
make PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=SHARED
sudo make install RAYLIB_LIBTYPE=SHARED
```

### 2. ENet (Réseau)
La bibliothèque ENet est nécessaire pour la communication UDP.

```bash
sudo apt install libenet-dev
```

3. Playit.gg (Optionnel - Pour l'hébergement)

Si vous souhaitez héberger une partie sur internet sans configurer votre routeur, installez l'agent Playit.


**Méthode recommandée :**
```bash
curl -SsL [https://playit-cloud.github.io/ppa/key.gpg](https://playit-cloud.github.io/ppa/key.gpg) | gpg --dearmor | sudo tee /etc/apt/trusted.gpg.d/playit.gpg >/dev/null
echo "deb [signed-by=/etc/apt/trusted.gpg.d/playit.gpg] [https://playit-cloud.github.io/ppa/data](https://playit-cloud.github.io/ppa/data) ./" | sudo tee /etc/apt/sources.list.d/playit-cloud.list
sudo apt update
sudo apt install playit
```
**Alternative (si le dépôt est déjà configuré) :**
```bash
# Installation via le dépôt officiel (Ubuntu/Debian)
sudo apt update
sudo apt install playit
```

# Pour lancer l'agent après l'installation :
```bash
playit
```

Si cette installation ne marche pas, utilise:

 ```bash
curl -SsL https://playit-cloud.github.io/ppa/key.gpg | gpg --dearmor | sudo tee /etc/apt/trusted.gpg.d/playit.gpg >/dev/null
echo "deb [signed-by=/etc/apt/trusted.gpg.d/playit.gpg] https://playit-cloud.github.io/ppa/data ./" | sudo tee /etc/apt/sources.list.d/playit-cloud.list
sudo apt update
sudo apt install playit
```

---

## 🚀 Comment Exécuter

### 1. Lancer le Serveur
Le serveur doit être actif pour permettre aux clients de se connecter. Il gère le relais des paquets entre les joueurs.

Pour compiler et lancer le serveur (exemple avec g++) :
```bash
g++ src/network/clientData.cpp server/server.cpp server/main.cpp -lenet -o server/build/server.out 
./server/build/server.out
```

### 2. Lancer le Jeu (Client)

Le client contient l'interface graphique et la logique du jeu. Un Makefile est fourni pour automatiser la compilation.

Pour compiler et lancer le jeu :

```bash
make run
```

### 3. Connexion

Localhost (test local)
- Si le serveur et les clients sont sur la même machine, utilisez :

-  IP : 127.0.0.1

-  Port : 7788 (par défaut)

- Internet (Playit.gg)
Pour jouer à distance sans ouvrir les ports du routeur, l’utilisation d’un tunnel comme playit.gg est recommandée.
Lancez le tunnel sur la machine serveur et utilisez l’adresse publique fournie par le service dans le code client.
(La dernière partie du README).

---


## 📂 Architecture du Projet
Le code source est organisé de manière modulaire pour respecter les principes d'encapsulation et de séparation des responsabilités.

### 📂 `src/server/`

Contient le point d'entrée du serveur dédié.

* Rôle : Gère une map des clients connectés et leurs IDs.

* Relais : Reçoit les paquets d'un joueur (mouvement, grille, attaque) et les transfère immédiatement à son adversaire.

* Gestion : S'occupe de l'appariement (matchmaking simple) et détecte les déconnexions.

### 📂 `src/network/`

Gère toute la couche de communication (basée sur la librairie ENet).

* NetworkManager : Classe centrale qui gère l'envoi et la réception des paquets dans un thread séparé (sécurisé par des Mutex) pour éviter de bloquer le rendu graphique.

* NetworkStructures : Fichier définissant les structs utilisées pour le protocole binaire. Il assure l'alignement mémoire (#pragma pack) pour :

    * `UsernamePacket` : Envoi du pseudo.
    * `StatsPacket` : Envoi du Score et du Niveau.
    * `GridPacket` : Synchronisation de la grille.
    * `BlockPacket` : Mouvement de la pièce courante.
    * `AttackPacket` : Envoi des lignes de malus.
    * `SetIdPacket` : Envoi de l'identifiant unique.
    * `GameOverPacket` : Notification de fin de partie (Game Over).
    * `PlayerReadyPacket` : Confirmation d'être prêt à jouer.

### 📂 `src/core/`

Contient la logique pure du jeu Tetris (indépendante du rendu graphique et du réseau).

* Game : Classe mère gérant la boucle de jeu principale, le timing et la gravité.

* Grid : Représente la matrice de jeu (le puits 10x20), gère les collisions et la suppression des lignes complètes.

* Block / Tetrominoes : Définit les formes géométriques, les systèmes de rotation et les coordonnées.

* Stats : Gère le score, le niveau actuel et les statistiques des blocs utilisés.

### 📂 `src/utils/`

Contient les structures utilitaires légères utilisées partout dans le projet.

* Position : Structure simple (row, col) pour manipuler les coordonnées dans la grille.

* Colors : Gestion centralisée des palettes de couleurs pour les tétrominos et l'interface utilisateur.

### 📂 `src/graphics/`
Gère l'interface utilisateur et le lien avec Raylib.

* RaylibApp : La classe principale de l'application. Elle :

  * Initialise la fenêtre et charge les ressources (textures, polices).

  * Gère la machine à états de l'application (Menu, Jeu, Multiplayer, GameOver).

  * Dessine l'état du jeu (le Grid local et distant) à chaque frame.

---

## 🎮 Commandes

⬅️ / ➡️ Flèches gauche / droite : déplacer la pièce latéralement

⬆️ Flèche haut : rotation de la pièce

⬇️ Flèche bas : accélérer la chute (Soft Drop)

⏎ Entrée : démarrer la partie / rejouer

M : retour au menu principal

---

## Concepts Objet

### Utilisation de la STL (Standard Template Library) :

* `std::vector` : Pour la gestion dynamique de la grille de jeu et le stockage des positions des blocs.

* `std::map` : Utilisé côté serveur pour mapper les ID de connexions réseau (ENetPeer) aux identifiants uniques des joueurs (ClientData).

* `std::array` : Pour définir les points de la classe Stat.

* `std::queue` : Pour la gestion des messages réseau entrants dans le tampon.

### Héritage (Héritage de classes)

#### Classe de Base (Game) : 
Contient la logique commune (gravité, collision, score).

Classes Dérivées : 
  * GameMultiplayer : Étend Game pour le joueur local avec envoi de données réseau.

  *GameRemote : Spécialise Game pour représenter l'adversaire (logique passive).

#### Types de Blocs : 
  * Utilisation de l'héritage pour définir les formes spécifiques (I, O, T, etc.) à partir d'une classe Block générique.

### Polymorphisme (Comportements spécifiques)
Méthodes Virtuelles (virtual) : Redéfinition des fonctions comme run(), moveBlock() ou rotateBlock().

Substitution de Comportement : 
  * Le joueur local (Multiplayer) génère des paquets lors d'un mouvement.

  * L'adversaire (Remote) ignore les entrées clavier et attend les données du serveur.

Extension de Code : Capacité d'exécuter des actions différentes à travers une interface commune.

### Utilisation des Pointeurs Intelligents (Smart Pointers)
L'utilisation de std::unique_ptr et std::shared_ptr permet une gestion automatique de la mémoire et évite les fuites (memory leaks).

* `std::unique_ptr` (Propriété unique) :

  * Utilisé pour les objets dont le cycle de vie est strictement lié à une seule classe.

  * Application : La classe Game possède l'exclusivité sur la Grid, le Block actuel et le Next Block.

  * Avantage : Libération automatique de la mémoire dès que l'objet Game est détruit.

* `std::shared_ptr` (Propriété partagée) :

  * Utilisé lorsqu'un objet doit être accédé par plusieurs composants du programme.

  * Application : Le NetworkManager est partagé entre le MatchManager (pour envoyer des données) et la classe Game (pour recevoir les attaques).

  * Avantage : L'objet reste en mémoire tant qu'au moins un composant l'utilise.

### Flux et Itérateurs
Surcharge de l'opérateur de flux (`operator<<`) :

* Utilisée pour le debug rapide des objets (classe Block).

* Permet d'afficher l'état interne d'un objet directement dans la console (std::cout).

Itérateurs de la STL :

* Utilisation des itérateurs de la classe std::vector pour parcourir les statistiques.

### Gestion des Erreurs (Exceptions) :

* Utilisation de std::exception para sécuriser l'initialisation de la bibliothèque ENet.

* Exception spécifique pour gérer l'échec de connexion au serveur (Timeout/Host unreachable).

### Multithreading (Modèle Producteur-Consommateur) :

* Implémentation dans le NetworkManager pour un traitement asynchrone des paquets.

* Gestion de la concurrence : Utilisation de deux Mutex pour protéger la file d'attente (Queue) et les appels aux fonctions ENet.

### Généricité (Templates) :

* Utilisation de fonctions Templates pour l'envoi de paquets.

* Permet de gérer de manière générique les différentes structures de données de paquets (GridPacket, AttackPacket, etc.).


## 🌐 Configuration du Tunnel (Playit.gg)

Pour rendre votre serveur accessible depuis internet sans modifier la configuration de votre routeur (Port Forwarding), suivez ces étapes :

### 1. Lancer l'Agent
Sur la machine qui exécute le serveur de jeu (`./bin/server`), ouvrez un terminal et lancez l'agent :
```bash
playit
```
Le terminal affichera un lien unique ressemblant à : https://playit.gg/claim/xxxxxx

### 2. Accéder au Tableau de Bord

Copiez ce lien et ouvrez-le dans votre navigateur web.

Si vous n'avez pas de compte, le site vous proposera de continuer en tant qu'Invité (Guest) ou de Créer un compte. Les deux fonctionnent.

### 3. Créer le Tunnel

Une fois l'agent lié à votre compte via le navigateur :

- Cliquez sur "Add Tunnel".

- Sélectionnez "Custom".

- Choisissez le protocole UDP (Impératif car ENet utilise UDP).

- Dans le champ Local Port, entrez le port sur lequel votre serveur C++ écoute (par défaut 7788 dans ce projet).

- Validez la création ("Add Tunnel").

### 4. Récupérer l'Adresse Publique

Playit va générer une adresse publique (ex: sierra-tunnel.playit.gg:14590). Les joueurs distants devront utiliser cette adresse pour se connecter.