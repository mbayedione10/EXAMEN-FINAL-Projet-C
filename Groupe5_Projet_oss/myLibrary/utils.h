#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include "redefs.h"

// Charge le contenu du fichier .sta fourni en paramètre et
// Renvoie une file contenant toutes les lignes du fichier
FileLine loadFileContent ( _STRING_ nomFichier, int * errorCode ) ;

//-- Affiche sur la console les lignes du fichiers contenu dans la file ...
void printFileLine ( FileLine fileLine ) ;

// -- Renvoie l'indice du caractere dans la chaine ...
int getNextIndexOf ( char caractere, _STRING_ chaine ) ;

// -- Extrait de la chaine 'src' la sous chaine de l'indice 'from' a l'indice 'to' et la stocke dans la chaine 'dest'
void getSubString ( _STRING_ src, int from, int to, _STRING_ dest ) ;

// -- Test le chargement d'un fichier 'sta'
void testFileLoader ( ) ;

// -- Extrait les champs d'une ligne et les stocke dans une file ... de champs ...
void split ( _STRING_ line, char separator, FileField * ptr_fileField ) ;
//
// --
// Primitives File Field ...
// --
// Supprime de la file l'élément de tête
void defilerField ( FileField * ptr_fileField ) ;

// -- Check si la file est vide ou non ...
int estVideField ( FileField fileField ) ;

// -- Ajoute à la fin de la file un nouvel élément ...
void enfilerField ( FileField * ptr_fileField, Field field ) ;

// -- Initialise les pointeur de tete et de queue à null
void initialiserField ( FileField * ptr_fileField ) ;

// -- Vide la file ... libère les ressources allouées ...
void viderField (  FileField * ptr_fileField ) ;

// -- Renvoie l'élément qui se trouve en tête de file (null si file vide)
_P_Field lireTeteFileField ( FileField fileField ) ;

// -- Construit une structure de type 'Field'
Field newField ( int f_number, _STRING_ c_field ) ;

// -- Affiche toute la file de champ ...
void printFileField ( FileField fileField ) ;
//
// --
// Primitives File Line ...
// --
// Supprime de la file l'élément de tête
void defilerLine ( FileLine * ptr_fileLine ) ;

// --  Check si la file est vide ou non ...
int estVideLine ( FileLine fileLine ) ;

// -- Ajoute à la fin de la file un nouvel élément ...
void enfilerLine ( FileLine * ptr_fileLine, Line line ) ;

// -- Initialise les pointeur de tete et de queue à NULL
void initialiserLine ( FileLine * ptr_fileLine ) ;

// -- Vide la file ... libère les ressources allouées ...
void viderLine (  FileLine * ptr_fileLine ) ;

// -- Renvoie l'élément qui se trouve en tête de file (null si file vide)
_P_Line lireTeteFileLine ( FileLine fileLine ) ;

// -- Construit une structure de type 'Line'
Line newLine ( int l_number, _STRING_ c_line ) ;
// --
// -- line : a static allocated buffer ...
// -- Construit une chaîne, buffer : line, à partir de la File contenant les champs
// -- Ex.- line => 'processus,1,iexplore.exe,10,8,BLOQUE'
void buildLine ( FileField fileField, _STRING_ line ) ;

// -- Construit une File Line (l'ensemble des lignes regroupées dans une file)
// -- à partir de la file de Processus et de leurs threads respectifs ...
void buildFileLine ( FileProcessus fileProcessus, FileLine * ptr_fileLine ) ;

// -- Sauvegarde dans un fichier 'sta#1' la file de processus à partir de la File de ligne construite
void saveFileLine ( FileLine fileLine, _STRING_ nomFichier, int * errorCode  ) ;

// -- Construit les lignes du processus et de ses threads puis les insère dans la file des lignes ...
void insertLinesProcessus ( _P_Processus ptr_processus, FileLine * ptr_fileLine, int number ) ;

// -- Construit une ligne de thread dans la chaîne 'tmp_line' à partir de la structure d'un 'Thread'
void buildLineThread ( _P_Thread ptr_thread, _STRING_ tmp_line ) ;

// -- Construit une ligne de processus dans la chaîne 'tmp_line' à partir de la structure d'un 'Processus'
void buildLineProcessus ( _P_Processus ptr_processus, _STRING_ tmp_line ) ;

// -- Poursuivre le traitement ?
int continuer ( ) ;

// -- Test la sauvegarde dans le fichier 'sta#1' ...
void testFileSave ( ) ;

// -- Lecture d'un entier ...
void lireInt ( char * message, int * adr_int ) ;

// -- Lecture d'une chaîne ...
void lireString ( char * message, char * adr_char ) ;

// -- Chargement des processus en mémoire depuis le fichier 'sta'
FileProcessus loadFileProcessus ( _STRING_ nomFichier ) ;
void menuDeDeroulement ();
void menuPrincipal();
void Gestion_menuDeDeroulement(FileProcessus *fileProcessus);
void sauvegarderFile ( FileProcessus * fileP );
void executeRoundRobin( FileProcessus * fileProcessus);
FileProcessus loadFileProcessus1 ( FileProcessus * fileProcessus );
int save ( );
 void quitterSimulateur (FileProcessus *fileProcessus);

#endif // UTILS_H_INCLUDED
