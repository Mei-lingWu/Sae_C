#include <stdio.h>
#include <assert.h>
#include <string.h>
#pragma warning(disable: 4996)

enum {
    MAX_ENTREPRISES = 50,
    MAX_MISSIONS = 500,
    MAX_SS_TR = 5,
    MAX_NOM = 31,
};

//structure d'une entreprise
typedef struct {
    char role[3]; // OP ou AG ou IN
    char nom[MAX_NOM];
    unsigned int identifiant; // l'id de l'entreprise
}Inscription;

//structure d'une mission
typedef struct {
    int identifiant_inscription;// l'id de l'entreprise qui a cree la mission
    int identifiant_mission;

    char nom[MAX_NOM]; // nom de celui qui a cree la mission
    char nom_op[MAX_NOM]; // nom de l'operateur
    double remuneration;

    int attribuee; // 0 = non attribue 1 = attribuee 2 = terminee
    char attribuee_idEnt; // l'id de l'entreprise a laquelle la mission est attribuee
    int sous_t; // nombre de sous-traitance
    int id_mission_st; // l'id de la mission fille
    char description[MAX_MISSIONS]; // echec
    int nbr_echec; //nombre d'echec de l'entreprise
    int echec_ent[MAX_ENTREPRISES]; // les entreprises qui ont echoues 

}Mission;

//prototype
void inscription(Inscription* table_insc, unsigned int* nombre_inscrit); //c1
void mission(Mission* table_mission, Inscription* table_insc, unsigned int* nombre_mission, unsigned int* nombre_inscrit); //c2
void consultation(Mission* table_mission, Inscription* table_insc, unsigned int* nombre_mission); //c3
void detail(Mission* table_mission, Inscription* table_insc); //c4
void acceptation(Mission* table_mission, Inscription* table_insc, unsigned int* nombre_inscrit, unsigned int* nombre_mission); //c5
void sous_traitance(Mission* table_mission, Inscription* table_insc, unsigned int* nombre_inscrit, unsigned int* nombre_mission); //c6
void rapport(Mission* table_mission, unsigned int* nombre_mission); //c7
void recapitulatif(Mission* table_mission, unsigned int* nombre_mission); //c8

//main
int main() {
    char type_valeur[MAX_NOM] = "";
    Inscription table_insc[MAX_ENTREPRISES];
    Mission table_mission[MAX_MISSIONS];
    unsigned int nombre_inscrit = 1;
    unsigned int nombre_mission = 1;

    do { // boucle

        scanf("%s", type_valeur);

        if (strcmp(type_valeur, "inscription") == 0) //c1
            inscription(table_insc, &nombre_inscrit);

        else if (strcmp(type_valeur, "mission") == 0) //c2
            mission(table_mission, table_insc, &nombre_mission, &nombre_inscrit);

        else if (strcmp(type_valeur, "consultation") == 0) //c3
            consultation(table_mission, table_insc, &nombre_mission);

        else if (strcmp(type_valeur, "detail") == 0) //c4
            detail(table_mission, table_insc);

        else if (strcmp(type_valeur, "acceptation") == 0) //c5
            acceptation(table_mission, table_insc, &nombre_inscrit, &nombre_mission);

        else if (strcmp(type_valeur, "sous-traitance") == 0) //c6
            sous_traitance(table_mission, table_insc, &nombre_inscrit, &nombre_mission);

        else if (strcmp(type_valeur, "rapport") == 0)//c7
            rapport(table_mission, &nombre_mission);

        else if (strcmp(type_valeur, "recapitulatif") == 0)//c8
            recapitulatif(table_mission, &nombre_mission);

    } while (strcmp(type_valeur, "exit") != 0); //c0

    return 0;
}

