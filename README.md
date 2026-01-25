# Tetris Multijoueur - Projet de POO

## 📝 Introduction

Ce projet est une implémentation complète du jeu Tetris en C++, réalisée dans le cadre d'un module de Programmation Orientée Objet (POO).

Au-delà de la reproduction des mécaniques classiques du jeu, ce projet se distingue par l'intégration d'une architecture **Multijoueur en temps réel**. Grâce à une gestion réseau personnalisée, deux joueurs peuvent s'affronter : les lignes complétées par l'un sont envoyées comme "malus" (lignes grises) à l'adversaire, ajoutant une dimension compétitive stratégique.

## 🛠️ Prérequis et Technologies

Le projet repose sur les bibliothèques suivantes :
* **Raylib** : Pour le rendu graphique et la gestion des entrées (fenêtrage, audio, textures).
* **ENet** : Pour la communication réseau UDP fiable (gestion des paquets, connexions).
* **C++17** (ou supérieur) : Le standard utilisé pour le développement.

---

## 🚀 Comment Exécuter

### 1. Lancer le Serveur
Le serveur doit être actif pour permettre aux clients de se connecter. Il gère le relais des paquets entre les joueurs.

Pour compiler et lancer le serveur (exemple avec g++) :
```bash
g++ server/server.cpp src/network/*.cpp -o bin/server -lenet -pthread
./bin/server
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

---


## 📂 Architecture du Projet
Le code source est organisé de manière modulaire pour respecter les principes d'encapsulation et de séparation des responsabilités.

### 📂 src/server/

Contient le point d'entrée du serveur dédié.

Rôle : Gère une map des clients connectés et leurs IDs.

Relais : Reçoit les paquets d'un joueur (mouvement, grille, attaque) et les transfère immédiatement à son adversaire.

Gestion : S'occupe de l'appariement (matchmaking simple) et détecte les déconnexions.

### 📂 src/network/

Gère toute la couche de communication (basée sur la librairie ENet).

NetworkManager : Classe centrale qui gère l'envoi et la réception des paquets dans un thread séparé (sécurisé par des Mutex) pour éviter de bloquer le rendu graphique.

NetworkStructures : Fichier définissant les structs utilisées pour le protocole binaire. Il assure l'alignement mémoire (#pragma pack) pour :

UsernamePacket : Envoi du pseudo.

GridPacket : Synchronisation de la grille.

BlockPacket : Mouvement de la pièce courante.

AttackPacket : Envoi des lignes de malus.

### 📂 src/core/

Contient la logique pure du jeu Tetris (indépendante du rendu graphique et du réseau).

Game : Classe mère gérant la boucle de jeu principale, le timing et la gravité.

Grid : Représente la matrice de jeu (le puits 10x20), gère les collisions et la suppression des lignes complètes.

Block / Tetrominoes : Définit les formes géométriques, les systèmes de rotation et les coordonnées.

Stats : Gère le score, le niveau actuel et les statistiques des blocs utilisés.

### 📂 src/utils/

Contient les structures utilitaires légères utilisées partout dans le projet.

Position : Structure simple (row, col) pour manipuler les coordonnées dans la grille.

Colors : Gestion centralisée des palettes de couleurs pour les tétrominos et l'interface utilisateur.

### 📂 src/graphics/
Gère l'interface utilisateur et le lien avec Raylib.

RaylibApp : La classe principale de l'application. Elle :

Initialise la fenêtre et charge les ressources (textures, polices).

Gère la machine à états de l'application (Menu, Jeu, Multiplayer, GameOver).

Dessine l'état du jeu (le Grid local et distant) à chaque frame.

## 🎮 Commandes

⬅️ / ➡️ Flèches gauche / droite : déplacer la pièce latéralement

⬆️ Flèche haut : rotation de la pièce

⬇️ Flèche bas : accélérer la chute (Soft Drop)

⏎ Entrée : démarrer la partie / rejouer

M : retour au menu principal

Échap : quitter le jeu