#include "graph.h"
#include "map"
#include <fstream>
#include <sstream>

/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );
}


/// Gestion du Vertex avant l'appel à l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_value vers le slider associé
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donnée m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Vertex après l'appel à l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les éléments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de réglage associés
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel à l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_weight vers le slider associé
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donnée m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}

/// Gestion du Edge après l'appel à l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de préparer un cadre d'accueil des
/// éléments qui seront ensuite ajoutés lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);

}


/// Méthode spéciale qui construit un graphe arbitraire (démo)
/// Cette méthode est à enlever et remplacer par un système
/// de chargement de fichiers par exemple.
/// Bien sûr on ne veut pas que vos graphes soient construits
/// "à la main" dans le code comme ça.
void Graph::make_example()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne précédente est en gros équivalente à :
    // m_interface = new GraphInterface(50, 0, 750, 600);
    for(int i=0; i<m_ordre; i++)
    {
        add_interfaced_vertex(i,m_sommet[i].m_value,m_sommet[i].coordx,m_sommet[i].coordy,m_sommet[i].nom_pic);
    }

    for(int j=0; j<nb_arrete; j++)
    {
        add_interfaced_edge(j,m_arrete[j].m_from,m_arrete[j].m_to,m_arrete[j].m_weight);
    }

}

/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();

    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
        elt.second.post_update();

    for (auto &elt : m_edges)
        elt.second.post_update();

}

/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Création d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
    m_vertices[idx].coordx = x;
    m_vertices[idx].coordy = y;
    m_vertices[idx].nom_pic = pic_name;
}

/// Aide à l'ajout d'arcs interfacés
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);

    m_edges[idx].m_from = id_vert1;
    m_edges[idx].m_to = id_vert2;

    m_vertices[id_vert1].m_out.push_back(idx);
    m_vertices[id_vert2].m_in.push_back(idx);
}

void Graph::chargement_fic(std::string ficbanq, std::string ficarete1)
{
    std::ifstream fic(ficbanq);
    std::ifstream fic1(ficarete1);
    double val=0.0;
    int x=0;
    int y=0;
    std::string nom;
    int som_d=0;
    int som_a=0;
    double poids=0.0;

    if(fic)
    {
        fic>>m_ordre;
        getline(fic,nom);
        m_sommet.resize(m_ordre);

        for(int i=0; i<m_ordre; i++)
        {
            fic>>val;
            fic>>x;
            fic>>y;
            getline(fic,nom);
            m_sommet[i].m_value=val;
            m_sommet[i].coordx=x;
            m_sommet[i].coordy=y;
            m_sommet[i].nom_pic=nom;
            std::cout<<m_sommet[i].m_value<<std::endl;
            std::cout<<m_sommet[i].coordx<<std::endl;
            std::cout<<m_sommet[i].coordy<<std::endl;
            std::cout<<m_sommet[i].nom_pic<<std::endl;

        }
        if(fic1)
        {
            fic1>>nb_arrete;
            m_arrete.resize(nb_arrete);
            for(int i=0; i<nb_arrete; i++)
            {
                fic1>>som_d;
                fic1>>som_a;
                fic1>>poids;
                m_arrete[i].m_from=som_d;
                m_arrete[i].m_to=som_a;
                m_arrete[i].m_weight=poids;
                std::cout<<m_arrete[i].m_from<<std::endl;
                std::cout<<m_arrete[i].m_to<<std::endl;
                std::cout<<m_arrete[i].m_weight<<std::endl;
            }
        }
        fic1.close();

    }
    fic.close();

}