//c1
// fonction qui prend un role et un nom d'entreprise pour l'inscrire
void inscription(Inscription* table_insc, unsigned int* nombre_inscrit) {

    scanf("%s %s", table_insc[*nombre_inscrit - 1].role, table_insc[*nombre_inscrit - 1].nom);

    if (*nombre_inscrit > 1) {
        // si le nom existe deja dans la table
        for (unsigned int i = 0; i < *nombre_inscrit - 1; ++i) {
            if (strcmp(table_insc[*nombre_inscrit - 1].nom, table_insc[i].nom) == 0) {
                printf("Nom incorrect\n");
                return;
            }
        }
    }

    // si le role est different de ceux imposes
    if (strcmp(table_insc[*nombre_inscrit - 1].role, "AG") != 0 && strcmp(table_insc[*nombre_inscrit - 1].role, "OP") != 0 && strcmp(table_insc[*nombre_inscrit - 1].role, "IN") != 0) {
        printf("Role incorrect\n");
        return;
    }

    // si le nom fait plus de 30 caracteres
    else if ((strlen(table_insc[*nombre_inscrit - 1].nom) > MAX_NOM)) {
        printf("Nom incorrect\n");
        return;
    }

    // tout va bien
    else {
        table_insc[*nombre_inscrit - 1].identifiant = *nombre_inscrit;
        printf("Inscription realisee (%u)\n", *nombre_inscrit);
        ++(*nombre_inscrit);
        return;
    }
}

//c2
//fonction qui cree une mission, il prend l'identifiant d'inscription, le nom de celui qui cree la mission et une r mun ration
void mission(Mission* table_mission, Inscription* table_insc, unsigned int* nombre_mission, unsigned int* nombre_inscrit) {

    scanf("%d %s %lf", &table_mission[*nombre_mission - 1].identifiant_inscription, table_mission[*nombre_mission - 1].nom, &table_mission[*nombre_mission - 1].remuneration);

    // si le nom fait plus de 30 caracteres
    if ((strlen(table_mission[*nombre_mission - 1].nom) > MAX_NOM)) {
        printf("Nom incorrect\n");
        return;
    }

    // si la remuneration est negative 
    else if (table_mission[*nombre_mission - 1].remuneration < 0) {
        printf("Remuneration incorrecte\n");
        return;
    }

    for (unsigned int i = 0; i < MAX_ENTREPRISES; ++i) {

        // si l'id de l'entreprise est dans la table inscription
        if (strcmp(&table_mission[*nombre_mission - 1].identifiant_inscription, &table_insc[i].identifiant) == 0) {

            // si le role est un operateur
            if (strcmp(table_insc[i].role, "OP") == 0) {
                table_mission[*nombre_mission - 1].identifiant_mission = *nombre_mission;
                // 0 = mission non attribuee
                table_mission[*nombre_mission - 1].attribuee = 0;
                strcpy(table_mission[*nombre_mission - 1].nom_op, table_insc[table_mission[*nombre_mission - 1].identifiant_inscription - 1].nom);
                // nombre de sous traitance 
                table_mission[*nombre_mission - 1].sous_t = 0;
                table_mission[*nombre_mission - 1].id_mission_st = 0;
                table_mission[*nombre_mission - 1].nbr_echec = 0;

                printf("Mission publiee (%u)\n", *nombre_mission);

                ++*nombre_mission;
                return;
            }
            // l'entreprise n'est un OP
            printf("Identifiant incorrect\n");
            return;
        }
    }
    // l'id n'est pas dans la table inscription
    printf("Identifiant incorrect\n");
    return;
}

//c3
// fonction qui permet d'afficher toutes les mission non-attribuee
void consultation(Mission* table_mission, Inscription* table_insc, unsigned int* nombre_mission) {

    // variable verifiant si des missions sont attribuee
    int mission_attribue = 0;

    for (unsigned int i = 0; i < *nombre_mission - 1; ++i) {
        // si il y a des missions disponible 
        if (table_mission[i].attribuee == 0) {
            printf("%-3d %-30s %-30s %.2lf (%d)\n", table_mission[i].identifiant_mission, &table_mission[i].nom, &table_mission[i].nom_op, table_mission[i].remuneration, table_mission[i].sous_t);
            mission_attribue++;
        }
    }

    // si la table mission est vide ou que aucune mission n'est libre
    if (mission_attribue == 0) {
        printf("Aucune mission disponible\n");
        return;
    }
    return;
}

