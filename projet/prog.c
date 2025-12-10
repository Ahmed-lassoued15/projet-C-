#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif

typedef struct {
    int id;
    char nom[50];
    int capacite;
    float tarif_horaire;
    char equipements[200];
} Salle;

typedef struct {
    int id;
    char nom_client[100];
    int id_salle;
    char nom_salle[50];
    char date[11];  
    char heure_debut[6];  
    char heure_fin[6];
    int nombre_personnes;
    float tarif;
    char statut[20];
} Reservation;

typedef struct {
    int id;
    char numero[20];
    char nom_client[100];
    char nom_salle[50];
    char date[11];
    float duree;
    float montant;
    char date_emission[11];
} Facture;

Salle salles[100];
Reservation reservations[500];
Facture factures[500];
int nb_salles = 0;
int nb_reservations = 0;
int nb_factures = 0;


void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pause_screen() {
    printf("\nAppuyez sur Entrée pour continuer...");
    getchar();
    getchar();
}

void get_current_date(char *date) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(date, 11, "%Y-%m-%d", tm_info);
}

float calculer_duree(const char *heure_debut, const char *heure_fin) {
    int h1, m1, h2, m2;
    sscanf(heure_debut, "%d:%d", &h1, &m1);
    sscanf(heure_fin, "%d:%d", &h2, &m2);
    
    int minutes_debut = h1 * 60 + m1;
    int minutes_fin = h2 * 60 + m2;
    
    return (float)(minutes_fin - minutes_debut) / 60.0;
}

// ============================================================================
// TÂCHE 1 : GESTION DES SALLES
// ============================================================================

// Fonction pour afficher toutes les salles
void afficher_salles() {
    clearScreen();
    printf("\n╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║            LISTE DES SALLES DISPONIBLES                      ║\n");
    printf("╚═══════════════════════════════════════════════════════════════╝\n\n");
    
    if (nb_salles == 0) {
        printf("Aucune salle enregistrée.\n");
        return;
    }
    
    for (int i = 0; i < nb_salles; i++) {
        printf("┌─────────────────────────────────────────────────────────────┐\n");
        printf("│ ID: %d\n", salles[i].id);
        printf("│ Nom: %s\n", salles[i].nom);
        printf("│ Capacité: %d personnes\n", salles[i].capacite);
        printf("│ Tarif horaire: %.2f TND\n", salles[i].tarif_horaire);
        printf("│ Équipements: %s\n", salles[i].equipements);
        printf("└─────────────────────────────────────────────────────────────┘\n\n");
    }
}

// Fonction pour ajouter une nouvelle salle
void ajouter_salle() {
    clearScreen();
    printf("\n╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║                AJOUTER UNE NOUVELLE SALLE                     ║\n");
    printf("╚═══════════════════════════════════════════════════════════════╝\n\n");
    
    if (nb_salles >= 100) {
        printf("❌ Limite de salles atteinte!\n");
        pause_screen();
        return;
    }
    
    Salle nouvelle_salle;
    nouvelle_salle.id = nb_salles + 1;
    
    printf("Nom de la salle: ");
    scanf(" %[^\n]", nouvelle_salle.nom);
    
    printf("Capacité (nombre de personnes): ");
    scanf("%d", &nouvelle_salle.capacite);
    
    printf("Tarif horaire (TND): ");
    scanf("%f", &nouvelle_salle.tarif_horaire);
    
    printf("Équipements (séparés par des virgules): ");
    scanf(" %[^\n]", nouvelle_salle.equipements);
    
    salles[nb_salles] = nouvelle_salle;
    nb_salles++;
    
    printf("\n✅ Salle ajoutée avec succès!\n");
    pause_screen();
}
int trouver_salle(const char *nom) {
    for (int i = 0; i < nb_salles; i++) {
        if (strcmp(salles[i].nom, nom) == 0) {
            return i;
        }
    }
    return -1;
}

int periodes_se_chevauchent(const char *debut1, const char *fin1, 
                            const char *debut2, const char *fin2) {
    int h1d, m1d, h1f, m1f, h2d, m2d, h2f, m2f;
    
    sscanf(debut1, "%d:%d", &h1d, &m1d);
    sscanf(fin1, "%d:%d", &h1f, &m1f);
    sscanf(debut2, "%d:%d", &h2d, &m2d);
    sscanf(fin2, "%d:%d", &h2f, &m2f);
    
    int min1d = h1d * 60 + m1d;
    int min1f = h1f * 60 + m1f;
    int min2d = h2d * 60 + m2d;
    int min2f = h2f * 60 + m2f;
    
    return (min1d < min2f && min1f > min2d);
}

