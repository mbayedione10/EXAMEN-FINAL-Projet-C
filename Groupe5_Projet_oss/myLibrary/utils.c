#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "processus.h"


// Tableau des diférents états des processus et threads ...
const char STATE [ ][ 16 + 1 ] = { "BLOQUE", "EN_COURS", "PRET", "TERMINE" } ;

// Charge le contenu du fichier .sta fourni en paramètre et
// Renvoie une file contenant toutes les lignes du fichier
FileLine loadFileContent ( _STRING_ nomFichier, int * errorCode )
{
    _FICHIER_ fichier ;
    FileLine fileLine ;
    Line uneLigne ;
    initialiserLine ( & fileLine ) ;
    int index = 1 ;

    * errorCode = CODE_RETOUR_OK ;
    fichier = fopen ( nomFichier, MODE_LECTURE ) ;

    if ( fichier == NULL )
    {
        * errorCode = CODE_RETOUR_NOK ;
    }
    else
    {
        while ( EOF != fscanf ( fichier, "%s", uneLigne.line ) )
        {
            uneLigne.lineNumber = index ++ ;
            uneLigne.suivant = NULL ;
            enfilerLine ( & fileLine, uneLigne ) ;
        }
        fclose ( fichier ) ;
    }

    return fileLine ;
}

// -- Construit une ligne de processus dans la chaîne 'tmp_line' à partir de la structure d'un 'Processus'
void buildLineProcessus ( _P_Processus ptr_processus, _STRING_ tmp_line )
{
    char tmp_field [ TAILLE_MAX_FIELD ] = { 0 } ;
    // processus
    strcpy ( tmp_line, "processus," ) ;

    // -id
    sprintf ( tmp_field, "%d,", ptr_processus->id ) ;
    strcpy ( & tmp_line [ strlen ( tmp_line ) ], tmp_field ) ;

    // -programName
    strcpy ( & tmp_line [ strlen ( tmp_line ) ], ptr_processus->programName ) ;

    // -totalTime
    sprintf ( tmp_field, ",%d,", ptr_processus->totalTime ) ;
    strcpy ( & tmp_line [ strlen ( tmp_line ) ], tmp_field ) ;

    // -remainingTime
    sprintf ( tmp_field, "%d,", ptr_processus->remainingTime ) ;
    strcpy ( & tmp_line [ strlen ( tmp_line ) ], tmp_field ) ;

    // -state
    strcpy ( & tmp_line [ strlen ( tmp_line ) ], STATE [ ptr_processus->state ] ) ;
}

// -- Construit une ligne de thread dans la chaîne 'tmp_line' à partir de la structure d'un 'Thread'
void buildLineThread ( _P_Thread ptr_thread, _STRING_ tmp_line )
{
    char tmp_field [ TAILLE_MAX_FIELD ] = { 0 } ;
    // thread
    strcpy ( tmp_line, "thread," ) ;

    // -id
    sprintf ( tmp_field, "%d,", ptr_thread->id ) ;
    strcpy ( & tmp_line [ strlen ( tmp_line ) ], tmp_field ) ;

    // -processusId
    sprintf ( tmp_field, "%d,", ptr_thread->processusId ) ;
    strcpy ( & tmp_line [ strlen ( tmp_line ) ], tmp_field ) ;

    // -serviceName
     strcpy ( & tmp_line [ strlen ( tmp_line ) ], ptr_thread->serviceName ) ;
    // -totalTime
    sprintf ( tmp_field, ",%d,", ptr_thread->totalTime ) ;
    strcpy ( & tmp_line [ strlen ( tmp_line ) ], tmp_field ) ;

    // -remainingTime
    sprintf ( tmp_field, "%d,", ptr_thread->remainingTime ) ;
    strcpy ( & tmp_line [ strlen ( tmp_line ) ], tmp_field ) ;

    // -state
    strcpy ( & tmp_line [ strlen ( tmp_line ) ], STATE [ ptr_thread->state ] ) ;
}