// c4
//fonction qui prend l'identifiant d'une mission non attribu e pour afficher ses informations
void detail(Mission* table_mission, Inscription* table_insc) {
    int id_valeur;

    scanf("%d", &id_valeur);

    for (unsigned int i = 0; i < MAX_ENTREPRISES; ++i) {

        // si l'identifiant est present dans la table mission
        if (table_mission[i].identifiant_mission == id_valeur && table_mission[i].attribuee == 0) {
            printf("%u %s %s %.2lf (%u)\n", table_mission[i].identifiant_mission, &table_mission[i].nom, &table_mission[i].nom_op, table_mission[i].remuneration, table_mission[i].sous_t);

            // affiche les echecs de cette mission
            for (unsigned int j = 0; j < table_mission[i].nbr_echec; ++j) {
                if (table_mission[i].description[j] == 1)
                    printf("Local non accessible\n");
                if (table_mission[i].description[j] == 2)
                    printf("Pas de signal dans le boitier general\n");
                if (table_mission[i].description[j] == 3)
                    printf("Recepteur defectueux\n");
            }
            return;
        }
    }

    printf("Identifiant incorrect\n");
    return;
}

// c5
// fonction qui prend un identifiant d'entreprise et l'identifiant d'une mission pour l'attribuee a celui ci
void acceptation(Mission* table_mission, Inscription* table_insc, unsigned int* nombre_inscrit, unsigned int* nombre_mission) {
    int id_ent, id_miss;

    scanf("%d %d", &id_ent, &id_miss);

    if (id_ent > 0) {
        for (unsigned int i = 0; i < *nombre_inscrit; i++) {

            // le role ne doit pas etre OP
            if (table_insc[i].identifiant == id_ent && strcmp(table_insc[i].role, "OP") == 0) {
                printf("Entreprise incorrecte\n");
                return;
            }
        }

        for (unsigned int i = 0; i < *nombre_mission; i++) {
            if (table_mission[i].identifiant_mission == id_miss) {
                // la mission ne doit pas etre deja attribuee
                if (table_mission[i].attribuee != 0) {
                    printf("Mission incorrecte\n");
                    return;
                }

                else {
                    //l'entreprise ne doit pas deja avoir echoue sur cette mission 
                    for (unsigned int j = 0; j < MAX_ENTREPRISES; j++) {
                        if (table_mission[i].echec_ent[j] == id_ent) {
                            printf("Entreprise incorrecte\n");
                            return;
                        }
                    }
                    table_mission[i].attribuee = 1;
                    table_mission[i].attribuee_idEnt = id_ent;
                    printf("Acceptation enregistree\n");
                    return;
                }
            }
        }
        printf("Mission incorrecte\n");
        return;
    }
}


