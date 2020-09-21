#ifndef PROCESSUS_H_INCLUDED
#define PROCESSUS_H_INCLUDED

#include "Thread.h"

void defilerProcessus ( FileProcessus * ptr_fileProcessus ) ;
int estVideProcessus ( FileProcessus fileProcessus ) ;
void enfilerProcessus ( FileProcessus * ptr_fileProcessus, Processus processus ) ;
void initialiserProcessus ( FileProcessus * ptr_fileProcessus ) ;
void viderProcessus (  FileProcessus * ptr_fileProcessus ) ;
_P_Processus lireTeteFileProcessus ( FileProcessus fileProcessus ) ;

// ... autres utilitaires ...
Processus creerProcessus ( int idProcessus, _STRING_ programName, int totalTime ) ;
void updateProcessus ( _P_Processus ptr_processus ) ;
int getProcessusStatus ( Processus processus ) ;

Processus newProcessus ( ) ;
// -- Mise à jour de l'id du processus éventuellement si l'id est déjà utilisé par un autre processus
// -- Les threads associés sont également mis à jour ...
void updateIdProcessus ( FileProcessus *fileProcessus, _P_Processus processus ) ;

// -- Calcul la somme de tous les id des processus du système
// -- qui devient le nouvel id du processus entrainant le conflit d'id ...
int sommeIdProcessus ( FileProcessus fileProcessus ) ;
//-- creation d'un processus a l'etet PRET
Processus newProcessusPret ( );
//le temps d'execution des processus en fonction du temps d'execution des thread
//void executionTime ( _P_Processus ptr_processus );
int updateTimeProcessus(Processus processus);
void showProcessusEnAttente (FileProcessus fileProcessus);
void showProcessusEnCours (FileProcessus fileProcessus);
FileProcessus traiterProcessus(FileProcessus * fileProcessus);
void showProcessus (FileProcessus fileProcessus);
void builFileProcessus(FileProcessus *pFileProcessus, FileLine fileLine);
void AddThreadInProcessus(FileProcessus * fileProcessus);

#endif // PROCESSUS_H_INCLUDED