// -- Construit les lignes du processus et de ses threads puis les insère dans la file des lignes ...
void insertLinesProcessus ( _P_Processus ptr_processus, FileLine * ptr_fileLine, int number )
{
    char tmp_line [ TAILLE_MAX_LIGNE ] = { 0 } ;
    int counter = 1 ;
    Line line ;
    _P_Thread p ;

    // --
    buildLineProcessus ( ptr_processus, tmp_line ) ;
    // Création de la ligne ...
    line = newLine ( number, tmp_line ) ;
    // Insertion de la ligne dans la file ...
    enfilerLine ( ptr_fileLine, line ) ;

    /************************************************************
     * Traitement des threads associés au processus courant ... *
     ************************************************************/
     for ( p = ptr_processus->fileThread.first ; p != NULL ; p = p->suivant )
     {
        tmp_line [ 0 ] = 0 ;
        buildLineThread ( p, tmp_line ) ;
        // Création de la ligne ...
        line = newLine ( counter ++, tmp_line ) ;
        // Insertion de la ligne dans la file ...
        enfilerLine ( ptr_fileLine, line ) ;
     }
}

// -- Construit une File Line (l'ensemble des lignes regroupées dans une file)
// -- à partir de la file de Processus et de leurs threads respectifs ...
void buildFileLine ( FileProcessus fileProcessus, FileLine * ptr_fileLine )
{
    _P_Processus p ;
    int counter = 1 ;

    initialiserLine ( ptr_fileLine ) ;
    p = fileProcessus.first ;
    while ( p != NULL )
    {
        insertLinesProcessus ( p, ptr_fileLine, counter ++ ) ;
        p = p->suivant ;
    }
}

// -- Sauvegarde dans un fichier 'sta#1' la file de processus à partir de la File de ligne construite
void saveFileLine ( FileLine fileLine, _STRING_ nomFichier, int * errorCode  )
{
    _FICHIER_ fichier ;
    _P_Line p ;

    * errorCode = CODE_RETOUR_OK ;
    fichier = fopen ( nomFichier, MODE_ECRITURE ) ;

    if ( fichier == NULL )
    {
        * errorCode = CODE_RETOUR_NOK ;
    }
    else
    {
        p = fileLine.first ;
        while ( p != NULL )
        {
            fprintf ( fichier, "%s\n", p->line ) ;
            p = p->suivant ;
        }

        fclose ( fichier ) ;
    }
}

//-- Affiche sur la console les lignes du fichiers contenu dans la file ...
void printFileLine ( FileLine fileLine )
{
    _P_Line p ;
    FileField fileField ;

    initialiserField ( & fileField ) ;

    if ( fileLine.first == NULL )
    {
        printf ( "Aucun Ligne dans la file !\n" ) ;
        return ;
    }

    for ( p = fileLine.first ; p != NULL ; p = p->suivant )
    {
        printf ( "\nLine %d : \n", p->lineNumber ) ;
        split ( p->line, SEPARATOR, & fileField ) ;
        printFileField ( fileField ) ;
        viderField ( & fileField ) ;
    }
}

// -- Renvoie l'indice du caractere dans la chaine ...
int getNextIndexOf ( char caractere, _STRING_ chaine )
{
    int iOf ;
    for ( iOf = 0 ; chaine [ iOf ++ ] ; )
    {
       if ( chaine [ iOf ] == caractere )
       {
           return iOf ;
       }
    }
    return -1 ;
}

// -- Extrait de la chaine 'src' la sous chaine de l'indice 'from' a l'indice 'to' et la stocke dans la chaine 'dest'
void getSubString ( _STRING_ src, int from, int to, _STRING_ dest )
{
    int i ;
    for ( i = 0 ; i < to - from + 1 ; i ++ )
    {
        dest [ i ] = src [ from + i ] ;
        dest [ i + 1 ] = 0 ;
    }
}

// -- Test le chargement d'un fichier 'sta'
void testFileLoader ( )
{
    FileLine fileLine ;
    int errorCode ;

    fileLine = loadFileContent ( INPUT_FILE, & errorCode ) ;

    switch ( errorCode )
    {
        case CODE_RETOUR_OK :
            printFileLine ( fileLine ) ;
            // Recopie dans le fichier output 'sta#1'
            saveFileLine ( fileLine, OUTPUT_FILE, & errorCode ) ;
            switch ( errorCode )
            {
                case CODE_RETOUR_OK :
                    viderLine ( & fileLine ) ;
                    break ;
                case CODE_RETOUR_NOK :
                    printf ( "Echec lors de la lecture du fichier : <%s>\n\n", OUTPUT_FILE ) ;
                    break ;
                default : ;
            }
            break ;

        case CODE_RETOUR_NOK :
            printf ( "Echec lors de la lecture du fichier : <%s>\n\n", INPUT_FILE ) ;
            break ;
        default : ;
    }

    printf ( "\nLiberation des ressources alllouees ... " ) ;
        viderLine ( & fileLine ) ;
    printf ( "done.\n" ) ;
}

