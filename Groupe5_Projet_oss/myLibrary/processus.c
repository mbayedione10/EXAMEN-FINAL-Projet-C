#include "processus.h"
#include "utils.h"

#include <stdlib.h>
#include <stdio.h>

void initialiserProcessus ( FileProcessus * ptr_fileProcessus )
{
    ptr_fileProcessus->first = ptr_fileProcessus->last = NULL ;
}
// --
void enfilerProcessus ( FileProcessus * ptr_fileProcessus, Processus Processus )
{
    _P_Processus newCell ;

    newCell = ( _P_Processus ) malloc ( sizeof ( Processus ) ) ;
    * newCell = Processus ;

    if ( estVideProcessus ( * ptr_fileProcessus ) )
    {
        ptr_fileProcessus->first = ptr_fileProcessus->last = newCell ;
    }
    else
    {
        ptr_fileProcessus->last->suivant = newCell ;
        ptr_fileProcessus->last = newCell ;
    }
}
// --
int estVideProcessus ( FileProcessus fileProcessus ) // Retourne 1 si la file est vide, 0 sinon
{
    return ( fileProcessus.first == NULL ) ? 1 : 0 ;
}
// --
void viderProcessus (  FileProcessus * ptr_fileProcessus )
{
    while ( ! estVideProcessus ( * ptr_fileProcessus ) )
    {
        defilerProcessus ( ptr_fileProcessus ) ;
    }
}
// --
void defilerProcessus ( FileProcessus * ptr_fileProcessus )
{
    _P_Processus q ;

    if ( ! estVideProcessus ( * ptr_fileProcessus ) )
    {
        q = ptr_fileProcessus->first ;
        ptr_fileProcessus->first = ptr_fileProcessus->first->suivant ;
        if ( ptr_fileProcessus->first == NULL )
        {
            ptr_fileProcessus->last = NULL ;
        }
        // --
        // Vider également la file des threads associés au processus ...
        viderThread ( & q->fileThread ) ;
        // --
        free ( q ) ;
    }
}
// --
_P_Processus lireTeteFileProcessus ( FileProcessus fileProcessus )
{
    return fileProcessus.first ;
}

// --
Processus newProcessus ( )
{
    Processus processus ;
    Thread thread ;
    // Ajout un contrôle sur l'id : id > 0
    do
    {
        lireInt ( "Id Processus : ", & processus.id ) ;
        if (processus.id<=0)
        {
            printf("Entrer un nombre positif svp.\n");
        }
    }while (processus.id<=0);

    // --
    lireString ( "Program name : ", processus.programName ) ;

    // Ajout d'un contrôle sur le temps d'exécution total : totalTime > 0
    do
    {
        lireInt ( "Total time : ", & processus.totalTime ) ;
        if (processus.totalTime<=0)
        {
            printf("Entrer un nombre positif svp.\n");
        }

    }while (processus.totalTime<=0);


    // Ajout d'un contrôle sur le temps d'exécution restant : remainingTime > 0 ET remainingTime < totalTime
    do
    {
        lireInt ( "Remaining time : ", & processus.remainingTime ) ;
        if (processus.remainingTime<=0 && processus.remainingTime>=processus.totalTime)
        {
            printf(" veillez a ce que remainingTime > 0 ET remainingTime < totalTime\n");
        }
    }while (processus.remainingTime<=0 && processus.remainingTime>=processus.totalTime);


    // Ajout un contrôle sur l'état du processus ( state must be : BLOQUE (0) or PRET(2) )
    do
    {
        lireInt ( "State : ", & processus.state ) ;
        if (processus.state<0 || processus.state!=BLOQUE || processus.state!=PRET || processus.state>4)
        {
            printf(" veillez a ce que state must be : BLOQUE (0) or PRET(2) \n");
        }

    }while (processus.state<0  || processus.state!=BLOQUE || processus.state!=PRET || processus.state>4);


    // --
    processus.suivant = NULL ;

    /*********************************************************
     * Lecture des threads associés au processus courant ... *
     *********************************************************/
     initialiserThread ( & processus.fileThread ) ;

     do
     {

         // Implémentation de la fonction newThread
         thread = newThread ( processus.id ) ;
         updateIdThread(&processus);
         enfilerThread ( & processus.fileThread, thread ) ;
         printf( "\tThread : " ) ;
     } while ( continuer ( ) ) ;
     processus.totalTime = updateTimeProcessus(processus);

     return processus ;
}
// --
void updateIdProcessus ( FileProcessus * fileProcessus, _P_Processus processus )
{
    _P_Processus p ;

    if(!estVideProcessus(* fileProcessus))
    {
        for ( p = fileProcessus->first ; p != NULL ; p = p->suivant )
        {
            if ( p->id == processus->id )
                {

                processus->id = sommeIdProcessus ( *fileProcessus ) ;
                updateIdThread ( processus ) ;


                }
        }


    }
}
int updateTimeProcessus(Processus processus)
{

    _P_Thread p ;
    int somme=0;
        for ( p = processus.fileThread.first ; p != NULL ; p = p->suivant )
        {
                somme =+ p->totalTime;
        }
         return somme ;
}