//c6 
// fonction qui prend deux id (agence et mission non attribuee) pour republier la mission sous le nom de l'agence
void sous_traitance(Mission* table_mission, Inscription* table_insc, unsigned int* nombre_inscrit, unsigned int* nombre_mission) {
    int id_ent, id_miss;
    double rem;

    scanf("%d %d %lf", &id_ent, &id_miss, &rem);

    if (id_ent > 0) {

        // si l'entreprise est AG
        if (strcmp(table_insc[id_ent - 1].role, "AG") != 0) {
            printf("Entreprise incorrecte\n");
            return;
        }

        // si l'id de la mission est negative ou egal a 0
        if (id_miss < 1) {
            printf("Mission incorrecte\n");
            return;
        }

        // si la mission est libre et a moins de 5 sous-traitance
        if (table_mission[id_miss - 1].attribuee == 0 && table_mission[id_miss - 1].sous_t < MAX_SS_TR) {

            // si la remuneration est negative
            if (rem <= 0) {
                printf("Remuneration incorrecte\n");
                return;
            }

            else {
                // la mission sous traitee est attribuee
                table_mission[id_miss - 1].attribuee = 1;
                table_mission[id_miss - 1].attribuee_idEnt = id_ent;
                table_mission[id_miss - 1].id_mission_st = *nombre_mission;

                // creation de la nouvelle mission
                table_mission[*nombre_mission - 1].identifiant_inscription = table_insc[id_ent - 1].identifiant;
                table_mission[*nombre_mission - 1].identifiant_mission = *nombre_mission;
                // toujours le meme qui a cree la mission
                strcpy(table_mission[*nombre_mission - 1].nom, table_mission[id_miss - 1].nom);
                // nouvelle entreprise chargee de cette mission
                strcpy(table_mission[*nombre_mission - 1].nom_op, table_insc[id_ent - 1].nom);
                table_mission[*nombre_mission - 1].remuneration = rem;
                table_mission[*nombre_mission - 1].attribuee = 0;
                // on incremente de 1 la sous-traitance
                table_mission[*nombre_mission - 1].sous_t = table_mission[id_miss - 1].sous_t + 1;
                table_mission[*nombre_mission - 1].id_mission_st = 0;
                // erreurs de la mission
                table_mission[*nombre_mission - 1].nbr_echec = table_mission[id_miss - 1].nbr_echec;
                for (unsigned int j = 0; j < MAX_MISSIONS; ++j) {
                    table_mission[*nombre_mission - 1].description[j] = table_mission[id_miss - 1].description[j];
                }
                printf("Sous-traitance enregistree (%u)\n", *nombre_mission);
                ++*nombre_mission;
                return;
            }
        }
        printf("Mission incorrecte\n");
        return;
    }
    printf("Entreprise incorrecte\n");
    return;
}

