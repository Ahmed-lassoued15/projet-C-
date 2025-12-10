# 🏢 Système de Gestion de Réservations

## 📋 Description du Projet

Système complet de gestion de réservations pour salles de réunion avec système de facturation automatique. Solution idéale pour petites entreprises et espaces de coworking.

**Projet réalisé dans le cadre des études à l'École Nationale des Sciences de l'Informatique (ENSI)**

---

## ✨ Fonctionnalités Principales

### 🏛️ Gestion des Salles
-  Ajout de nouvelles salles
-  Configuration de la capacité
-  Définition des tarifs horaires
-  Spécification des équipements disponibles
-  Affichage de toutes les salles

### 📅 Gestion des Réservations
-  Création de réservations
-  Vérification automatique de la disponibilité
-  Détection des conflits de réservation (chevauchements)
-  Vérification de la capacité de la salle
-  Calcul automatique du coût (tarif_horaire × durée)
-  Affichage de toutes les réservations
-  Gestion des statuts (confirmée, annulée, en_attente)

### 💰 Gestion des Tarifs
-  Tarifs différents selon les salles
-  Calcul automatique basé sur la durée
-  Sauvegarde des tarifs dans un fichier de configuration

### 🧾 Génération de Factures
-  Création de factures au format professionnel
-  Numérotation automatique (FACT-XXX)
-  Informations complètes : client, salle, date, durée, montant
-  Date d'émission automatique
-  Sauvegarde dans des fichiers dédiés

### 📊 Module de Statistiques
-  Chiffre d'affaires par salle
-  Nombre de réservations par mois
-  Classement des salles les plus populaires
-  Analyse de l'utilisation

### 💾 Persistance des Données
-  Sauvegarde automatique dans des fichiers binaires
-  Chargement des données au démarrage
-  Sauvegarde manuelle disponible
-  Trois fichiers : salles.dat, reservations.dat, factures.dat

---

## 🛠️ Technologies Utilisées

- **Langage** : C (standard C99)
- **Structures de données** : Structures, tableaux statiques
- **Bibliothèques** : 
  - `stdio.h` : entrées/sorties
  - `stdlib.h` : fonctions standard
  - `string.h` : manipulation de chaînes
  - `time.h` : gestion des dates

---

## 📂 Architecture du Projet

```
projet-reservation/
│
├── main.c                    # Code source principal
├── salles.dat               # Fichier de données des salles
├── reservations.dat         # Fichier de données des réservations
├── factures.dat             # Fichier de données des factures
└── README.md                # Ce fichier
```

---

## 🚀 Installation et Compilation

### Prérequis
- Compilateur C (GCC recommandé)
- Terminal/Console

### Compilation

#### Sur Linux/Mac :
```bash
gcc -o reservation main.c
./reservation
```

#### Sur Windows (avec MinGW) :
```bash
gcc -o reservation.exe main.c
reservation.exe
```

#### Avec des options d'optimisation :
```bash
gcc -Wall -Wextra -O2 -o reservation main.c
```

---

## 📖 Guide d'Utilisation

### Démarrage
1. Compilez le programme
2. Lancez l'exécutable
3. Le menu principal s'affiche

### Menu Principal

```
╔═══════════════════════════════════════════════════════════════╗
║      SYSTÈME DE GESTION DE RÉSERVATIONS                       ║
║      École Nationale des Sciences de l'Informatique           ║
╚═══════════════════════════════════════════════════════════════╝

┌───────────────────────────────────────────────────────────────┐
│                    GESTION DES SALLES                         │
├───────────────────────────────────────────────────────────────┤
│  1. Ajouter une salle                                         │
│  2. Afficher les salles                                       │
├───────────────────────────────────────────────────────────────┤
│                 GESTION DES RÉSERVATIONS                      │
├───────────────────────────────────────────────────────────────┤
│  3. Créer une réservation                                     │
│  4. Afficher les réservations                                 │
├───────────────────────────────────────────────────────────────┤
│                  GESTION DES FACTURES                         │
├───────────────────────────────────────────────────────────────┤
│  5. Générer une facture                                       │
│  6. Afficher les factures                                     │
├───────────────────────────────────────────────────────────────┤
│                     STATISTIQUES                              │
├───────────────────────────────────────────────────────────────┤
│  7. Afficher les statistiques                                 │
├───────────────────────────────────────────────────────────────┤
│                   SAUVEGARDE & QUITTER                        │
├───────────────────────────────────────────────────────────────┤
│  8. Sauvegarder les données                                   │
│  0. Quitter                                                   │
└───────────────────────────────────────────────────────────────┘
```

### Workflow Typique

#### 1️⃣ Première utilisation - Configuration
```
1. Ajouter des salles (Option 1)
   - Salle A : 10 personnes, 50 TND/h
   - Salle B : 20 personnes, 80 TND/h
   - Salle C : 50 personnes, 150 TND/h

2. Sauvegarder (Option 8)
```

#### 2️⃣ Créer une réservation (Option 3)
```
Nom du client: Mohamed Ali
Salle: 1 (Salle A)
Date: 2024-12-15
Heure début: 09:00
Heure fin: 11:00
Nombre de personnes: 8

✅ Réservation confirmée!
Coût: 100.00 TND (2h × 50 TND/h)
```

#### 3️⃣ Générer une facture (Option 5)
```
Sélectionner la réservation
→ Facture FACT-1 générée automatiquement
```

#### 4️⃣ Consulter les statistiques (Option 7)
```
📊 Chiffre d'affaires par salle
📅 Réservations par mois
⭐ Salles les plus populaires
```

---

## 🔒 Gestion des Conflits

### Le système vérifie automatiquement :