// --
int sommeIdProcessus ( FileProcessus fileProcessus )
{
    _P_Processus p ;
     int somme = 0 ;

    for ( p = fileProcessus.first ; p != NULL ; p = p->suivant )
    {

        somme += p->id ;
    }
    return somme ;
}
//-- creation d'un processus a l'etet PRET
Processus newProcessusPret ( )
{
    Processus processus ;
    Thread thread ;
    // Ajout un contrôle sur l'id : id > 0
    do
    {
        lireInt ( "Id Processus : ", & processus.id ) ;
        if (processus.id<=0)
        {
            printf("Entrer un nombre positif svp.\n");
        }
    }while (processus.id<=0);

    // --
    lireString ( "Program name : ", processus.programName ) ;
    // Ajout d'un contrôle sur le temps d'exécution total : totalTime > 0
    do
    {
        lireInt ( "Total time : ", & processus.totalTime ) ;
        if (processus.totalTime<=0)
        {
            printf("Entrer un nombre positif svp.\n");
        }

    }while (processus.totalTime<=0);


    // Ajout d'un contrôle sur le temps d'exécution restant : remainingTime > 0 ET remainingTime < totalTime
    do
    {
        lireInt ( "Remaining time : ", & processus.remainingTime ) ;
        if (processus.remainingTime<=0 || processus.remainingTime>processus.totalTime)
        {
            printf(" veillez a ce que remainingTime > 0 ET remainingTime < totalTime\n");
        }
    }while (processus.remainingTime<=0 || processus.remainingTime>=processus.totalTime);


    //mettre l'etat du processus a PRET

    processus.state = PRET;

    // --
    processus.suivant = NULL ;

    /*********************************************************
     * Lecture des threads associés au processus courant ... *
     *********************************************************/
     initialiserThread ( & processus.fileThread ) ;

     do
     {

         // Implémentation de la fonction newThread
         thread = newThread ( processus.id ) ;
         enfilerThread ( & processus.fileThread, thread ) ;
         printf( "\tThread : " ) ;
     } while ( continuer ( ) ) ;
    // processus.totalTime = updateTimeProcessus(processus);

     return processus ;
}

void updateProcessus ( _P_Processus ptr_processus )
{
    int totalTime = 0, remindTime = 0;
    _P_Thread p;

    for (p = ptr_processus->fileThread.first;p!=NULL; p=p->suivant)
    {
        remindTime = remindTime + p->remainingTime;
        totalTime = totalTime + p->totalTime;
    }
    ptr_processus->remainingTime = remindTime;
    ptr_processus->totalTime = totalTime;
}