// -- Test la sauvegarde dans le fichier 'sta#1' ...
void testFileSave ( )
{
    FileProcessus fileProcessus ;
    FileLine fileLine ;

    int errorCode ;
    Processus processus ;

    initialiserProcessus ( & fileProcessus ) ;
    initialiserLine ( & fileLine ) ;

    do
    {
        processus = newProcessus ( ) ;
        // TO DO
        // Ajouter un controle sur l'id
        // S'il est présent le modifier en considérant le nouvel id comme
        // la somme des id déjà présents dans la file.
        updateIdProcessus ( &fileProcessus,  &processus ) ;
        // --
        enfilerProcessus ( & fileProcessus, processus ) ;
        // --
        printf( "Processus : " ) ;
    } while ( continuer ( ) ) ;

    buildFileLine ( fileProcessus, & fileLine ) ;
    saveFileLine ( fileLine, OUTPUT_FILE, & errorCode ) ;

    switch ( errorCode )
    {
        case CODE_RETOUR_OK :
            printf ( "\nSauvegarde effectuee avec succes !\n" ) ;
            break ;

        case CODE_RETOUR_NOK :
            printf ( "Echec lors de la lecture du fichier : <%s>\n\n", OUTPUT_FILE ) ;
            break ;
        default : ;
    }

    printf ( "\nLiberation des ressources alllouees ... " ) ;
        viderProcessus ( & fileProcessus ) ;
        viderLine ( & fileLine ) ;
    printf ( "done.\n" ) ;
}

// -- Construit une structure de type 'Field'
Field newField ( int f_number, _STRING_ c_field )
{
    Field field ;

    strcpy ( field.field, c_field ) ;
    field.fieldNumber = f_number ;
    field.suivant = NULL ;

    return field ;
}

// -- Construit une structure de type 'Line'
Line newLine ( int l_number, _STRING_ c_line )
{
    Line line ;

    strcpy ( line.line, c_line ) ;
    line.lineNumber = l_number ;
    line.suivant = NULL ;

    return line ;
}
// -- line : a static allocated buffer ...
// -- Construit une chaîne dans le buffer : line, à partir de la File contenant les champs
// -- Ex.- line => 'processus,1,iexplore.exe,10,8,BLOQUE'
void buildLine ( FileField fileField, _STRING_ line )
{
    _P_Field p ;

    for ( p = fileField.first ; p != NULL ; p = p->suivant )
    {
        strcpy ( & line [ strlen ( line ) ], p->field ) ;
        if ( p->suivant != NULL  )
        {
            strcpy ( & line [ strlen ( line ) ], "," ) ;
        }
    }
}
// -- Extrait les champs d'une ligne et les stocke dans une file ... de champs ...
void split ( _STRING_ line, char separator, FileField * ptr_fileField )
{
    int from, to, count ;
    char tmp_field [ TAILLE_MAX_FIELD ] ;
    int lineLength = strlen ( line ) ;
    from = to = 0 ;
    count = 0 ;

    for ( count = 0 ; from < lineLength ; from += to + 2 )
    {
        while ( line [ from ] == separator )
        {
            enfilerField ( ptr_fileField, newField ( count, "" ) ) ;
            count ++ ;
            from ++ ;
        }
        // -
        to = getNextIndexOf ( separator, line + from ) ;
        if ( to == -1 )
        {
            to = lineLength - 1 ;
            getSubString ( line, from, from + to, tmp_field ) ;
            enfilerField ( ptr_fileField, newField ( count, tmp_field ) ) ;
        }
        else
        {
            to -- ;
            getSubString ( line, from, from + to, tmp_field ) ;
            enfilerField ( ptr_fileField, newField ( count, tmp_field ) ) ;
        }
        count ++ ;
    }
}

// -- Initialise les pointeur de tete et de queue à NULL
void initialiserLine ( FileLine * ptr_fileLine )
{
    ptr_fileLine->first = ptr_fileLine->last = NULL ;
}