int verifier_disponibilite(const char *nom_salle, const char *date, 
                           const char *heure_debut, const char *heure_fin) {
    for (int i = 0; i < nb_reservations; i++) {
        if (strcmp(reservations[i].nom_salle, nom_salle) == 0 &&
            strcmp(reservations[i].date, date) == 0 &&
            strcmp(reservations[i].statut, "confirmée") == 0) {
            
            if (periodes_se_chevauchent(heure_debut, heure_fin,
                                       reservations[i].heure_debut,
                                       reservations[i].heure_fin)) {
                return 0;  
            }
        }
    }
    return 1; 
}

float calculer_cout(int index_salle, const char *heure_debut, const char *heure_fin) {
    float duree = calculer_duree(heure_debut, heure_fin);
    return duree * salles[index_salle].tarif_horaire;
}

void creer_reservation() {
    clearScreen();
    printf("\n╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║              CRÉER UNE NOUVELLE RÉSERVATION                   ║\n");
    printf("╚═══════════════════════════════════════════════════════════════╝\n\n");
    
    if (nb_salles == 0) {
        printf("❌ Aucune salle disponible. Veuillez d'abord ajouter des salles.\n");
        pause_screen();
        return;
    }
    
    if (nb_reservations >= 500) {
        printf("❌ Limite de réservations atteinte!\n");
        pause_screen();
        return;
    }
    
    Reservation nouvelle_res;
    nouvelle_res.id = nb_reservations + 1;
    
    printf("Nom du client: ");
    scanf(" %[^\n]", nouvelle_res.nom_client);
   
    printf("\nSalles disponibles:\n");
    for (int i = 0; i < nb_salles; i++) {
        printf("  %d. %s (Capacité: %d, Tarif: %.2f TND/h)\n", 
               i+1, salles[i].nom, salles[i].capacite, salles[i].tarif_horaire);
    }
    
    int choix_salle;
    printf("\nChoisissez une salle (1-%d): ", nb_salles);
    scanf("%d", &choix_salle);
    
    if (choix_salle < 1 || choix_salle > nb_salles) {
        printf("❌ Choix invalide!\n");
        pause_screen();
        return;
    }
    
    int index_salle = choix_salle - 1;
    nouvelle_res.id_salle = salles[index_salle].id;
    strcpy(nouvelle_res.nom_salle, salles[index_salle].nom);
    
    printf("Date (YYYY-MM-DD): ");
    scanf("%s", nouvelle_res.date);
    
    printf("Heure de début (HH:MM): ");
    scanf("%s", nouvelle_res.heure_debut);
    
    printf("Heure de fin (HH:MM): ");
    scanf("%s", nouvelle_res.heure_fin);
    
    printf("Nombre de personnes: ");
    scanf("%d", &nouvelle_res.nombre_personnes);
    
    if (nouvelle_res.nombre_personnes > salles[index_salle].capacite) {
        printf("\n❌ CAPACITÉ INSUFFISANTE!\n");
        printf("La salle %s a une capacité de %d personnes.\n", 
               salles[index_salle].nom, salles[index_salle].capacite);
        pause_screen();
        return;
    }

    if (!verifier_disponibilite(nouvelle_res.nom_salle, nouvelle_res.date,
                                nouvelle_res.heure_debut, nouvelle_res.heure_fin)) {
        printf("\n❌ CONFLIT DE RÉSERVATION!\n");
        printf("La salle n'est pas disponible pour cette période.\n");
        pause_screen();
        return;
    }
   
    nouvelle_res.tarif = calculer_cout(index_salle, nouvelle_res.heure_debut, 
                                       nouvelle_res.heure_fin);
    
    strcpy(nouvelle_res.statut, "confirmée");
    
    reservations[nb_reservations] = nouvelle_res;
    nb_reservations++;
    
    printf("\n✅ RÉSERVATION CONFIRMÉE!\n");
    printf("═══════════════════════════════════════\n");
    printf("Client: %s\n", nouvelle_res.nom_client);
    printf("Salle: %s\n", nouvelle_res.nom_salle);
    printf("Date: %s\n", nouvelle_res.date);
    printf("Horaire: %s - %s\n", nouvelle_res.heure_debut, nouvelle_res.heure_fin);
    printf("Durée: %.2f heures\n", calculer_duree(nouvelle_res.heure_debut, nouvelle_res.heure_fin));
    printf("Coût total: %.2f TND\n", nouvelle_res.tarif);
    printf("═══════════════════════════════════════\n");
    
    pause_screen();
}