void showProcessusEnAttente (FileProcessus fileProcessus)
{
    _P_Processus p;
    _P_Thread th;

    for (p = fileProcessus.first;p!=NULL;p=p->suivant)
    {
        if (p->state==PRET)
        {
            printf("Id processus: %i\n",p->id);
            printf("ProgramName: %s\n",p->programName);
            printf("remainingTime: %i\n",p->remainingTime);
            printf("totalTime: %i\n",p->totalTime);


            for(th=p->fileThread.first;th!=NULL;th=th->suivant)
            {
                printf("\tId Thread: %i\n",th->id);
                printf("\tServiceName: %s\n",th->serviceName);
                printf("\tremainingTime du thread: %i\n",th->remainingTime);
                printf("\ttotalTimedu thread: %i\n",th->totalTime);
                printf("\tState du thread: %i\n",th->state);
            }
        }

       // gets(p->fileThread);

    }
}

void showProcessusEnCours (FileProcessus fileProcessus)
{
    _P_Processus p;
    _P_Thread th;

    for (p = fileProcessus.first;p!=NULL;p=p->suivant)
    {
        if (p->state==EN_COURS)
        {
            printf("\n____________________la file de processus et ses threads associes____________________\n");
            printf("Id processus: %i\n",p->id);
            printf("ProgramName: %s\n",p->programName);
            printf("remainingTime: %i\n",p->remainingTime);
            printf("totalTime: %i\n",p->totalTime);


            for(th=p->fileThread.first;th!=NULL;th=th->suivant)
            {
                printf("\n\tId Thread: %i\n",th->id);
                printf("\tServiceName: %s\n",th->serviceName);
                printf("\tremainingTime du thread: %i\n",th->remainingTime);
                printf("\ttotalTimedu thread: %i\n",th->totalTime);
                printf("\tState du thread: %i\n\n",th->state);
            }
        }
        else
        {
            printf("\nLe processus n'est pas en Cours\n");
        }

    }
}
//
FileProcessus traiterProcessus(FileProcessus * fileProcessus)
{
    _P_Processus ptrSurProcessus;
    if (!estVideProcessus(*fileProcessus))
    {
        lireTeteFileProcessus(*fileProcessus);
        ptrSurProcessus = fileProcessus->first;
        switch (ptrSurProcessus->state)
        {

            case PRET:ptrSurProcessus->state = EN_COURS;
                ptrSurProcessus->fileThread.first->state = EN_COURS;
                printf("\n\tLe processus est mis en cours.\n");
                break;
            case EN_COURS: defilerProcessus(fileProcessus);
                    //updateProcessus(ptrSurProcessus);
                printf("\n\tLe processus a ete defile.\n");
                break;
            case BLOQUE:  ptrSurProcessus->state = PRET;
                ptrSurProcessus->fileThread.first->state = PRET;
                printf("\n\tLe processus est pret.\n");
                break;
        }
    }
    return * fileProcessus;

}

void showProcessus (FileProcessus fileProcessus)
{
    _P_Processus p;
    _P_Thread th;
    if (!estVideProcessus(fileProcessus))
        {
                for (p = fileProcessus.first;p!=NULL;p=p->suivant)
                {
                        printf("\n____________________la file de processus et ses threads associes____________________\n");
                        printf("\nId processus: %i\n",p->id);
                        printf("ProgramName: %s\n",p->programName);
                        printf("remainingTime: %i\n",p->remainingTime);
                        printf("totalTime: %i\n",p->totalTime);
                        printf("State:%i\n\n\n",p->state);


                        for(th=p->fileThread.first;th!=NULL;th=th->suivant)
                        {
                            printf("\tId Thread: %i\n",th->id);
                            printf("\tServiceName: %s\n",th->serviceName);
                            printf("\tremainingTime du thread: %i\n",th->remainingTime);
                            printf("\ttotalTime du thread: %i\n",th->totalTime);
                            printf("\tState du thread: %i\n\n\n",th->state);
                        }
                    }

        }
        else
        {
            printf("\nla file est vide\n");
        }


}