// -- Ajoute à la fin de la file un nouvel élément ...
void enfilerLine ( FileLine * ptr_fileLine, Line line )
{
    _P_Line newCell ;

    newCell = ( _P_Line ) malloc ( sizeof ( Line ) ) ;
    * newCell = line ;

    if ( estVideLine ( * ptr_fileLine ) )
    {
        ptr_fileLine->first = ptr_fileLine->last = newCell ;
    }
    else
    {
        ptr_fileLine->last->suivant = newCell ;
        ptr_fileLine->last = newCell ;
    }
}

// -- Check si la file est vide ou non ...
int estVideLine ( FileLine fileLine ) // Retourne 1 si la file est vide, 0 sinon
{
    return ( fileLine.first == NULL ) ? 1 : 0 ;
}

// --  Check si la file est vide ou non ...
void viderLine (  FileLine * ptr_fileLine )
{
    while ( ! estVideLine ( * ptr_fileLine ) )
    {
        defilerLine ( ptr_fileLine ) ;
    }
}

// Supprime de la file l'élément de tête
void defilerLine ( FileLine * ptr_fileLine )
{
    _P_Line q ;

    if ( ! estVideLine ( * ptr_fileLine ) )
    {
        q = ptr_fileLine->first ;
        ptr_fileLine->first = ptr_fileLine->first->suivant ;
        if ( ptr_fileLine->first == NULL )
        {
            ptr_fileLine->last = NULL ;
        }
        free ( q ) ;
    }
}

// -- Renvoie l'élément qui se trouve en tête de file (null si file vide)
_P_Line lireTeteFileLine ( FileLine fileLine )
{
    return fileLine.first ;
}

// -- Initialise les pointeur de tete et de queue à NULL
void initialiserField ( FileField * ptr_fileField )
{
    ptr_fileField->first = ptr_fileField->last = NULL ;
}

// -- Ajoute à la fin de la file un nouvel élément ...
void enfilerField ( FileField * ptr_fileField, Field field )
{
    _P_Field newCell ;

    newCell = ( _P_Field ) malloc ( sizeof ( Field ) ) ;
    * newCell = field ;

    if ( estVideField ( * ptr_fileField ) )
    {
        ptr_fileField->first = ptr_fileField->last = newCell ;
    }
    else
    {
        ptr_fileField->last->suivant = newCell ;
        ptr_fileField->last = newCell ;
    }
}

// -- Check si la file est vide ou non ...
int estVideField ( FileField fileField ) // Retourne 1 si la file est vide, 0 sinon
{
    return ( fileField.first == NULL ) ? 1 : 0 ;
}

// -- Vide la file ... libère les ressources allouées ...
void viderField (  FileField * ptr_fileField )
{
    while ( ! estVideField ( * ptr_fileField ) )
    {
        defilerField ( ptr_fileField ) ;
    }
}

// Supprime de la file l'élément de tête
void defilerField ( FileField * ptr_fileField )
{
    _P_Field q ;

    if ( ! estVideField ( * ptr_fileField ) )
    {
        q = ptr_fileField->first ;
        ptr_fileField->first = ptr_fileField->first->suivant ;
        if ( ptr_fileField->first == NULL )
        {
            ptr_fileField->last = NULL ;
        }
        free ( q ) ;
    }
}

// -- Renvoie l'élément qui se trouve en tête de file (null si file vide)
_P_Field lireTeteFileField ( FileField fileField )
{
    return fileField.first ;
}

// -- Affiche toute la file de champ ...
void printFileField ( FileField fileField )
{
    _P_Field p ;

    for ( p = fileField.first ; p != NULL ; p = p->suivant )
    {
        printf ( "Field value [%d] = '%s'.\n", p->fieldNumber, p->field ) ;
    }
}

// -- Poursuivre le traitement ?
int continuer ( )
{
    int stop ;

    lireInt ( "Poursuivre ? 0:non 1:oui _ ", & stop ) ;

    return stop ;
}

// -- Lecture d'un entier ...
void lireInt ( char * message, int * adr_int )
{
    char entree ;
    printf ( message ) ;
    scanf ( "%d%c", adr_int, & entree ) ;
}

// -- Lecture d'une chaîne ...
void lireString ( char * message, char * adr_char )
{
    printf ( message ) ;
    gets ( adr_char ) ;
}



