#include "../myLibrary/utils.h"
#include "../myLibrary/processus.h"

#define test_Save

int main ( )
{

#ifdef test_exe
    execute () ;
#endif // execute

#ifdef test_Save
  //  testFileSave ( ) ;
#endif // testFileSave

#ifdef test_Loader
    testFileLoader ( ) ;
#endif // testFileLoader

    int choix;
   do
    {
        menuPrincipal();
        scanf("%i",&choix);
        FileProcessus fileProcessus;

        initialiserProcessus(&fileProcessus);

        switch (choix)
        {

        case 1:printf("\n Vous etes en train de charger un fichier de sauvegarde et poursuivre la simulation\n ");

            fileProcessus = loadFileProcessus(INPUT_FILE);
            Gestion_menuDeDeroulement(&fileProcessus);
            break;

        case 2:
            printf("\n Vous etes en train de démarrer une nouvelle simulation \n ");
            Gestion_menuDeDeroulement(&fileProcessus);



        break;
        case 3:printf("\n Vous etes en train de quitter la simulation\n ");



            ;break;
        default : printf("\nchoisir entre 1 et 3 svp\n");
       }
     }while (choix!= 3);


    return 0 ;
}