//c7
//fonction qui prend l'identifiant d'une mission et d'un code de retour et permet de valider ou recreer une mission si echec
void rapport(Mission* table_mission, unsigned int* nombre_mission) {
    int code_retour, id_miss;
    double rem;

    scanf("%d %d", &id_miss, &code_retour);

    // si le code de retour est different de ceux imposee de 0 a 3
    if (code_retour > 3 || code_retour < 0) {
        printf("Code de retour incorrect\n");
        return;
    }

    for (unsigned int i = 0; i < *nombre_mission; i++) {

        // cherche la mission dans la table mission
        if (table_mission[i].identifiant_mission == id_miss) {

            // verifie si la mission est bien attribuee et est en derniere position de la chaine de sous traitance, 0 = mission non sous-traitee
            if (table_mission[i].attribuee == 1 && table_mission[i].id_mission_st == 0) {

                // la mission est un succes 
                if (code_retour == 0) {
                    table_mission[i].description[table_mission[i].nbr_echec] = code_retour;
                    table_mission[i].attribuee = 2; // mission terminee

                    // transmission du succes a toute la chaine de sous-traitance
                    if (table_mission[i].sous_t > 0) {
                        int a = id_miss;

                        for (unsigned int j = 0; j < table_mission[i].sous_t; ++j) {
                            for (unsigned int k = 0; k < *nombre_mission; ++k) {
                                if (table_mission[k].id_mission_st = a) {
                                    table_mission[k].description[table_mission[k].nbr_echec] = 0;
                                    table_mission[k].attribuee = 2;
                                    a = table_mission[k].identifiant_mission;
                                }
                            }
                        }

                    }
                    printf("Rapport enregistre\n");
                    return;
                }

                // echec 1
                else if (code_retour == 1) {
                    // publication de la nouvelle mission sans augmentation de la remuneration
                    rem = table_mission[i].remuneration;
                    table_mission[i].description[table_mission[i].nbr_echec] = code_retour;
                    ++table_mission[i].nbr_echec;
                    table_mission[i].attribuee = 2; //mission terminee
                    // transmission de fin a toute la chaine de sous-traitance
                    if (table_mission[i].sous_t > 0) {
                        int a = id_miss;

                        for (unsigned int j = 0; j < table_mission[i].sous_t; ++j) {
                            for (unsigned int k = 0; k < *nombre_mission; ++k) {
                                if (table_mission[k].id_mission_st = a) {
                                    table_mission[k].attribuee = 2;
                                    a = table_mission[k].identifiant_mission;
                                }
                            }
                        }
                    }
                    printf("Rapport enregistre (%u)\n", *nombre_mission);
                    // publication de la nouvelle mission
                    table_mission[*nombre_mission - 1].identifiant_mission = *nombre_mission;
                    table_mission[*nombre_mission - 1].identifiant_inscription = table_mission[i].identifiant_inscription;
                    strcpy(table_mission[*nombre_mission - 1].nom, table_mission[i].nom);
                    strcpy(table_mission[*nombre_mission - 1].nom_op, table_mission[i].nom_op);
                    table_mission[*nombre_mission - 1].sous_t = 0;
                    table_mission[*nombre_mission - 1].remuneration = rem;
                    table_mission[*nombre_mission - 1].attribuee = 0;
                    table_mission[*nombre_mission - 1].id_mission_st = 0;
                    table_mission[*nombre_mission - 1].nbr_echec = table_mission[i].nbr_echec;
                    // erreurs de la mission 
                    for (unsigned int j = 0; j < MAX_MISSIONS; ++j) {
                        table_mission[*nombre_mission - 1].description[j] = table_mission[i].description[j];
                    }
                    // id des entreprises qui ont echouees
                    for (unsigned int j = 0; j < MAX_ENTREPRISES + 1; ++j) {
                        table_mission[*nombre_mission - 1].echec_ent[j] = table_mission[i].echec_ent[j];
                    }
                    table_mission[*nombre_mission - 1].echec_ent[i] = table_mission[i].attribuee_idEnt;


                    ++*nombre_mission;
                    return;
                }

                // echec 2
                else if (code_retour == 2) {
                    // augmentation de la remuneration de 55% quand code = 2
                    rem = table_mission[i].remuneration * 1.055;
                    table_mission[i].description[table_mission[i].nbr_echec] = code_retour;
                    ++table_mission[i].nbr_echec;
                    // mission terminee
                    table_mission[i].attribuee = 2;
                    // transmission de fin a toute la chaine de sous-traitance
                    if (table_mission[i].sous_t > 0) {
                        int a = id_miss;

                        for (unsigned int j = 0; j < table_mission[i].sous_t; ++j) {
                            for (unsigned int k = 0; k < *nombre_mission; ++k) {
                                if (table_mission[k].id_mission_st = a) {
                                    table_mission[k].attribuee = 2;
                                    a = table_mission[k].identifiant_mission;
                                }
                            }
                        }
                    }
                    printf("Rapport enregistre (%u)\n", *nombre_mission);
                    // publication de la nouvelle mission
                    table_mission[*nombre_mission - 1].identifiant_mission = *nombre_mission;
                    table_mission[*nombre_mission - 1].identifiant_inscription = table_mission[i].identifiant_inscription;
                    strcpy(table_mission[*nombre_mission - 1].nom, table_mission[i].nom);
                    strcpy(table_mission[*nombre_mission - 1].nom_op, table_mission[i].nom_op);
                    table_mission[*nombre_mission - 1].sous_t = 0;
                    table_mission[*nombre_mission - 1].remuneration = rem;
                    table_mission[*nombre_mission - 1].attribuee = 0;
                    table_mission[*nombre_mission - 1].id_mission_st = 0;
                    table_mission[*nombre_mission - 1].nbr_echec = table_mission[i].nbr_echec;

                    for (unsigned int j = 0; j < MAX_MISSIONS; ++j) {
                        table_mission[*nombre_mission - 1].description[j] = table_mission[i].description[j];
                    }
                    for (unsigned int j = 0; j < MAX_ENTREPRISES; ++j) {
                        table_mission[*nombre_mission - 1].echec_ent[j] = table_mission[i].echec_ent[j];
                    }
                    table_mission[*nombre_mission - 1].echec_ent[i] = table_mission[i].attribuee_idEnt;


                    ++*nombre_mission;
                    return;
                }

                // echec 3
                else if (code_retour == 3) {
                    // augmentation de la remuneration de 40% quand code = 3
                    rem = table_mission[i].remuneration * 1.04;
                    table_mission[i].description[table_mission[i].nbr_echec] = code_retour;
                    ++table_mission[i].nbr_echec;
                    // mission terminee
                    table_mission[i].attribuee = 2;
                    // transmission de fin a toute la chaine de sous-traitance
                    if (table_mission[i].sous_t > 0) {
                        int a = id_miss;

                        for (unsigned int j = 0; j < table_mission[i].sous_t; ++j) {
                            for (unsigned int k = 0; k < *nombre_mission; ++k) {
                                if (table_mission[k].id_mission_st = a) {
                                    table_mission[k].attribuee = 2;
                                    a = table_mission[k].identifiant_mission;
                                }
                            }
                        }
                    }

                    printf("Rapport enregistre (%u)\n", *nombre_mission);
                    // publication de la nouvelle mission 
                    table_mission[*nombre_mission - 1].identifiant_mission = *nombre_mission;
                    table_mission[*nombre_mission - 1].identifiant_inscription = table_mission[i].identifiant_inscription;
                    strcpy(table_mission[*nombre_mission - 1].nom, table_mission[i].nom);
                    strcpy(table_mission[*nombre_mission - 1].nom_op, table_mission[i].nom_op);
                    table_mission[*nombre_mission - 1].sous_t = 0;
                    table_mission[*nombre_mission - 1].remuneration = rem;
                    table_mission[*nombre_mission - 1].attribuee = 0;
                    table_mission[*nombre_mission - 1].id_mission_st = 0;
                    table_mission[*nombre_mission - 1].nbr_echec = table_mission[i].nbr_echec;

                    for (unsigned int j = 0; j < MAX_MISSIONS; ++j) {
                        table_mission[*nombre_mission - 1].description[j] = table_mission[i].description[j];
                    }
                    for (unsigned int j = 0; j < MAX_ENTREPRISES; ++j) {
                        table_mission[*nombre_mission - 1].echec_ent[j] = table_mission[i].echec_ent[j];
                    }
                    table_mission[*nombre_mission - 1].echec_ent[i] = table_mission[i].attribuee_idEnt;

                    ++*nombre_mission;
                    return;
                }
            }
        }
    }

    printf("Mission incorrecte\n");
    return;
}

