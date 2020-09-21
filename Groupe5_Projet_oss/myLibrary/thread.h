#ifndef THREAD_H_INCLUDED
#define THREAD_H_INCLUDED

#include "redefs.h"

// Primitives ...
void defilerThread ( FileThread * ptr_fileThread ) ;
int estVideThread ( FileThread fileThread ) ;
void enfilerThread ( FileThread * ptr_fileThread, Thread Thread ) ;
void initialiserThread ( FileThread * ptr_fileThread ) ;
void viderThread (  FileThread * ptr_fileThread ) ;
_P_Thread lireTeteFileThread ( FileThread fileThread ) ;

// ... autres utilitaires ...
Thread creerThread ( int idProcessus, int idThread, _STRING_ programName, int totalTime ) ;
void updateThread ( _P_Thread ptr_thread ) ;
int getThreadStatus ( Thread thread ) ;
Thread newThread ( int idProcessus ) ;
void bloqueThreadEnCours(FileProcessus fileProcessus);
void updateIdThread ( _P_Processus processus ) ;
int sommeIdThread ( FileThread fileThread );
// cette fonction si un id thread n'est pas duppliqué si oui  modifié l'id du thread
int isUniqueIdThread(FileThread *fileTh, int idThread);
int SaveThreadState ( );
void debloqueThread (FileProcessus * fileprocessus);
#endif // THREAD_H_INCLUDED