void Graph::ajout()
{
    char choix=0;
    Edge temp;
    int depart=0;
    int arrive=0;
    double poids=0.0;
    Vertex ephemere;
    double population=0.0;
    int x=0;
    int y=0;
    std::string nom;


    do
    {
        std::cout<<"Voulez-vous ajouter une arrete ou un sommet?"<<std::endl;
        std::cout<<"1-Sommet"<<std::endl;
        std::cout<<"2-Arrete"<<std::endl;
        std::cin>>choix;
        switch(choix)
        {
        case '1':
        {
            do
            {
                std::cout<<"Choisissez la population de depart de l'animal:"<<std::endl;
                std::cin>>population;
                ephemere.m_value=population;
            }
            while ((population<0.0)||(population>100.0));

            do
            {
            std::cout<<"Choisissez l'abscisse:"<<std::endl;
            std::cin>>x;
            ephemere.coordx=x;
            }
            while ((x<1)||(x>773));

            do
            {
            std::cout<<"Choisissez l'ordonne:"<<std::endl;
            std::cin>>y;
            ephemere.coordy=y;
            }
            while ((y<1)||(y>615));

            std::cout<<"Choisissez le nom de l'animal:"<<std::endl;
            std::cin>>nom;
            ephemere.nom_pic=nom;

            add_interfaced_vertex(m_vertices.size(),ephemere.m_value,ephemere.coordx,ephemere.coordy,ephemere.nom_pic);

            break;
        }

        case '2':
        {

            std::cout<<"Choisissez le sommet de depart:"<<std::endl;
            std::cin>>depart;
            temp.m_from=depart;

            std::cout<<"Choisissez le sommet d'arrive:"<<std::endl;
            std::cin>>arrive;
            temp.m_to=arrive;

            do
            {
            std::cout<<"Choisissez le poids de l'arrete:"<<std::endl;
            std::cin>>poids;
            temp.m_weight=poids;
            }
            while ((poids<0.0)||(poids>100.0));

            add_interfaced_edge(m_edges.size(),temp.m_from,temp.m_to,temp.m_weight);

            break;
        }
        }
    }
    while ((choix!='1')&&(choix!='2'));
}

void Graph::sauver_banquise()
{

    std::ofstream fichier_sauv_som("fic_banquise_sauv.txt",  std::ios::out | std::ios::trunc) ;  /// lecture du fichier et écrire à la fin du fichier texte en écrasant ce qui a été fait dans ce fichier
    std::ofstream fichier_sauv_arete("fic_arete1_sauv.txt", std::ios::out | std::ios::trunc) ;

    if (fichier_sauv_arete)
    {
        std::map <int, Edge>::iterator paires ; // l'iterator va permette d'ouvrir des instances de classe pairequi se compose de la clé et de la valeur
        std::cout << m_edges.size() << std::endl;
        fichier_sauv_arete << m_edges.size() << std::endl;
        for (paires=m_edges.begin(); paires!=m_edges.end(); paires ++)
        {

            std::cout << paires-> second.m_from << " " << paires-> second.m_to << " " << paires->second.m_weight << std::endl ;
            fichier_sauv_arete << paires->second.m_from << " " << paires-> second.m_to << " " << paires->second.m_weight << std::endl ;
        }
        std::cout << "Vous avez sauvegarde les aretes presentes dans le graphe actuel\n" ;
    }
    else
    {
        std::cout << "EOF ! \n\n\n" ;
    }
    fichier_sauv_arete.close();

    if (fichier_sauv_som) /// il faut maintenant sauvegarder les artes
    {
        std::map <int, Vertex>::iterator paires2 ;
        std::cout << m_vertices.size() << std::endl;
        fichier_sauv_som << m_vertices.size() << std::endl;
        for (paires2=m_vertices.begin(); paires2!=m_vertices.end(); paires2 ++)
        {

            std::cout << paires2->second.m_value << " " << paires2->second.m_interface->m_top_box.get_posx() << " " << paires2->second.m_interface->m_top_box.get_posy() << " " << paires2->second.nom_pic <<std::endl ;
            fichier_sauv_som <<  paires2->second.m_value << " " << paires2->second.m_interface->m_top_box.get_posx() << " " << paires2->second.m_interface->m_top_box.get_posy()<< paires2->second.nom_pic<<std::endl ;
        }
        std::cout <<"Vous avez sauvegarde les sommets du graphe actuel \n";
    }
    else
    {
        std::cout << "EOF ! \n\n\n" ;
    }
    fichier_sauv_som.close();
}