// -- Chargement des processus en mémoire depuis le fichier 'sta'
FileProcessus loadFileProcessus ( _STRING_ nomFichier )
{
    FileProcessus fileProcessus ;
    int errorCode ;

    // -
    FileLine fileLine = loadFileContent ( INPUT_FILE, & errorCode ) ;

    // -
    initialiserProcessus ( & fileProcessus ) ;

    switch ( errorCode )
    {
        case CODE_RETOUR_OK :
            builFileProcessus(&fileProcessus, fileLine);
            printf("\nchargement du fichier: <%s>\n\n", INPUT_FILE);
            break ;

        case CODE_RETOUR_NOK :
            printf ( "Echec lors de la lecture du fichier : <%s>\n\n", INPUT_FILE ) ;
            break ;
        default : ;
    }

    // - Libérer la file de lignes
    // On en a plus besoin ...
    viderLine ( & fileLine ) ;

    return fileProcessus ;
}


void menuDeDeroulement ()
{
    printf("\t\t\t-------------------------------------------------------\t\t\t\t\n");
    printf("\t\t\t|        Bienvenue dans le Menu de deroulement         |\t\t\t\t\n");
    printf("\t\t\t-------------------------------------------------------\t\t\t\t\n");
    printf("\n 1- Ajouter un nouveau processus\n ");
    printf("\n 2- Ajouter un thread dans un processus existant\n ");
    printf("\n 3- Visualiser la file de processus en attente \n ");
    printf("\n 4- Visualiser le processus en cours \n ");
    printf("\n 5- Traiter le processus en tete de file intéégralement\n ");
    printf("\n 6- Sauvegarde l'etat de la file actuelle dans le fichier d'extension 'sta#1'\n ");
    printf("\n 7- Exécuter le Round Robin (quantum de temps = 2s\n ");
    printf("\n 8- Bloquer le thread en cours d'execution (donc le proce0ssus associé)\n ");
    printf("\n 9- Débloquer un thread connaissant son Id et l'Id de son processus associé\n ");
    printf ("\n 10- Quitter le simulateur\n");

}

void menuPrincipal()
{
    printf("\t\t\t\tDIONE Mamadou Mbaye\n");
    printf("\t\t\t\tSALL Fatoumata Oumoul Khairy \n");
    printf("\t\t\t\tTHIAM Mame Diarra\n");
    printf("\t\t\t\t-------------------------------------------------------\t\t\t\t\n");
    printf("\t\t\t\t|         BIENVENUE DANS LE MENU PRINCIPAL             |\t\t\t\t\n");
    printf("\t\t\t\t-------------------------------------------------------\t\t\t\t\n");

    printf("\t\t--------------------------------------------------------------------------------\t\t\n");
    printf("\t\t| 1-Chargement d'un fichier de sauvegarde de l'etat des processus '.sta'...     |\t\t\n");
    printf("\t\t| 2-Demarrer une nouvelle simulation                                            |\t\t\n");
    printf("\t\t| 3-Quitter                                                                     |\t\t\n");
    printf("\t\t--------------------------------------------------------------------------------\t\t\n");
}


void sauvegarderFile ( FileProcessus * fileP )
{
    system ( "CLS" ) ;
    printf("\n\n\t ____________________ SAUVEGARDE File Processus ____________________\n\n") ;

    if ( ! estVideProcessus ( * fileP ) )
    {
        int coderetour ;
        FileLine lignes ;
        buildFileLine ( * fileP , & lignes ) ;

        saveFileLine ( lignes , OUTPUT_FILE , & coderetour ) ;

        if ( coderetour == CODE_RETOUR_OK )
            printf ("\n >> File de Processus sauvegardee avec succes !\n >> Fichier de sauvegarde : %s", OUTPUT_FILE ) ;
        else
            printf ( "\n >> Erreur lors de la sauvegarde - Code erreur : %d" , coderetour ) ;
    }

    printf("\n\n\t ____________________ Fin SAUVEGARDE PROCESSUS _____________________\n\n") ;


}

int save ( )
{
    int stop ;

    lireInt ( "Voulez-vous sauvegardez la file des processus dans le fichier? OUI 1 NON 0:_  ", & stop ) ;

    return stop ;
}


 void quitterSimulateur (FileProcessus * fileProcessus)
 {
     if (save() == 1)
     {
         sauvegarderFile(fileProcessus);
         viderProcessus(fileProcessus);
         printf("\n\tVous avez sauvegardé avant de quitter.\n");
     }
     else
     {
         viderProcessus(fileProcessus);
         printf("\n\tVous avez quitté sans sauvegardé.\n");
     }
     system("CLS");
 }

