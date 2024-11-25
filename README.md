# LambEngine

## Description

Un moteur 3D développé en C++ utilisant OpenGL et un ensemble de bibliothèques modernes pour offrir un rendu graphique performant et des fonctionnalités modulaires. Ce projet est conçu pour être une base de travail simple mais puissante pour expérimenter avec les concepts de rendu 3D et les systèmes ECS.

---

## Fonctionnalités

### 🎨 Rendu graphique
- **Parseur de fichiers MTL** : Chargement et gestion des matériaux avec prise en charge des propriétés `ambient`, `diffuse`, et `specular`.
- **Illumination de Phong** : Simulation réaliste des réflexions et éclairages.
- **Lighting Maps** : Gestion avancée des textures d'éclairage, y compris diffuse et specular maps.
- **Système de lumières** : Support des lumières directionnelles, ponctuelles, et spotlights avec des bordures douces.
- **Création de primitive** : Création paramètrique de cube et de sphère.

### 📷 Gestion de la caméra
- Mouvement fluide avec souris et clavier.
- Modes caméra FPS et orbitale.
- Utilisation de la bibliothèque `glm` pour la gestion des transformations 3D.

### 🛠️ Système ECS (Entity Component System)
- Gestion minimale des entités et composants pour une organisation claire des objets de la scène.
- Extensible pour ajouter de nouvelles fonctionnalités comme des animations ou des comportements personnalisés.

### 📂 Gestion des fichiers
- Chargement de modèles via **Assimp** (formats supportés : `.obj`, `.fbx`, etc.).
- Parsing et gestion des matériaux via les fichiers `.mtl`.

### 🔍 Tests unitaires
- Utilisation de **Google Test (gtest)** pour assurer la stabilité et la fiabilité du moteur.

### 🖱️ Interface utilisateur
- Intégration de **Dear ImGui** pour un outil d'inspection et de débogage interactif.

---

## Technologies utilisées

- **OpenGL** : API de rendu graphique.
- **GLM** : Librairie mathématique pour les transformations et calculs 3D.
- **SDL2** : Gestion de la fenêtre, des entrées et des événements.
- **Assimp** : Chargement et parsing des fichiers de modèles 3D.
- **GLAD** : Loader d'extensions OpenGL.
- **Dear ImGui** : Interface utilisateur graphique.
- **Google Test** : Tests unitaires pour valider le fonctionnement du moteur.
- **nlohmann-json** : Parsing et manipulation des fichiers JSON pour les configurations.

---

## Dépendances

Le gestionnaire de packages [**vcpkg**](https://github.com/microsoft/vcpkg) est utilisé pour gérer toutes les bibliothèques externes.