void Graph::sauver_foret()
{
    std::ofstream fichier_sauv_som("fic_foret_sauv.txt",  std::ios::out | std::ios::trunc) ;  /// lecture du fichier et écrire à la fin du fichier texte en écrasant ce qui a été fait dans ce fichier
    std::ofstream fichier_sauv_arete("fic_arete2_sauv.txt", std::ios::out | std::ios::trunc) ;

    if (fichier_sauv_arete)
    {
        std::map <int, Edge>::iterator paires ; // l'iterator va permette d'ouvrir des instances de classe pairequi se compose de la clé et de la valeur
        std::cout << m_edges.size() << std::endl;
        fichier_sauv_arete << m_edges.size() << std::endl;
        for (paires=m_edges.begin(); paires!=m_edges.end(); paires ++)
        {

            std::cout << paires-> second.m_from << " " << paires-> second.m_to << " " << paires->second.m_weight << std::endl ;
            fichier_sauv_arete << paires->second.m_from << " " << paires-> second.m_to << " " << paires->second.m_weight << std::endl ;
        }
        std::cout << "Vous avez sauvegarde les aretes presentes dans le graphe actuel\n" ;
    }
    else
    {
        std::cout << "EOF ! \n\n\n" ;
    }
    fichier_sauv_arete.close();

    if (fichier_sauv_som) /// il faut maintenant sauvegarder les aretes
    {
        std::map <int, Vertex>::iterator paires2 ;
        std::cout << m_vertices.size() << std::endl;
        fichier_sauv_som << m_vertices.size() << std::endl;
        for (paires2=m_vertices.begin(); paires2!=m_vertices.end(); paires2 ++)
        {

            std::cout << paires2->second.m_value << " " << paires2->second.m_interface->m_top_box.get_posx() << " " << paires2->second.m_interface->m_top_box.get_posy() << " " << paires2->second.nom_pic <<std::endl ;
            fichier_sauv_som <<  paires2->second.m_value << " " << paires2->second.m_interface->m_top_box.get_posx() << " " << paires2->second.m_interface->m_top_box.get_posy() << paires2->second.nom_pic<<std::endl ;
        }
        std::cout <<"Vous avez sauvegarde les sommets du graphe actuel \n";
    }
    else
    {
        std::cout << "EOF ! \n\n\n" ;
    }
    fichier_sauv_som.close();
}

void Graph::sauver_savane()
{

    std::ofstream fichier_sauv_som("fic_savane_sauv.txt",  std::ios::out | std::ios::trunc) ;  /// lecture du fichier et écrire à la fin du fichier texte en écrasant ce qui a été fait dans ce fichier
    std::ofstream fichier_sauv_arete("fic_arete3_sauv.txt", std::ios::out | std::ios::trunc) ;

    if (fichier_sauv_arete)
    {
        std::map <int, Edge>::iterator paires ; // l'iterator va permette d'ouvrir des instances de classe pairequi se compose de la clé et de la valeur
        std::cout << m_edges.size() << std::endl;
        fichier_sauv_arete << m_edges.size() << std::endl;
        for (paires=m_edges.begin(); paires!=m_edges.end(); paires ++)
        {

            std::cout << paires-> second.m_from << " " << paires-> second.m_to << " " << paires->second.m_weight << std::endl ;
            fichier_sauv_arete << paires->second.m_from << " " << paires-> second.m_to << " " << paires->second.m_weight << std::endl ;
        }
        std::cout << "Vous avez sauvegarde les aretes presentes dans le graphe actuel\n" ;
    }
    else
    {
        std::cout << "EOF ! \n\n\n" ;
    }
    fichier_sauv_arete.close();

    if (fichier_sauv_som) /// il faut maintenant sauvegarder les artes
    {
        std::map <int, Vertex>::iterator paires2 ;
        std::cout << m_vertices.size() << std::endl;
        fichier_sauv_som << m_vertices.size() << std::endl;
        for (paires2=m_vertices.begin(); paires2!=m_vertices.end(); paires2 ++)
        {

            std::cout << paires2->second.m_value << " " << paires2->second.m_interface->m_top_box.get_posx() << " " << paires2->second.m_interface->m_top_box.get_posy() << " " << paires2->second.nom_pic <<std::endl ;
            fichier_sauv_som <<  paires2->second.m_value << " " << paires2->second.m_interface->m_top_box.get_posx() << " " << paires2->second.m_interface->m_top_box.get_posy() << paires2->second.nom_pic <<std::endl ;
        }
        std::cout <<"Vous avez sauvegarde les sommets du graphe actuel \n";
    }
    else
    {
        std::cout << "EOF ! \n\n\n" ;
    }
    fichier_sauv_som.close();
}

