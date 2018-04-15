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
    m_top_box.set_bg_color(BLANC);

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

    m_top_box.add_child(m_bouton);
    m_bouton.set_frame(110,0,25,20);
    m_bouton.set_bg_color(BLEU);
    m_bouton.add_child(m_bouton_label);
    m_bouton_label.set_message("x");
    m_bouton_label.set_bg_color(NOIR);

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
    m_but=0;
    if(m_interface->m_bouton.clicked())
    {
        m_but=1;
    }
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
    m_top_edge.set_bg_color(CYAN);

    // Une boite pour englober les widgets de réglage associés
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(CYAN);

    // Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);
    m_slider_weight.set_bg_color(CYAN);


    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);
    m_label_weight.set_bg_color(CYAN);

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

    m_tool_box.add_child(bouton1);
    bouton1.set_frame(0,50,90,100);
    bouton1.set_bg_color(JAUNE);
    bouton1.add_child(bouton1_label);
    bouton1_label.set_message("Ajouter");

    m_tool_box.add_child(bouton2);
    bouton2.set_frame(0,200,90,100);
    bouton2.set_bg_color(VERT);
    bouton2.add_child(bouton2_label);
    bouton2_label.set_message("Sauvegarder");

    m_tool_box.add_child(bouton3);
    bouton3.set_frame(0,350,90,100);
    bouton3.set_bg_color(ROUGE);
    bouton3.add_child(bouton3_label);
    bouton3_label.set_message("Dynamique");

    m_tool_box.add_child(bouton4);
    bouton4.set_frame(0,500,90,100);
    bouton4.set_bg_color(ORANGE);
    bouton4.add_child(bouton4_label);
    bouton4_label.set_message("Matrice");

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);

    m_main_box.add_child(banquise);
    banquise.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_main_box.add_child(foret);
    foret.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_main_box.add_child(savane);
    savane.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

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
        add_interfaced_vertex(m_sommet[i].m_indice,m_sommet[i].m_value,m_sommet[i].coordx,m_sommet[i].coordy,m_sommet[i].nom_pic);
    }

    for(int j=0; j<nb_arrete; j++)
    {
        add_interfaced_edge(m_arrete[j].m_indice2,m_arrete[j].m_from,m_arrete[j].m_to,m_arrete[j].m_weight);
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
    m_vertices[idx].m_indice = idx;
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
    m_edges[idx].m_indice2 = idx;

    m_vertices[id_vert1].m_out.push_back(id_vert2);
    m_vertices[id_vert2].m_in.push_back(id_vert1);
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
    int indice=0;
    int indice2=0;

    if(fic)
    {
        fic>>m_ordre;
        getline(fic,nom);
        m_sommet.resize(m_ordre);

        for(int i=0; i<m_ordre; i++)
        {
            fic>>indice;
            fic>>val;
            fic>>x;
            fic>>y;
            getline(fic,nom);
            m_sommet[i].m_indice=indice;
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
                fic1>>indice2;
                fic1>>som_d;
                fic1>>som_a;
                fic1>>poids;
                m_arrete[i].m_indice2=indice2;
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
    std::map<int,Vertex>::iterator it;


    do
    {
        std::cout<<"Voulez-vous ajouter une arrete ou un sommet?"<<std::endl;
        std::cout<<"1-Sommet"<<std::endl;
        std::cout<<"2-Arrete"<<std::endl;
        std::cin>>choix;
        switch(choix)
        {
        case '1':
            ephemere.m_indice=0;
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

            for(it=m_vertices.begin(); it!=m_vertices.end(); ++it)
            {
                if(it->first>ephemere.m_indice)
                {
                    ephemere.m_indice=it->first;
                }
            }
            ephemere.m_indice++;
            add_interfaced_vertex(ephemere.m_indice,ephemere.m_value,ephemere.coordx,ephemere.coordy,ephemere.nom_pic);
            break;

        case '2':
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
            fichier_sauv_arete << paires->second.m_indice2 << " " << paires->second.m_from << " " << paires-> second.m_to << " " << paires->second.m_weight << std::endl ;
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
            fichier_sauv_som << paires2->second.m_indice << " " <<  paires2->second.m_value << " " << paires2->second.m_interface->m_top_box.get_posx() << " " << paires2->second.m_interface->m_top_box.get_posy()<< paires2->second.nom_pic<<std::endl ;
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
            fichier_sauv_arete << paires->second.m_indice2 << " " << paires->second.m_from << " " << paires-> second.m_to << " " << paires->second.m_weight << std::endl ;
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
            fichier_sauv_som << paires2->second.m_indice << " " <<  paires2->second.m_value << " " << paires2->second.m_interface->m_top_box.get_posx() << " " << paires2->second.m_interface->m_top_box.get_posy() << paires2->second.nom_pic<<std::endl ;
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
            fichier_sauv_arete << paires->second.m_indice2 << " " << paires->second.m_from << " " << paires-> second.m_to << " " << paires->second.m_weight << std::endl ;
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
            fichier_sauv_som << paires2->second.m_indice << " " <<  paires2->second.m_value << " " << paires2->second.m_interface->m_top_box.get_posx() << " " << paires2->second.m_interface->m_top_box.get_posy() << paires2->second.nom_pic <<std::endl ;
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

    std::map <int, Vertex>::iterator iter ;

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

        m_interface->banquise.set_pic_name("banquise.bmp");
        if(m_interface->bouton1.clicked())
        {
            this->ajout();
        }
        if (m_interface->bouton2.clicked())
        {
            this->sauver_banquise();
        }
        if (m_interface->bouton4.clicked())
        {
            this->Matrice_Adj("mat_adj_banq_base.txt");
        }

        for (iter=m_vertices.begin(); iter!=m_vertices.end(); iter ++)
        {
            if(iter->second.m_but==1)
            {
                int ind_supp=iter->second.m_indice;
                std::cout<< ind_supp <<std::endl;
                this->supprimer_sommet(ind_supp);
            }
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
        m_interface->banquise.set_pic_name("banquise.bmp");
        if(m_interface->bouton1.clicked())
        {
            this->ajout();
        }
        if (m_interface->bouton2.clicked())
        {
            this->sauver_banquise();
        }
        if (m_interface->bouton4.clicked())
        {
            this->Matrice_Adj("mat_adj_banq_sauv.txt");
        }
        std::map <int, Vertex>::iterator iter ;
        for (iter=m_vertices.begin(); iter!=m_vertices.end(); iter ++)
        {
            if(iter->second.m_but==1)
            {
                int indice=iter->second.m_indice;
                std::cout<< indice <<std::endl;
                this->supprimer_sommet(indice);
            }
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
        m_interface->foret.set_pic_name("foret.bmp");
        if(m_interface->bouton1.clicked())
        {
            this->ajout();
        }
        if (m_interface->bouton2.clicked())
        {
            this->sauver_foret();
        }
        if (m_interface->bouton4.clicked())
        {
            this->Matrice_Adj("mat_adj_foret_base.txt");
            this->toutes_CFC();
        }
        std::map <int, Vertex>::iterator iter ;
        for (iter=m_vertices.begin(); iter!=m_vertices.end(); iter ++)
        {
            if(iter->second.m_but==1)
            {
                int indice=iter->second.m_indice;
                std::cout<< indice <<std::endl;
                this->supprimer_sommet(indice);
            }
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
        m_interface->foret.set_pic_name("foret.bmp");
        if(m_interface->bouton1.clicked())
        {
            this->ajout();
        }
        if (m_interface->bouton2.clicked())
        {
            this->sauver_foret();
        }
        if (m_interface->bouton4.clicked())
        {
            this->Matrice_Adj("mat_adj_foret_sauv.txt");
        }
        std::map <int, Vertex>::iterator iter ;
        for (iter=m_vertices.begin(); iter!=m_vertices.end(); iter ++)
        {
            if(iter->second.m_but==1)
            {
                int indice=iter->second.m_indice;
                std::cout<< indice <<std::endl;
                this->supprimer_sommet(indice);
            }
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
        m_interface->savane.set_pic_name("savane.bmp");
        if(m_interface->bouton1.clicked())
        {
            this->ajout();
        }
        if (m_interface->bouton2.clicked())
        {
            this->sauver_savane();
        }
        if (m_interface->bouton4.clicked())
        {
            this->Matrice_Adj("mat_adj_savane_base.txt");
        }
        std::map <int, Vertex>::iterator iter ;
        for (iter=m_vertices.begin(); iter!=m_vertices.end(); iter ++)
        {
            if(iter->second.m_but==1)
            {
                int indice=iter->second.m_indice;
                std::cout<< indice <<std::endl;
                this->supprimer_sommet(indice);
            }
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
        m_interface->savane.set_pic_name("savane.bmp");
        if(m_interface->bouton1.clicked())
        {
            this->ajout();
        }
        if (m_interface->bouton2.clicked())
        {
            this->sauver_savane();
        }
        if (m_interface->bouton4.clicked())
        {
            Matrice_Adj("mat_adj_savane_sauv.txt");
        }
        std::map <int, Vertex>::iterator iter ;
        for (iter=m_vertices.begin(); iter!=m_vertices.end(); iter ++)
        {
            if(iter->second.m_but==1)
            {
                int indice=iter->second.m_indice;
                std::cout<< indice <<std::endl;
                this->supprimer_sommet(indice);
            }
        }
        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        this->update();

        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }
}

void Graph::supprimer_sommet(int indice)
{
    std::map<int,Edge>::iterator it;
    std::vector<int> select;
    std::cout<<"i= "<<indice<<std::endl;
    for(it=m_edges.begin(); it!=m_edges.end(); ++it)
    {
        std::cout<<"sommet depart: "<<it->second.m_from<<" sommet arrive"<<it->second.m_to<<std::endl;
        if(indice==it->second.m_from || indice==it->second.m_to)
        {
            std::cout<<"indice "<<it->first<<" "<<"sommet départ " <<it->second.m_from<<" "<<"sommet arrive "<< it->second.m_to<<std::endl;
            select.push_back(it->first);
        }
    }
    if(select.size()!=0)
    {
        for(int i=0; i<select.size(); i++)
        {
            std::cout<<"indice="<<select[i]<<std::endl;
            test_remove_edge(select[i]);
        }
    }
    std::cout<<"indice2="<<indice<<std::endl;

    test_remove_Vertex(indice);
    std::cout<<"indice supprime= "<<indice<<std::endl;
}

void Graph::test_remove_Vertex(int indice)
{
    int test =0;
    /// référence vers le Edge à enlever
    std::cout<<indice<<std::endl;
    Vertex &remed=m_vertices.at(indice);

    /// test : on a bien des éléments interfacés

    if (m_interface && remed.m_interface)

    {

        m_interface->m_main_box.remove_child(remed.m_interface->m_top_box);

    }

    m_vertices.erase(indice);
}

void Graph::test_remove_edge(int eidx)
{

    /// référence vers le Edge à enlever

    Edge &remed=m_edges.at(eidx);



    std::cout << "Removing edge " << eidx << " " << remed.m_from << "->" << remed.m_to << " " << remed.m_weight << std::endl;

    /// test : on a bien des éléments interfacés

    if (m_interface && remed.m_interface)

    {

        /// Ne pas oublier qu'on a fait ça à l'ajout de l'arc :

        /* EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]); */

        /* m_interface->m_main_box.add_child(ei->m_top_edge);  */

        /* m_edges[idx] = Edge(weight, ei); */

        /// Le new EdgeInterface ne nécessite pas de delete car on a un shared_ptr

        /// Le Edge ne nécessite pas non plus de delete car on n'a pas fait de new (sémantique par valeur)

        /// mais il faut bien enlever le conteneur d'interface m_top_edge de l'arc de la main_box du graphe

        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );

    }



    /// Il reste encore à virer l'arc supprimé de la liste des entrants et sortants des 2 sommets to et from !

    /// References sur les listes de edges des sommets from et to

    std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;

    std::vector<int> &veto = m_vertices[remed.m_to].m_in;

    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );

    veto.erase( std::remove( veto.begin(), veto.end(), eidx ), veto.end() );



    /// Le Edge ne nécessite pas non plus de delete car on n'a pas fait de new (sémantique par valeur)

    /// Il suffit donc de supprimer l'entrée de la map pour supprimer à la fois l'Edge et le EdgeInterface

    /// mais malheureusement ceci n'enlevait pas automatiquement l'interface top_edge en tant que child de main_box !

    m_edges.erase( eidx );

}


void Graph::Matrice_Adj(std::string fichier_matrice)
{

    std::ofstream fichier_mat(fichier_matrice, std::ios::out | std::ios::trunc) ;

    int** adjacence;


    if(fichier_mat)
    {

        adjacence = new int*[m_ordre];
        std::map <int, Edge>::iterator it ;

        std::cout << " " ;
        for(int i=0; i<m_ordre; i++)
        {
            std::cout << " " << i  ;
            fichier_mat << "   " << i  ;
        }
        std::cout << "\n\n" ;
        fichier_mat << "\n" ;

        for(int i=0; i<m_ordre; i++)
        {
            adjacence[i] = new int [m_ordre];
            std::cout << i << " " ;
            fichier_mat << i << "  " ;
            for(int j=0; j<m_ordre; j++)
            {

                adjacence[i][j]=0;


                for (it=m_edges.begin(); it!=m_edges.end(); it ++)
                {
                    if((i==it->second.m_from)&&(j==it->second.m_to))
                    {
                        adjacence[i][j]=1;

                    }
                }

                std::cout << adjacence[i][j] << " " ;
                fichier_mat << adjacence[i][j] << "  " ;

            }
            std::cout << "\n" ;
            fichier_mat << "\n" ;
        }


    }

}



int * Graph::CFC(int som)
{
    int** adjacence;
    adjacence = new int*[m_ordre];
    std::map <int, Edge>::iterator it ;

    for(int i=0; i<m_ordre; i++)
    {
        adjacence[i] = new int [m_ordre];

        for(int j=0; j<m_ordre; j++)
        {

            adjacence[i][j]=0;


            for (it=m_edges.begin(); it!=m_edges.end(); it ++)
            {
                if((i==it->second.m_from)&&(j==it->second.m_to))
                {
                    adjacence[i][j]=1;

                }
            }

        }

    }
    int *c1 ; // CC directe partant de som
    int *c2 ; // CC indirecte arrivant vers le som
    int *c ;
    int *marque ;
    int x, y ;
    bool ajoute = true ; // si CC ajouté

    /// allocation des tableaux dynamiques c1 c2 c et marque de taille m_ordre
    c1 = new int [m_ordre] ;
    c2 = new int [m_ordre] ;
    c = new int [m_ordre] ;
    marque= new int [m_ordre] ;


    /// initialation des valeurs à 0
    for (int i=0 ; i<m_ordre ; i++)
    {
        c1[i]=0 ;
    }
    for (int i=0 ; i<m_ordre ; i++)
    {
        c2[i]=0 ;
    }
    for (int i=0 ; i<m_ordre ; i++)
    {
        c[i]=0 ;
    }

    /// sommet som connexe
    c1[som]=1 ;
    c2[som]=1 ;

    /// Recherche des composantes connexes partant de som
    while (ajoute)
    {
        ajoute = false ; // pour chaque tour : recherche d'une nouvelle CC à ajouter

        for (x=0 ; x<m_ordre; x++)
        {
            if (!marque[x] && c1[x])
            {
                marque[x]=1 ;
                for (y = 0 ; y < m_ordre ; y++)
                {
                    if (adjacence[x][y] && !marque[y])
                    {
                        c1[y]=1 ;
                        ajoute = true ; /// la nouvelle CC est ajoutée
                    }
                }
            }

        }
        for (x=0 ; x<m_ordre; x++)
        {
            if (!marque[x] && c2[x])
            {
                marque[x]=1 ;
                for (y = 0 ; y < m_ordre ; y++)
                {
                    if (adjacence[y][x] && !marque[y])
                    {
                        c2[y]=1 ;
                        ajoute = true ; /// la nouvelle CC est ajoutée
                    }
                }
            }

        }
    }

    /// CFC c = intersection de c1 et de c2
    for (x=0 ; x<m_ordre ; x++)
    {
        c[x]=c1[x] & c2[x] ;
    }
    std::cout << "La nouvelle composante connexe est ajoute" << std::endl;
    delete c1 ;
    delete c2 ;
    delete marque ;

    return c ;

}

int ** Graph::toutes_CFC() //récupère la matrice d'adjacence
{
    int **tabc ; // tab dyn des CFC à retourner
    int *marque ;
    int x, y ;

    /// allocations des tab dyn
    tabc = new int *[m_ordre] ;
    for (int i=0 ; i<m_ordre; i++)
    {
        tabc[i] = new int [m_ordre] ;
        tabc[i]=0 ;
    }
    marque = new int [m_ordre] ;
    for (int i = 0 ; i<m_ordre ; i++)
    {
        marque[i]=0 ;
    }

    Vertex som_init;
    int sommet = som_init.m_indice;
    std::cout << "Par quel sommet voulez vous commencer ?\nEntrez le numero d'un sommet :" << std::endl;
    std::cin >> sommet;

    ///pr tt som nonmarqués
    for (x=0 ; x<m_ordre ; x++)
    {
        if (!marque[x])
        {
            tabc[x]= CFC(sommet);
            marque[x]=1 ;
            for (y=0 ; y<m_ordre; y++)
            {
                if (tabc[x][y] && (!marque[y]))
                {
                    marque[y]=1 ;
                }
            }
        }
    }

    delete marque ;

    return tabc ;
}

/*
///Sous-programme dynamisme des populations
void Graph::Dynamique()
{
    ///parcours de tous les sommets
    for(auto& it1:m_vertices)
    {
        ///parcours des arrêtes orientés vers ce sommet
        for (auto& it : it1.second.m_in)
        {
            ///calcul du de la capacité de portage de l'environnement
            it1.second.Kmange= it1.second.Kmange + (m_edges[it].m_weight*m_vertices[m_edges[it].m_from].m_value);
        }
    }
    ///parcours tous les sommets
    for(auto& it1:m_vertices)
    {
      ///parcous de toutes les arrêtes de sortis
      for (auto& it : it1.second.m_out)
        {
            ///calcul du coefficient predateur
            it1.second.Kmangeur=it1.second.Kmangeur + (m_edges[it].m_weight*m_vertices[m_edges[it].m_to].m_value);
        }
        ///Borne à 0 dans le cas où la population est morte pour ne pas avoir de nombre négatif
        if(it1.second.Kmange<=0 || it1.second.Kmangeur>it1.second.m_value+0.1*it1.second.m_value*((1-it1.second.m_value)/(it1.second.Kmange)))
        {
          it1.second.m_value=0;
        }
        ///calcul de la valeur de la population au cours du temps
        else
        {
            it1.second.m_value=(it1.second.m_value)+0.1*it1.second.m_value*((1-it1.second.m_value)/(it1.second.Kmange))-(it1.second.Kmangeur);
        }
    }

}

*/

