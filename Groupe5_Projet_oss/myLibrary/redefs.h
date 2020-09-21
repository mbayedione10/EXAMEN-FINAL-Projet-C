#ifndef REDEFS_H_INCLUDED
#define REDEFS_H_INCLUDED
#include <stdio.h>
#include <string.h>

#define INPUT_FILE "C:\\Users\\mamadoumbaye\\Desktop\\IG IT\\Langage C\\Mes Projets C\\EXAMEN FINAL Projet C\\Groupe5_Projet_oss\\os-scheduler-simulator\\bin\\Debug\\processus.sta"
//"C:\\Users\\mamadoumbaye\\Desktop\\IG IT\\Langage C\\Mes Projets C\\EXAMEN FINAL Projet C\\oss-simulator-input\\os-scheduler-simulator\\bin\\Debug\\processus.sta"




#define OUTPUT_FILE "C:\\Users\\mamadoumbaye\\Desktop\\IG IT\\Langage C\\Mes Projets C\\EXAMEN FINAL Projet C\\Groupe5_Projet_oss\\os-scheduler-simulator\\bin\\Debug\\processus.sta#1"
//"C:\\Users\\mamadoumbaye\\Desktop\\IG IT\\Langage C\\Mes Projets C\\EXAMEN FINAL Projet C\\oss-simulator-input\\os-scheduler-simulator\\bin\\Debug\\processus.sta#1"




#define TAILLE_MAX_LIGNE 512
#define TAILLE_MAX_FIELD 32
#define CODE_RETOUR_OK 0
#define CODE_RETOUR_NOK -1

#define MODE_LECTURE "r"
#define MODE_ECRITURE "w"
#define SEPARATOR ','


#define BLOQUE 0
#define EN_COURS 1
#define PRET 2
#define TERMINE 3

#define NON 0
#define OUI 1

typedef char * _STRING_ ;
typedef FILE * _FICHIER_ ;

typedef struct model_thread
{
    /*
     * Caractéristiques d'un thread
     */
    // Identifiant (unique) du thread ...
    int id ;
    // Identifiant du Thread(processus?) associé ...
    // Un thread appartient toujours à un Thread(processus?) ...
    int processusId ;
    // Nom du service associé ...
    char serviceName [ TAILLE_MAX_FIELD ] ;
    // Temps total ...
    int totalTime ;
    // Temps restant ...
    int remainingTime ;
    // Etat du thread
    int state ;
    // Thread suivant ...
    struct model_thread * suivant ;
} Thread, * _P_Thread ;

typedef struct fileThread
{
    // Tete de file ...
    _P_Thread first ;
    // Queue de file ...
    _P_Thread last ;

} FileThread ;

typedef struct model_processus
{
    /*
     * Caractéristiques d'un processus
     */
    // Identifiant (unique) du processus ...
    int id ;
    // Nom du programme associé ...
    char programName [ TAILLE_MAX_FIELD ] ;
    // Temps total ...
    int totalTime ;
    // Temps restant ...
    int remainingTime ;
    // Etat du processus
    int state ;
    // Les Threads du processus ...
    FileThread fileThread ;
    // Processus suivant ...
    struct model_processus * suivant ;
} Processus, * _P_Processus ;

typedef struct fileProcessus
{
    // Tete de file ...
    _P_Processus first ;
    // Queue de file ...
    _P_Processus last ;

} FileProcessus ;

typedef struct model_ligne
{
    int lineNumber ;
    char line [ TAILLE_MAX_LIGNE ] ;
    struct model_ligne * suivant ;
} Line, * _P_Line ;

typedef struct fileLigne
{
    // Tete de file ...
    _P_Line first ;
    // Queue de file ...
    _P_Line last ;

} FileLine ;

typedef struct model_field
{
    int fieldNumber ;
    char field [ TAILLE_MAX_FIELD ] ;
    struct model_field * suivant ;
} Field, * _P_Field ;

typedef struct fileField
{
    // Tete de file ...
    _P_Field first ;
    // Queue de file ...
    _P_Field last ;

} FileField ;

#endif // REDEFS_H_INCLUDED