void executeRoundRobin( FileProcessus * fileProcessus)
{
    _P_Processus PointProcessus;
    _P_Thread PointThread;

    if(!estVideProcessus(*fileProcessus))
    {

        for(PointProcessus= fileProcessus->first; PointProcessus!=NULL; PointProcessus= PointProcessus->suivant)
        {

            for(PointThread=fileProcessus->first->fileThread.first; PointThread!=NULL; PointThread = PointThread->suivant)
            {
                if(PointThread->state==PRET || PointThread->state==EN_COURS)
                {
                    if (PointThread->state==PRET)
                    {
                        PointProcessus->state = EN_COURS;
                        PointThread->state = EN_COURS ;
                    }

                     printf("\n\tLe Processus est en cours...\n");
                     if(PointThread->remainingTime >= 2)
                     {
                        PointThread->remainingTime = PointThread->remainingTime - 2;

                        if(PointThread->remainingTime == 0)
                        {
                            defilerThread(&PointProcessus->fileThread);
                        }
                     }
                     else
                     {
                         PointThread->remainingTime = PointThread->remainingTime - 1;

                         defilerThread(&PointProcessus->fileThread);
                     }
                    updateTimeProcessus(*PointProcessus);
                     PointThread->state = PRET ;

                }
            }

            updateProcessus(PointProcessus);
            if(PointProcessus->remainingTime == 0)
            {
                defilerProcessus(fileProcessus);
            }
        }
    }else
    {
        printf("\nAucun processus dans la file\n");
    }

}


void Gestion_menuDeDeroulement(FileProcessus * fileProcessus)
{

    Processus processus;
    int choix;
        menuDeDeroulement();
        scanf("%i",&choix);

            switch (choix)
            {
            case 1:printf("\n Vous etes en train d'ajouter un nouveau processus\n ");



                    processus = newProcessusPret();


                    updateIdProcessus(fileProcessus,&processus);

                    updateIdThread(&processus);

                    updateProcessus(&processus);
                    enfilerProcessus(fileProcessus,processus);



                    //showProcessus(*fileProcessus);

                    Gestion_menuDeDeroulement(fileProcessus);
                    break;

            case 2:printf("\n Vous etes en train d'ajouter un thread dans un processus existant\n ");
                    AddThreadInProcessus(fileProcessus);
                    Gestion_menuDeDeroulement(fileProcessus);
                    break;


            case 3: printf("\n Vous etes en train de Visualiser la file de processus en attente \n ");
                    showProcessus(*fileProcessus);
                    Gestion_menuDeDeroulement(fileProcessus);
                    break;

            case 4:printf("\n Vous etes en train de Visualiser le processus en cours \n ");
                    showProcessusEnCours(*fileProcessus);
                    Gestion_menuDeDeroulement(fileProcessus);
                    break;

            case 5:printf("\n Traiter le processus en tete de file integralement\n");
                    traiterProcessus(fileProcessus);
                    Gestion_menuDeDeroulement(fileProcessus);
                    break;

            case 6:printf("\n Vous etes en train de Sauvegarde l’état de la file actuelle dans le fichier d’extension '.sta#1");
                    sauvegarderFile(fileProcessus);
                    Gestion_menuDeDeroulement(fileProcessus);
                    break;

            case 7:printf("\n Vous etes en train d'exécuter le Round Robin (quantum de temps = 2s)");
                    executeRoundRobin(fileProcessus);
                    Gestion_menuDeDeroulement(fileProcessus);
                    break;
            case 8: printf("\n Vous etes en train de bloquer le thread en cours d'excution\n");
                    bloqueThreadEnCours(*fileProcessus);
                    Gestion_menuDeDeroulement(fileProcessus);
                    break;
            case 9: printf("\n Vous etes en train de debloquer le thread \n");
                    debloqueThread(fileProcessus);
                    Gestion_menuDeDeroulement(fileProcessus);
                    break;


            case 10: printf("\n Vous êtes en train de quiiter le similateur\n");
                    quitterSimulateur(fileProcessus);

                    break;
            default: printf("\n\t\t\t SAISIR UN NOMBRE ENTRE 1 ET 10 SVP.\t\t\t\n");

            }


}
