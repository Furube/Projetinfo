#include "grman/grman.h"
#include <iostream>

#include "graph.h"

int main()
{
    /*char Choix_Menu ;
    char Choix_sauv;
    char Choix_graphe;
    Graph g;
    do
    {
        std::cout << "----------MENU----------"<< std::endl ;
        std::cout <<"1. Afficher un graphe \n" ;
        std::cout << "2. Quitter\n" ;

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
                std::cout << "Fait ton choix : \n" ;
                std::cin >> Choix_graphe ;
                std::cout <<"\n\n" ;

                switch (Choix_graphe)
                {
                case '1' :
                {

                    if () ///si le fichier sauv n'est pas vide :
                    {
                        do
                        {

                            std::cout << "Il y a un fichier sauvegarde. Voulez vous l'ouvrir ? \n" ;
                            std::cout << "1. Oui \n" ;
                            std::cout << "2. Non \n" ;
                            std::cin >> Choix_sauv ;

                            if (Choix_sauv == '2') /// on ouvre le fichier de base
                            {
                                g.appel_graphe_banquise();
                            }
                            else if (Choix_sauv == '1') ///on ouvre le fichier sauvergardé
                            {

                            }
                        }
                        while ((Choix_sauv != '1') && (Choix_sauv != '2'));

                        grman::fermer_allegro();
                    }
                    else
                    {
                        g.appel_graphe_banquise();
                    }
                    break;
                }
                case '2' :
                {
                    if () ///si le fichier sauv n'est pas vide :
                    {
                        do
                        {

                            std::cout << "Il y a un fichier sauvegarde. Voulez vous l'ouvrir ? \n" ;
                            std::cout << "1. Oui \n" ;
                            std::cout << "2. Non \n" ;
                            std::cin >> Choix_sauv ;

                            if (Choix_sauv == '2') /// on ouvre le fichier de base
                            {
                                g.appel_graphe_foret();
                            }
                            else if (Choix_sauv == '1') ///on ouvre le fichier sauvergardé
                            {

                            }
                        }
                        while ((Choix_sauv != '1') && (Choix_sauv != '2'));

                        grman::fermer_allegro();
                    }
                    else
                    {
                        g.appel_graphe_foret();
                    }
                    break;
                }
                case '3' :
                {
                    if () ///si le fichier sauv n'est pas vide :
                    {
                        do
                        {

                            std::cout << "Il y a un fichier sauvegarde. Voulez vous l'ouvrir ? \n" ;
                            std::cout << "1. Oui \n" ;
                            std::cout << "2. Non \n" ;
                            std::cin >> Choix_sauv ;

                            if (Choix_sauv == '2') /// on ouvre le fichier de base
                            {
                                g.appel_graphe_savane();
                            }
                            else if (Choix_sauv == '1') ///on ouvre le fichier sauvergardé
                            {

                            }
                        }
                        while ((Choix_sauv != '1') && (Choix_sauv != '2'));

                        grman::fermer_allegro();
                    }
                    else
                    {
                        g.appel_graphe_savane();
                    }
                    break;
                }
                }
            }
            while ((Choix_graphe != '1')&&(Choix_graphe != '2')&&(Choix_graphe != '3'));
            break ;
        }
        case '2':
        {

            break ;
        }
        default :
            std::cout << "Refait ton choix ! ";

        }
    }
    while (Choix_Menu!='2');  //&& !(Choix_Menu>='a' || Choix_Menu<='z'));


    */
    Graph g;
/// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe

    g.chargement_fic_banquise();
    g.make_example();


    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
    while ( !key[KEY_ESC] )
    {
        if(key[KEY_SPACE])
        {
            g.ajout();
        }
        if (key[KEY_S])
            {
                while (key[KEY_S]);
                g.sauver_banquise();
            }
        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        g.update();

        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }
    return 0;
}
END_OF_MAIN();