void Graph::appel_graphe_banquise_base()
{

    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe

    this->chargement_fic("ficbanquise_base.txt", "ficarete1_base.txt");
    this->make_example();


    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
    while ( !key[KEY_ESC] )
    {
        if(key[KEY_SPACE])
        {
            this->ajout();
        }
        if (key[KEY_S])
        {
            while (key[KEY_S]);
            this->sauver_banquise();
        }
        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        this->update();

        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }
}

void Graph::appel_graphe_banquise_sauv()
{

    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe

    this->chargement_fic("fic_banquise_sauv.txt", "fic_arete1_sauv.txt");
    this->make_example();


    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
    while ( !key[KEY_ESC] )
    {
        if(key[KEY_SPACE])
        {
            this->ajout();
        }
        if (key[KEY_S])
        {
            while (key[KEY_S]);
            this->sauver_banquise();
        }
        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        this->update();

        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }
}

void Graph::appel_graphe_foret_base()
{

    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe

    this->chargement_fic("ficforet_base.txt", "ficarete2_base.txt");
    this->make_example();


    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
    while ( !key[KEY_ESC] )
    {
        if(key[KEY_SPACE])
        {
            this->ajout();
        }
        if (key[KEY_S])
        {
            while (key[KEY_S]);
            this->sauver_foret();
        }
        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        this->update();

        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }
}

void Graph::appel_graphe_foret_sauv()
{

    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe

    this->chargement_fic("fic_foret_sauv.txt", "fic_arete2_sauv.txt");
    this->make_example();


    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
    while ( !key[KEY_ESC] )
    {
        if(key[KEY_SPACE])
        {
            this->ajout();
        }
        if (key[KEY_S])
        {
            while (key[KEY_S]);
            this->sauver_foret();
        }
        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        this->update();

        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }
}

void Graph::appel_graphe_savane_base()
{

    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe

    this->chargement_fic("ficsavane_base.txt", "ficarete3_base.txt");
    this->make_example();


    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
    while ( !key[KEY_ESC] )
    {
        if(key[KEY_SPACE])
        {
            this->ajout();
        }
        if (key[KEY_S])
        {
            while (key[KEY_S]);
            this->sauver_savane();
        }
        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        this->update();

        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }
}

void Graph::appel_graphe_savane_sauv()
{

    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe

    this->chargement_fic("fic_savane_sauv.txt", "fic_arete3_sauv.txt");
    this->make_example();


    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
    while ( !key[KEY_ESC] )
    {
        if(key[KEY_SPACE])
        {
            this->ajout();
        }
        if (key[KEY_S])
        {
            while (key[KEY_S]);
            this->sauver_savane();
        }
        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        this->update();

        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }
}


void Graph::CFC_fic()
{
    /*int numCC = 1 ;
    Vertex som_init;
    std::map <int, Vertex>::iterator it ;

    std::cout << "Par quel sommet voulez vous commencer ? " << std::endl;
    std::cin >> som_init.nom_pic;

    for (it=m_vertices.begin(); it!=m_vertices.end(); it ++)
    {


    }*/
    int** adjacence;
    adjacence=new int*[7];
    for(int i=0; i<7;i++)
    {
        adjacence[i]=new int [7];
    }



}