//
void builFileProcessus(FileProcessus *pFileProcessus, FileLine fileLine)
{

    FileField fileField;
    _P_Field fd;
    initialiserField(&fileField);
    _P_Line p;
    initialiserProcessus(pFileProcessus);
    Processus processus;
    Thread thread;
    FileThread fileThread;
    initialiserThread(&fileThread);

    for(p = fileLine.first; p!=NULL; p= p->suivant)
    {
        split(p->line,SEPARATOR, &fileField);

        if(strstr(p->line , "processus")){
            //si ligne processus
            initialiserThread(&fileThread);
            //processus.id = atoi(*(fd->field+1));
            for(fd= fileField.first;fd!=NULL; fd= fd->suivant){
                //on a 6 champs et autant de cellules
                if(fd->fieldNumber==1){
                    processus.id = atoi(fd->field);

                }
                if(fd->fieldNumber==2){
                    strcpy(processus.programName,fd->field);

                }
                if(fd->fieldNumber==3){
                    processus.totalTime= atoi(fd->field);

                }
                if(fd->fieldNumber==4){
                    processus.remainingTime= atoi(fd->field);

                }
                if(fd->fieldNumber==5){
                    if(strstr(fd->field,"PRET")){
                        processus.state= 2;
                    }
                    if(strstr(fd->field,"BLOQUE")){
                        processus.state= 0;
                    }
                    if(strstr(fd->field,"EN_COURS")){
                        processus.state= 1;
                    }
                    if(strstr(fd->field,"TERMINE")){
                        processus.state= 3;
                    }

                }
                processus.suivant= NULL;


               processus.fileThread =fileThread;

            }
            //
            enfilerProcessus(pFileProcessus,processus);
        }else{
            //si ligne thread
            if(strstr(p->line, "thread")){
                for(fd= fileField.first;fd!=NULL; fd= fd->suivant){
                //on a 6 champs et autant de cellules
                if(fd->fieldNumber==1){
                    thread.id = atoi(fd->field);
                }
                if(fd->fieldNumber==2){
                    thread.processusId = atoi(fd->field);
                    //printf("%d",thread.processusId);
                }
                if(fd->fieldNumber==3){
                    strcpy(thread.serviceName,fd->field);
                    //puts(thread.serviceName);
                }
                if(fd->fieldNumber==4){
                    thread.totalTime= atoi(fd->field);
                    //printf("%d", thread.totalTime);
                }
                if(fd->fieldNumber==5){
                    thread.remainingTime= atoi(fd->field);
                    //printf("%d", thread.remainingTime);
                }
                if(fd->fieldNumber==6){
                    if(strstr(fd->field,"PRET")){
                        thread.state= 2;
                    }
                    if(strstr(fd->field,"BLOQUE")){
                        thread.state= 0;
                    }
                    if(strstr(fd->field,"EN_COURS")){
                        thread.state= 1;
                    }
                    if(strstr(fd->field,"TERMINE")){
                        thread.state= 3;
                    }
                     //printf("%d", thread.state);
                }
                thread.suivant= NULL;
            }
                 enfilerThread(&fileThread,thread);
                 pFileProcessus->last->fileThread = fileThread;
            }
        }
    viderField(&fileField);
    }
}

void AddThreadInProcessus(FileProcessus * fileProcessus)
{
        _P_Processus p ;

        Thread monThread;

        p = fileProcessus->first ;
        if (!estVideProcessus(*fileProcessus))
        {
            if(p->fileThread.first->state == EN_COURS)
            {
                monThread = newThread ( p->id ) ;

                enfilerThread(  &p->fileThread , monThread) ;

                updateIdThread(p);

                updateProcessus(p);


            }
            else
            {
                printf("\n\tle processus en tete n'est pas en cours.\n");
            }

        }
       else
        {
            printf ( "\n\tla file est vide.\n" ) ;
        }
}