//c8
//fonction qui prend l'identifiant d'une entreprise et affiche toutes les missions publiees et acceptees par cette entreprise
void recapitulatif(Mission* table_mission, unsigned int* nombre_mission) {
    int id_ent;
    // table publie non acceptee, table publiee acceptee, table terminee
    int table_p_NA[MAX_MISSIONS], table_p_A[MAX_MISSIONS], table_p_T[MAX_MISSIONS];
    //table acceptee non-terminee, table acceptee terminee
    int table_a_NT[MAX_MISSIONS], table_a_T[MAX_MISSIONS];
    int indice = 0, i_p_NA = 0, i_p_A = 0, i_p_T = 0, i_a_NT = 0, i_a_T = 0;

    scanf("%d", &id_ent);

    if (id_ent > 0) {

        for (unsigned int i = 0; i < *nombre_mission; ++i) {
            // cherche si l'entreprise a publie des missions
            if (table_mission[i].identifiant_inscription == id_ent) {
                if (table_mission[i].attribuee == 0) {
                    // recolte les id de missions cree par l'entreprise non acceptee
                    table_p_NA[i_p_NA] = table_mission[i].identifiant_mission;
                    ++i_p_NA;
                }
                else if (table_mission[i].attribuee == 1) {
                    // recolte les id de missions cree par l'entreprise acceptee
                    table_p_A[i_p_A] = table_mission[i].identifiant_mission;
                    ++i_p_A;
                    /*table_a_NT[i_a_NT] = table_mission[i].identifiant_mission;
                    ++i_a_NT;*/
                }
                else if (table_mission[i].attribuee == 2) { /*table_mission[i].description[table_mission[i].nbr_echec] == 0 ||*/
                    // recolte les id de missions cree par l'entreprise terminee (echec et reussite)
                    table_p_T[i_p_T] = table_mission[i].identifiant_mission;
                    ++i_p_T;
                    /*table_a_T[i_a_T] = table_mission[i].identifiant_mission;
                    ++i_a_T;*/
                }
                ++indice;
            }
            // cherche si l'entreprise a acceptee des missions
            if (table_mission[i].attribuee_idEnt == id_ent) {
                if (table_mission[i].description[table_mission[i].nbr_echec] == 0 || table_mission[i].attribuee == 2) {
                    // recolte les id de missions acceptees par l'entreprise terminee
                    table_a_T[i_a_T] = table_mission[i].identifiant_mission;
                    ++i_a_T;
                }
                else if (table_mission[i].attribuee == 1 && table_mission[i].id_mission_st == 0) {
                    // recolte les id de missions acceptee par l'entreprise non terminee
                    table_a_NT[i_a_NT] = table_mission[i].identifiant_mission;
                    ++i_a_NT;
                }
                ++indice;
            }
        }
    }

    // si id_ent ne correspond a aucune entreprise qui a cree ou acceptee des missions
    if (indice == 0) {
        printf("Entreprise incorrecte\n");
        return;
    }

    //affichage
    if (i_p_NA != 0) {
        printf("* non attribuees\n");
        for (unsigned int i = 0; i < i_p_NA; ++i) {
            printf("%3d %-30s %-30s %.2lf (%d)\n", table_p_NA[i], &table_mission[table_p_NA[i] - 1].nom, &table_mission[table_p_NA[i] - 1].nom_op, table_mission[table_p_NA[i] - 1].remuneration, table_mission[table_p_NA[i] - 1].sous_t);
        }
    }
    if (i_p_A != 0) {
        printf("* attribuees\n");
        for (unsigned int i = 0; i < i_p_A; ++i) {
            printf("%3d %-30s %-30s %.2lf (%d)\n", table_p_A[i], &table_mission[table_p_A[i] - 1].nom, &table_mission[table_p_A[i] - 1].nom_op, table_mission[table_p_A[i] - 1].remuneration, table_mission[table_p_A[i] - 1].sous_t);
        }
    }
    if (i_p_T != 0) {
        printf("* terminees\n");
        for (unsigned int i = 0; i < i_p_T; ++i) {
            printf("%3d %-30s %-30s %.2lf (%d)\n", table_p_T[i], &table_mission[table_p_T[i] - 1].nom, &table_mission[table_p_T[i] - 1].nom_op, table_mission[table_p_T[i] - 1].remuneration, table_mission[table_p_T[i] - 1].sous_t);
        }
        return;
    }
    if (i_a_NT != 0) {
        printf("* a realiser\n");
        for (unsigned int i = 0; i < i_a_NT; ++i) {
            printf("%3d %-30s %-30s %.2lf (%d)\n", table_a_NT[i], &table_mission[table_a_NT[i] - 1].nom, &table_mission[table_a_NT[i] - 1].nom_op, table_mission[table_a_NT[i] - 1].remuneration, table_mission[table_a_NT[i] - 1].sous_t);
        }
    }
    if (i_a_T != 0) {
        printf("* realisees\n");
        for (unsigned int i = 0; i < i_a_T; ++i) {
            printf("%3d %-30s %-30s %.2lf (%d)\n", table_a_T[i], &table_mission[table_a_T[i] - 1].nom, &table_mission[table_a_T[i] - 1].nom_op, table_mission[table_a_T[i] - 1].remuneration, table_mission[table_a_T[i] - 1].sous_t);
        }
    }
    return;
}