void afficher_reservations() {
    clearScreen();
    printf("\n╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║                 LISTE DES RÉSERVATIONS                        ║\n");
    printf("╚═══════════════════════════════════════════════════════════════╝\n\n");
    
    if (nb_reservations == 0) {
        printf("Aucune réservation enregistrée.\n");
        pause_screen();
        return;
    }
    
    for (int i = 0; i < nb_reservations; i++) {
        printf("┌─────────────────────────────────────────────────────────────┐\n");
        printf("│ ID: %d\n", reservations[i].id);
        printf("│ Client: %s\n", reservations[i].nom_client);
        printf("│ Salle: %s\n", reservations[i].nom_salle);
        printf("│ Date: %s\n", reservations[i].date);
        printf("│ Horaire: %s - %s\n", reservations[i].heure_debut, reservations[i].heure_fin);
        printf("│ Personnes: %d\n", reservations[i].nombre_personnes);
        printf("│ Coût: %.2f TND\n", reservations[i].tarif);
        printf("│ Statut: %s\n", reservations[i].statut);
        printf("└─────────────────────────────────────────────────────────────┘\n\n");
    }
    
    pause_screen();
}

void generer_facture() {
    clearScreen();
    printf("\n╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║                   GÉNÉRER UNE FACTURE                         ║\n");
    printf("╚═══════════════════════════════════════════════════════════════╝\n\n");
    
    if (nb_reservations == 0) {
        printf("❌ Aucune réservation disponible.\n");
        pause_screen();
        return;
    }
    
    // Afficher les réservations
    printf("Réservations disponibles:\n");
    for (int i = 0; i < nb_reservations; i++) {
        if (strcmp(reservations[i].statut, "confirmée") == 0) {
            printf("  %d. %s - %s - %s (%.2f TND)\n", 
                   reservations[i].id,
                   reservations[i].nom_client,
                   reservations[i].nom_salle,
                   reservations[i].date,
                   reservations[i].tarif);
        }
    }
    
    int id_res;
    printf("\nEntrez l'ID de la réservation: ");
    scanf("%d", &id_res);

    int index = -1;
    for (int i = 0; i < nb_reservations; i++) {
        if (reservations[i].id == id_res) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        printf("❌ Réservation non trouvée!\n");
        pause_screen();
        return;
    }
    
    Facture nouvelle_facture;
    nouvelle_facture.id = nb_factures + 1;
    sprintf(nouvelle_facture.numero, "FACT-%d", nouvelle_facture.id);
    strcpy(nouvelle_facture.nom_client, reservations[index].nom_client);
    strcpy(nouvelle_facture.nom_salle, reservations[index].nom_salle);
    strcpy(nouvelle_facture.date, reservations[index].date);
    nouvelle_facture.duree = calculer_duree(reservations[index].heure_debut, 
                                           reservations[index].heure_fin);
    nouvelle_facture.montant = reservations[index].tarif;
    get_current_date(nouvelle_facture.date_emission);
    
    factures[nb_factures] = nouvelle_facture;
    nb_factures++;
    
    // Afficher la facture
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║                         FACTURE                               ║\n");
    printf("╠═══════════════════════════════════════════════════════════════╣\n");
    printf("║ Numéro: %-52s ║\n", nouvelle_facture.numero);
    printf("║ Date d'émission: %-43s ║\n", nouvelle_facture.date_emission);
    printf("╠═══════════════════════════════════════════════════════════════╣\n");
    printf("║ Client: %-52s ║\n", nouvelle_facture.nom_client);
    printf("║ Salle: %-53s ║\n", nouvelle_facture.nom_salle);
    printf("║ Date de réservation: %-39s ║\n", nouvelle_facture.date);
    printf("║ Durée: %.2f heures %-41s ║\n", nouvelle_facture.duree, "");
    printf("╠═══════════════════════════════════════════════════════════════╣\n");
    printf("║ MONTANT TOTAL: %.2f TND %-35s ║\n", nouvelle_facture.montant, "");
    printf("╚═══════════════════════════════════════════════════════════════╝\n");
    
    printf("\n✅ Facture générée avec succès!\n");
    pause_screen();
}

