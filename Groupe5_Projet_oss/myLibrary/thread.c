#include <stdlib.h>
#include "utils.h"
#include "thread.h"
#include "processus.h"

void initialiserThread ( FileThread * ptr_fileThread )
{
    ptr_fileThread->first = ptr_fileThread->last = NULL ;
}
// -
void enfilerThread ( FileThread * ptr_fileThread, Thread Thread )
{
    _P_Thread newCell ;

    newCell = ( _P_Thread ) malloc ( sizeof ( Thread ) ) ;
    * newCell = Thread ;

    if ( estVideThread ( * ptr_fileThread ) )
    {
        ptr_fileThread->first = ptr_fileThread->last = newCell ;
    }
    else
    {
        ptr_fileThread->last->suivant = newCell ;
        ptr_fileThread->last = newCell ;
    }
}
// -
int estVideThread ( FileThread fileThread ) // Retourne 1 si la file est vide, 0 sinon
{
    return ( fileThread.first == NULL ) ? 1 : 0 ;
}
// -
void viderThread (  FileThread * ptr_fileThread )
{
    while ( ! estVideThread ( * ptr_fileThread ) )
    {
        defilerThread ( ptr_fileThread ) ;
    }
}
// -
void defilerThread ( FileThread * ptr_fileThread )
{
    _P_Thread q ;

    if ( ! estVideThread ( * ptr_fileThread ) )
    {
        q = ptr_fileThread->first ;
        ptr_fileThread->first = ptr_fileThread->first->suivant ;
        if ( ptr_fileThread->first == NULL )
        {
            ptr_fileThread->last = NULL ;
        }
        free ( q ) ;
    }
}
// -
_P_Thread lireTeteFileThread ( FileThread fileThread )
{
    return fileThread.first ;
}
// --
void updateIdThread ( _P_Processus processus )
{
     _P_Thread p ;

   /*// printf("%d\n",processus->fileThread.first->id);
     //printf("%d\n",processus->fileThread.last->id);*/

    for ( p = processus->fileThread.first ; p->suivant != NULL ; p = p->suivant )
    {
        p->processusId= processus->id;
        if ( processus->fileThread.last->id == p->id)
        {
            //mise à jour de l'id du thread
            processus->fileThread.last->id = sommeIdThread( processus->fileThread );
        }

    }

}

// cette fonction si un id thread n'est pas duppliqué si oui  modifié l'id du thread
int isUniqueIdThread(FileThread *fileTh, int idThread)
{
    		// Chercher un id dupliqué dans la file
    		_P_Thread th;
    		int nombre;
    		nombre=0;

    		for(th= fileTh->first;((th!=NULL));th=th->suivant)
            {
        		if(th->id==idThread)
        		{
            		nombre+=1;
        		}
    		}
    		return nombre;
}


int sommeIdThread ( FileThread fileThread )
{
    _P_Thread p ;
     int somme = 0 ;

    for ( p = fileThread.first ; p != NULL ; p = p->suivant )
    {
        somme += p->id ;
    }
    return somme ;
}
// --
Thread newThread ( int idProcessus )
{
    Thread thread ;

   // thread.processusId = processus.id;
    // Ajout un contrôle sur l'id : id > 0
    do
    {
        lireInt ( "\tId Thread : ", & thread.id ) ;
        if (thread.id<=0)
        {
            printf("Entrer un nombre positif svp.\n");
        }
    }while (thread.id<=0);



    // --
    thread.processusId = idProcessus ;

     // --
    lireString ( "\tService name : ", thread.serviceName ) ;


    // Ajout un contrôle sur le temps d'exécution total : totalTime > 0
    do
    {
        lireInt ( "\tTotal time : ", & thread.totalTime ) ;

        if (thread.totalTime<=0)
        {
            printf("Entrer un nombre positif svp.\n");
        }
    }while (thread.totalTime<=0);



    // Ajout un contrôle sur le temps d'exécution restant : remainingTime > 0 ET remainingTime < totalTime
    do
    {
        lireInt ( "\tRemaining time : ", & thread.remainingTime ) ;
        if (thread.remainingTime<=0 || thread.remainingTime>thread.totalTime)
        {
            printf(" veillez a ce que remainingTime > 0 ET remainingTime < totalTime\n");
        }
    }while (thread.remainingTime<=0 || thread.remainingTime>thread.totalTime);


    // Ajout un contrôle sur l'état du thread ( state must be : BLOQUE (0) or PRET(2) )
    ///revoir les etats et leurs conditions
    do
    {
        thread.state = SaveThreadState();
        if((thread.state != 2 && thread.state != 0))
        {
            printf("\n\tVeillez a ce que  state soit : BLOQUE (0) ou PRET(2)  \n") ;
        }


    }while((thread.state != 2 && thread.state != 0)) ;

    // --
    thread.suivant = NULL ;

    return thread ;
}

int SaveThreadState ( )
{
    int mon_entier;
    int test = 0;
    char enter ;
    while(test!=1)
    {
        printf("\tState :_ ");
        scanf ("%d%c",&mon_entier , &enter);
        //La limite superieur est aleatoire
        if (mon_entier >-1 && mon_entier < 1000000)
        {
            test = 1;
        }
        else
        {
            printf("\tErreur veuillez recommencer...");
            scanf ("%*[^\n]");
            getchar ();

        }
    }

    return mon_entier;
}


int getThreadStatus ( Thread thread )
{
    return thread.state;
}

void bloqueThreadEnCours(FileProcessus fileProcessus)
{

           fileProcessus.first->fileThread.first->state = BLOQUE;
            fileProcessus.first->state =BLOQUE;
            printf("le 1er processus et le 1er thread sont bloque");
}
void debloqueThread (FileProcessus * fileprocessus)
{
        Thread thread;
        Processus processus;
        _P_Processus p;
        _P_Thread th;
        lireInt("\tId processus : ", &processus.id);
        for (p=fileprocessus->first; p!=NULL; p=p->suivant )
        {

            if (processus.id == p->id)
            {
                lireInt ( "\tId Thread associe : ", & thread.id) ;
                for (th=fileprocessus->first->fileThread.first; th!=NULL; th=th->suivant )
                {
                     if (thread.id == th->id)
                     {
                         p->state = PRET;
                         th->state = PRET;
                         printf("le  processus et le  thread sont debloque");
                     }
                }


            }
            else
            {
                printf("\nl'id entré n'est pas dans la file des processus.\n");
            }

        }

}

