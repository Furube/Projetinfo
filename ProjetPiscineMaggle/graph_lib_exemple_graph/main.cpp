#include "grman/grman.h"
#include <iostream>

#include "graph.h"

int main()
{
    char Choix_Menu ;
    char Choix_sauv;
    char Choix_graphe;
    Graph g;
    do
    {
        std::cout << "----------MENU----------"<< std::endl ;
        std::cout <<"1. Afficher un graphe \n" ;
        std::cout << "5. Quitter\n" ;

        std::cout << "Fait ton choix : \n" ;
        std::cin >> Choix_Menu ;
        std::cout <<"\n\n" ;

        switch (Choix_Menu)
        {
        case '1' :
        {
            do
            {
                std::cout <<"Quel graphe voulez vous afficher ? \n" ;
                std::cout << "1. Banquise\n" ;
                std::cout << "2. Foret\n" ;
                std::cout << "3. Savane\n" ;
                std::cout << "4. Retour\n" ;
                std::cout << "Fait ton choix : \n" ;
                std::cin >> Choix_graphe ;
                std::cout <<"\n\n" ;

                switch (Choix_graphe)
                {
                case '1' :
                {


                    std::cout << "Quel fichier voulez vous ouvrir ? \n" ;
                    std::cout << "1. Graphe de base \n" ;
                    std::cout << "2. Graphe sauvegarde \n" ;
                    std::cin >> Choix_sauv ;

                    if (Choix_sauv == '1') /// on ouvre le fichier de base
                    {
                        g.appel_graphe_banquise_base();
                    }
                    else if (Choix_sauv == '2') ///on ouvre le fichier sauvergardé
                    {
                        g.appel_graphe_banquise_sauv();
                    }

                    grman::fermer_allegro();
                    break;
                }

                case '2' :
                {


                    std::cout << "Quel fichier voulez vous ouvrir ? \n" ;
                    std::cout << "1. Graphe de base \n" ;
                    std::cout << "2. Graphe sauvegarde \n" ;
                    std::cin >> Choix_sauv ;

                    if (Choix_sauv == '1') /// on ouvre le fichier de base
                    {
                        g.appel_graphe_foret_base();
                    }
                    else if (Choix_sauv == '2') ///on ouvre le fichier sauvergardé
                    {
                        g.appel_graphe_foret_sauv();
                    }


                    grman::fermer_allegro();

                    break;
                }
                case '3' :
                {

                    std::cout << "Quel fichier voulez vous ouvrir ? \n" ;
                    std::cout << "1. Graphe de base \n" ;
                    std::cout << "2. Graphe sauvegarde \n" ;
                    std::cin >> Choix_sauv ;

                    if (Choix_sauv == '1') /// on ouvre le fichier de base
                    {
                        g.appel_graphe_savane_base();
                    }
                    else if (Choix_sauv == '2') ///on ouvre le fichier sauvergardé
                    {
                        g.appel_graphe_savane_sauv();
                    }


                    grman::fermer_allegro();

                    break;
                }

                case '4':
                {

                    break ;
                }

                }

            }
            while (Choix_graphe != '4');
            break ;
        }
        case '5':
        {

            break ;
        }
        default :
            std::cout << "Refait ton choix ! \n";


        }
    }
    while (Choix_Menu!='5');

    return 0;
}
END_OF_MAIN();