void afficher_factures() {
    clearScreen();
    printf("\n╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║                   LISTE DES FACTURES                          ║\n");
    printf("╚═══════════════════════════════════════════════════════════════╝\n\n");
    
    if (nb_factures == 0) {
        printf("Aucune facture générée.\n");
        pause_screen();
        return;
    }
    
    for (int i = 0; i < nb_factures; i++) {
        printf("┌─────────────────────────────────────────────────────────────┐\n");
        printf("│ Numéro: %s\n", factures[i].numero);
        printf("│ Client: %s\n", factures[i].nom_client);
        printf("│ Salle: %s\n", factures[i].nom_salle);
        printf("│ Date: %s\n", factures[i].date);
        printf("│ Durée: %.2f heures\n", factures[i].duree);
        printf("│ Montant: %.2f TND\n", factures[i].montant);
        printf("│ Émise le: %s\n", factures[i].date_emission);
        printf("└─────────────────────────────────────────────────────────────┘\n\n");
    }
    
    pause_screen();
}

void afficher_statistiques() {
    clearScreen();
    printf("\n╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║                      STATISTIQUES                             ║\n");
    printf("╚═══════════════════════════════════════════════════════════════╝\n\n");
    
    if (nb_reservations == 0) {
        printf("Aucune donnée disponible.\n");
        pause_screen();
        return;
    }

    printf("📊 CHIFFRE D'AFFAIRES PAR SALLE\n");
    printf("═══════════════════════════════════════\n");
    
    float ca_par_salle[100] = {0};
    for (int i = 0; i < nb_reservations; i++) {
        if (strcmp(reservations[i].statut, "confirmée") == 0) {
            for (int j = 0; j < nb_salles; j++) {
                if (strcmp(reservations[i].nom_salle, salles[j].nom) == 0) {
                    ca_par_salle[j] += reservations[i].tarif;
                    break;
                }
            }
        }
    }
    
    for (int i = 0; i < nb_salles; i++) {
        if (ca_par_salle[i] > 0) {
            printf("  %s: %.2f TND\n", salles[i].nom, ca_par_salle[i]);
        }
    }
    
    printf("\n📅 RÉSERVATIONS PAR MOIS\n");
    printf("═══════════════════════════════════════\n");
    
    char mois[50][8];
    int count_mois[50] = {0};
    int nb_mois = 0;
    
    for (int i = 0; i < nb_reservations; i++) {
        char mois_res[8];
        strncpy(mois_res, reservations[i].date, 7);
        mois_res[7] = '\0';
        
        int trouve = 0;
        for (int j = 0; j < nb_mois; j++) {
            if (strcmp(mois[j], mois_res) == 0) {
                count_mois[j]++;
                trouve = 1;
                break;
            }
        }
        
        if (!trouve) {
            strcpy(mois[nb_mois], mois_res);
            count_mois[nb_mois] = 1;
            nb_mois++;
        }
    }
    
    for (int i = 0; i < nb_mois; i++) {
        printf("  %s: %d réservations\n", mois[i], count_mois[i]);
    }
  
    printf("\n⭐ SALLES LES PLUS POPULAIRES\n");
    printf("═══════════════════════════════════════\n");
    
    int popularite[100] = {0};
    for (int i = 0; i < nb_reservations; i++) {
        for (int j = 0; j < nb_salles; j++) {
            if (strcmp(reservations[i].nom_salle, salles[j].nom) == 0) {
                popularite[j]++;
                break;
            }
        }
    }
    
    for (int i = 0; i < nb_salles - 1; i++) {
        for (int j = 0; j < nb_salles - i - 1; j++) {
            if (popularite[j] < popularite[j + 1]) {
                // Échanger les popularités
                int temp_pop = popularite[j];
                popularite[j] = popularite[j + 1];
                popularite[j + 1] = temp_pop;
                
                // Échanger les salles
                Salle temp_salle = salles[j];
                salles[j] = salles[j + 1];
                salles[j + 1] = temp_salle;
            }
        }
    }
    
    for (int i = 0; i < nb_salles; i++) {
        if (popularite[i] > 0) {
            printf("  %d. %s: %d réservations\n", i+1, salles[i].nom, popularite[i]);
        }
    }
    
    pause_screen();
}