#### ✅ Disponibilité temporelle
```c
Réservation existante : 09:00 - 11:00
Nouvelle demande     : 10:00 - 12:00
→ ❌ REFUSÉE (chevauchement)

Nouvelle demande     : 11:00 - 13:00
→ ✅ ACCEPTÉE (pas de conflit)
```

#### ✅ Capacité de la salle
```c
Salle A : capacité 10 personnes
Demande : 15 personnes
→ ❌ REFUSÉE (capacité insuffisante)

Demande : 8 personnes
→ ✅ ACCEPTÉE
```

---

## 📊 Exemples de Statistiques

### Chiffre d'affaires par salle
```
Salle A: 450.00 TND
Salle B: 640.00 TND
Salle C: 300.00 TND
```

### Réservations par mois
```
2024-12: 15 réservations
2024-11: 12 réservations
2024-10: 18 réservations
```

### Salles les plus populaires
```
1. Salle B: 25 réservations
2. Salle A: 18 réservations
3. Salle C: 12 réservations
```

---

## 🗂️ Structure des Données

### Structure Salle
```c
typedef struct {
    int id;                    // Identifiant unique
    char nom[50];             // Nom de la salle
    int capacite;             // Nombre max de personnes
    float tarif_horaire;      // Prix par heure (TND)
    char equipements[200];    // Liste des équipements
} Salle;
```

### Structure Réservation
```c
typedef struct {
    int id;                   // Identifiant unique
    char nom_client[100];     // Nom du client
    int id_salle;            // ID de la salle réservée
    char nom_salle[50];      // Nom de la salle
    char date[11];           // Format: YYYY-MM-DD
    char heure_debut[6];     // Format: HH:MM
    char heure_fin[6];       // Format: HH:MM
    int nombre_personnes;    // Nombre de participants
    float tarif;             // Coût total calculé
    char statut[20];         // État de la réservation
} Reservation;
```

### Structure Facture
```c
typedef struct {
    int id;                   // Identifiant unique
    char numero[20];          // Numéro de facture (FACT-XXX)
    char nom_client[100];     // Nom du client
    char nom_salle[50];       // Nom de la salle
    char date[11];            // Date de réservation
    float duree;              // Durée en heures
    float montant;            // Montant total (TND)
    char date_emission[11];   // Date de création
} Facture;
```

---

## 🎯 Tâches Implémentées

###  Tâche 1 : Modèle de Données Complet
- Structures pour Réservation, Salle, Facture
- Tous les champs requis présents

###  Tâche 2 : Système de Réservation
- Vérification de disponibilité
- Vérification de capacité
- Empêchement des chevauchements
- Calcul automatique du tarif

###  Tâche 3 : Gestion des Tarifs
- Tarifs différents par salle
- Calcul automatique du coût total
- Sauvegarde dans fichier de configuration

###  Tâche 4 : Génération de Factures
- Format simple avec toutes les informations
- Sauvegarde dans des fichiers dédiés
- Numérotation automatique

###  Tâche 5 : Module de Statistiques
- Chiffre d'affaires par salle
- Nombre de réservations par mois
- Salles les plus populaires

###  Tâche 6 : Persistance des Données
- Sauvegarde dans des fichiers binaires
- Chargement au démarrage
- Sauvegarde automatique après chaque modification

---

## 🔧 Améliorations Possibles

### Fonctionnalités additionnelles :
- [ ] Modification de réservations existantes
- [ ] Annulation de réservations
- [ ] Recherche de réservations par critères
- [ ] Export des statistiques en CSV
- [ ] Système de notifications par email
- [ ] Gestion multi-utilisateurs avec authentification
- [ ] Interface graphique (GUI)
- [ ] Base de données SQL au lieu de fichiers binaires
- [ ] API REST pour intégration web/mobile
- [ ] Système de paiement en ligne

### Optimisations techniques :
- [ ] Allocation dynamique de mémoire
- [ ] Listes chaînées au lieu de tableaux statiques
- [ ] Amélioration de l'algorithme de recherche
- [ ] Gestion d'erreurs plus robuste
- [ ] Tests unitaires

---

## 🐛 Résolution de Problèmes

### Problème : Le programme ne compile pas
**Solution** :
```bash
# Vérifier que GCC est installé
gcc --version

# Compiler avec tous les avertissements
gcc -Wall -Wextra main.c -o reservation
```

### Problème : Les données ne se sauvegardent pas
**Solution** :
- Vérifier les permissions d'écriture dans le dossier
- Utiliser l'option 8 pour sauvegarder manuellement
- Vérifier que les fichiers .dat sont créés

### Problème : Erreur de segmentation
**Solution** :
- Vérifier que vous ne dépassez pas les limites des tableaux
- Maximum 100 salles, 500 réservations, 500 factures

---

## 👨‍💻 Auteur

**Étudiant de l'École Nationale des Sciences de l'Informatique (ENSI)**

**Keyword** : Entreprenariat

---

## 📄 Licence

Projet académique - ENSI

---

## 📞 Support

Pour toute question ou suggestion, contactez votre professeur ou l'équipe pédagogique de l'ENSI.

---

## 🎓 Contexte Académique

Ce projet a été développé dans le cadre du cours d'**Entreprenariat** à l'**École Nationale des Sciences de l'Informatique**. Il démontre les compétences suivantes :

- Programmation en C
- Structures de données
- Gestion de fichiers
- Conception d'algorithmes
- Interface utilisateur en console
- Gestion de projet
- Documentation technique

---

## ⭐ Remerciements

Merci à l'équipe pédagogique de l'ENSI pour l'encadrement et le soutien dans la réalisation de ce projet.

---

**Date de dernière mise à jour** : Décembre 2024

**Version** : 1.0.0