void sauvegarder_donnees() {
    FILE *f_salles = fopen("salles.dat", "wb");
    if (f_salles != NULL) {
        fwrite(&nb_salles, sizeof(int), 1, f_salles);
        fwrite(salles, sizeof(Salle), nb_salles, f_salles);
        fclose(f_salles);
    }

    FILE *f_res = fopen("reservations.dat", "wb");
    if (f_res != NULL) {
        fwrite(&nb_reservations, sizeof(int), 1, f_res);
        fwrite(reservations, sizeof(Reservation), nb_reservations, f_res);
        fclose(f_res);
    }
    
    FILE *f_fact = fopen("factures.dat", "wb");
    if (f_fact != NULL) {
        fwrite(&nb_factures, sizeof(int), 1, f_fact);
        fwrite(factures, sizeof(Facture), nb_factures, f_fact);
        fclose(f_fact);
    }
    
    printf("✅ Données sauvegardées avec succès!\n");
}

void charger_donnees() {
    FILE *f_salles = fopen("salles.dat", "rb");
    if (f_salles != NULL) {
        fread(&nb_salles, sizeof(int), 1, f_salles);
        fread(salles, sizeof(Salle), nb_salles, f_salles);
        fclose(f_salles);
    }

    FILE *f_res = fopen("reservations.dat", "rb");
    if (f_res != NULL) {
        fread(&nb_reservations, sizeof(int), 1, f_res);
        fread(reservations, sizeof(Reservation), nb_reservations, f_res);
        fclose(f_res);
    }
    
    // Charger les factures
    FILE *f_fact = fopen("factures.dat", "rb");
    if (f_fact != NULL) {
        fread(&nb_factures, sizeof(int), 1, f_fact);
        fread(factures, sizeof(Facture), nb_factures, f_fact);
        fclose(f_fact);
    }
}

void afficher_menu() {
    clearScreen();
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║                                                               ║\n");
    printf("║      SYSTÈME DE GESTION DE RÉSERVATIONS                       ║\n");
    printf("║      École Nationale des Sciences de l'Informatique           ║\n");
    printf("║                                                               ║\n");
    printf("╚═══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    printf("┌───────────────────────────────────────────────────────────────┐\n");
    printf("│                    GESTION DES SALLES                         │\n");
    printf("├───────────────────────────────────────────────────────────────┤\n");
    printf("│  1. Ajouter une salle                                         │\n");
    printf("│  2. Afficher les salles                                       │\n");
    printf("├───────────────────────────────────────────────────────────────┤\n");
    printf("│                 GESTION DES RÉSERVATIONS                      │\n");
    printf("├───────────────────────────────────────────────────────────────┤\n");
    printf("│  3. Créer une réservation                                     │\n");
    printf("│  4. Afficher les réservations                                 │\n");
    printf("├───────────────────────────────────────────────────────────────┤\n");
    printf("│                  GESTION DES FACTURES                         │\n");
    printf("├───────────────────────────────────────────────────────────────┤\n");
    printf("│  5. Générer une facture                                       │\n");
    printf("│  6. Afficher les factures                                     │\n");
    printf("├───────────────────────────────────────────────────────────────┤\n");
    printf("│                     STATISTIQUES                              │\n");
    printf("├───────────────────────────────────────────────────────────────┤\n");
    printf("│  7. Afficher les statistiques                                 │\n");
    printf("├───────────────────────────────────────────────────────────────┤\n");
    printf("│                   SAUVEGARDE & QUITTER                        │\n");
    printf("├───────────────────────────────────────────────────────────────┤\n");
    printf("│  8. Sauvegarder les données                                   │\n");
    printf("│  0. Quitter                                                   │\n");
    printf("└───────────────────────────────────────────────────────────────┘\n");
    printf("\nVotre choix: ");
    }
int main() {
int choix;

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    charger_donnees();

do {
    afficher_menu();
    scanf("%d", &choix);
    
    switch(choix) {
        case 1:
            ajouter_salle();
            break;
        case 2:
            afficher_salles();
            pause_screen();
            break;
        case 3:
            creer_reservation();
            break;
        case 4:
            afficher_reservations();
            break;
        case 5:
            generer_facture();
            break;
        case 6:
            afficher_factures();
            break;
        case 7:
            afficher_statistiques();
            break;
        case 8:
            sauvegarder_donnees();
            pause_screen();
            break;
        case 0:
            printf("\n💾 Sauvegarde automatique en cours...\n");
            sauvegarder_donnees();
            printf("\n👋 Merci d'avoir utilisé le système!\n");
            printf("Au revoir!\n\n");
            break;
        default:
            printf("\n❌ Choix invalide! Réessayez.\n");
            pause_screen();
    }
} while(choix != 0);

return 0;
}