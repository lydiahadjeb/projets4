#include "graph.h"
#include <fstream>

/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de r�glage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  // Valeurs arbitraires, � adapter...
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


/// Gestion du Vertex avant l'appel � l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_value vers le slider associ�
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donn�e m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Vertex apr�s l'appel � l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
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

    // Une boite pour englober les widgets de r�glage associ�s
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de r�glage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);  // Valeurs arbitraires, � adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel � l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_weight vers le slider associ�
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donn�e m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}

/// Gestion du Edge apr�s l'appel � l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de pr�parer un cadre d'accueil des
/// �l�ments qui seront ensuite ajout�s lors de la mise ne place du Graphe
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

    // bouton delete sommet
    m_tool_box.add_child( m_delete_vertex );
    m_delete_vertex.set_frame(2,2,80,80);
    m_delete_vertex.add_child( m_img );
    m_img.set_pic_name("delete.png");

    // bouton ajouter sommet
    m_tool_box.add_child( m_add_vertex );
    m_add_vertex.set_frame(2,100,80,80);
    m_add_vertex.add_child( m_img3 );
    m_img3.set_pic_name("add.jpg");



    // bouton sauvgarde
    m_tool_box.add_child( m_save );
    m_save.set_frame(2,640,80,80);
    m_save.add_child( m_img2 );
    m_img2.set_pic_name("save.jpg");

    // bouton graphe1
     m_tool_box.add_child( m_graphe1 );
     m_graphe1.set_frame(2, 300,80,80);
     m_graphe1.add_child(m_img5);
     m_img5.set_pic_name("numero1.jpg");

         // bouton graphe2
     m_tool_box.add_child( m_graphe2 );
     m_graphe2.set_frame(2, 400,80,80);
     m_graphe2.add_child(m_img6);
     m_img6.set_pic_name("numero2.jpg");


         // bouton graphe3
     m_tool_box.add_child( m_graphe3 );
     m_graphe3.set_frame(2, 500,80,80);
     m_graphe3.add_child(m_img7);
     m_img7.set_pic_name("numero3.jpg");


}


/// M�thode sp�ciale qui construit un graphe arbitraire (d�mo)
/// Cette m�thode est � enlever et remplacer par un syst�me
/// de chargement de fichiers par exemple.
/// Bien s�r on ne veut pas que vos graphes soient construits
/// "� la main" dans le code comme �a.
void Graph::make_example()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne pr�c�dente est en gros �quivalente � :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent �tre d�finis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
    add_interfaced_vertex(0, 30.0, 200, 100, "clown1.jpg");
    add_interfaced_vertex(1, 60.0, 400, 100, "clown2.jpg");
    add_interfaced_vertex(2,  50.0, 200, 300, "clown3.jpg");
    add_interfaced_vertex(3,  0.0, 400, 300, "clown4.jpg");
    add_interfaced_vertex(4,  100.0, 600, 300, "clown5.jpg");
    add_interfaced_vertex(5,  0.0, 100, 500, "bad_clowns_xx3xx.jpg", 0);
    add_interfaced_vertex(6,  0.0, 300, 500, "bad_clowns_xx3xx.jpg", 1);
    add_interfaced_vertex(7,  0.0, 500, 500, "bad_clowns_xx3xx.jpg", 2);

    /// Les arcs doivent �tre d�finis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
    add_interfaced_edge(0, 1, 2, 50.0);
    add_interfaced_edge(1, 0, 1, 50.0);
    add_interfaced_edge(2, 1, 3, 75.0);
    add_interfaced_edge(3, 4, 1, 25.0);
    add_interfaced_edge(4, 6, 3, 25.0);
    add_interfaced_edge(5, 7, 3, 25.0);
    add_interfaced_edge(6, 3, 4, 0.0);
    add_interfaced_edge(7, 2, 0, 100.0);
    add_interfaced_edge(8, 5, 2, 20.0);
    add_interfaced_edge(9, 3, 7, 80.0);
}

/// La m�thode update � appeler dans la boucle de jeu pour les graphes avec interface
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

    if(m_interface->m_delete_vertex.clicked())
    {
        test_remove_vertex();
    }


     if(m_interface->m_graphe1.clicked())
    {
        clear();
        charger("r1.txt");
    }
    if(m_interface->m_graphe2.clicked())
    {
        clear();
        charger("r2.txt");
    }
     if(m_interface->m_graphe3.clicked())
    {
        clear();
        charger("r3.txt");
    }





}

/// Aide � l'ajout de sommets interfac�s
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Cr�ation d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}

/// Aide � l'ajout d'arcs interfac�s
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

    m_edges[idx].m_from=id_vert1;
    m_edges[idx].m_to=id_vert2;

    m_vertices[id_vert1].m_out.push_back(idx);
    m_vertices[id_vert2].m_in.push_back(idx);
}

void Graph::test_remove_edge(int eidx)
{
    /// r�f�rence vers le Edge � enlever
    Edge &remed=m_edges.at(eidx);

    //std::cout << "Removing edge " << eidx << " " << remed.m_from << "->" << remed.m_to << " " << remed.m_weight << std::endl;

    /// Tester la coh�rence : nombre d'arc entrants et sortants des sommets 1 et 2
    //std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    //std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    //std::cout << m_edges.size() << std::endl;

    /// test : on a bien des �l�ments interfac�s
    if (m_interface && remed.m_interface)
    {
        /// Ne pas oublier qu'on a fait �a � l'ajout de l'arc :
        /* EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]); */
        /* m_interface->m_main_box.add_child(ei->m_top_edge);  */
        /* m_edges[idx] = Edge(weight, ei); */
        /// Le new EdgeInterface ne n�cessite pas de delete car on a un shared_ptr
        /// Le Edge ne n�cessite pas non plus de delete car on n'a pas fait de new (s�mantique par valeur)
        /// mais il faut bien enlever le conteneur d'interface m_top_edge de l'arc de la main_box du graphe
        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );
    }

    /// Il reste encore � virer l'arc supprim� de la liste des entrants et sortants des 2 sommets to et from !
    /// References sur les listes de edges des sommets from et to
    std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;
    std::vector<int> &veto = m_vertices[remed.m_to].m_in;
    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );
    veto.erase( std::remove( veto.begin(), veto.end(), eidx ), veto.end() );

    /// Le Edge ne n�cessite pas non plus de delete car on n'a pas fait de new (s�mantique par valeur)
    /// Il suffit donc de supprimer l'entr�e de la map pour supprimer � la fois l'Edge et le EdgeInterface
    /// mais malheureusement ceci n'enlevait pas automatiquement l'interface top_edge en tant que child de main_box !
    m_edges.erase( eidx );


    /// Tester la coh�rence : nombre d'arc entrants et sortants des sommets 1 et 2
    //std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    //std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    //std::cout << m_edges.size() << std::endl;

}




void Graph::test_remove_vertex()
{
    int vidx;

    std::cout<<"quel sommet voulez-vous supprimer?"<<std::endl;
    std::cin>>vidx;

    Vertex &remed=m_vertices.at(vidx);

//supprime les aretes entrantes
    for(unsigned int i=0; i<m_vertices[vidx].m_in.size(); i++)
    {
        int a=m_vertices[vidx].m_in[i];
        test_remove_edge(a);

    }

//supprime les aretes sortantes
    for(unsigned int i=0; i<m_vertices[vidx].m_out.size(); i++)
    {
        int a=m_vertices[vidx].m_out[i];
        test_remove_edge(a);

    }




    for(auto& elem: m_edges)
    {
        if((elem.second.m_to==vidx)||(elem.second.m_from==vidx))
        {
            test_remove_edge(elem.first);

        }
    }

    m_interface->m_main_box.remove_child(remed.m_interface->m_top_box);
    m_vertices.erase(vidx);

}

void Graph::test_remove_vertex(int vidx)
{
    Vertex &remed=m_vertices.at(vidx);

//supprime les aretes entrantes
    for(unsigned int i=0; i<m_vertices[vidx].m_in.size(); i++)
    {
        int a=m_vertices[vidx].m_in[i];
        test_remove_edge(a);

    }

//supprime les aretes sortantes
    for(unsigned int i=0; i<m_vertices[vidx].m_out.size(); i++)
    {
        int a=m_vertices[vidx].m_out[i];
        test_remove_edge(a);

    }





    for(auto& elem: m_edges)
    {
        if((elem.second.m_to==vidx)||(elem.second.m_from==vidx))
        {
            test_remove_edge(elem.first);

        }
    }

    m_interface->m_main_box.remove_child(remed.m_interface->m_top_box);
    m_vertices.erase(vidx);



}




/*void Graph::charger()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    int som, x,y, nbSommets, nbAretes;
    double population ;
    std::string nom,nomfic;

    int indice, som1,som2;
    double poids;
    std::ifstream fichier("r1.txt");
    if(fichier)
    {
        fichier >> nbSommets;
        fichier >> nbAretes;
        for(unsigned int i(0); i < nbSommets; ++i)
        {
            fichier >> som >> population >> x >> y >> nom;
            add_interfaced_vertex(som,population,x,y,nom);
        }

        for(unsigned int i(0); i < nbAretes; ++i)
        {
            fichier >> indice >> som1 >> som2 >> poids;
            add_interfaced_edge(indice,som1,som2,poids);
        }
    }
}*/

void Graph::clear() {
    for (auto &it : m_vertices)
        test_remove_vertex(it.first);

    for (auto &it : m_edges)
        test_remove_edge(it.first);
}


void Graph::charger(std::string nomFichier)
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    int som, x,y, nbSommets, nbAretes;
    double population ;
    std::string nom;

    int indice, som1,som2;
    double poids;
    std::fstream fichier(nomFichier, std::ios_base::in);
    if(fichier.is_open())
    {
        fichier >> nbSommets;
        fichier >> nbAretes;
        for(unsigned int i(0); i < nbSommets; ++i)
        {
            fichier >> som >> population >> x >> y >> nom;
            add_interfaced_vertex(som,population,x,y,nom);
        }

        for(unsigned int i(0); i < nbAretes; ++i)
        {
            fichier >> indice >> som1 >> som2 >> poids;
            add_interfaced_edge(indice,som1,som2,poids);
        }
    }
}


void Graph::sauvegarde()
{
    std::string nom, nom2;
///vertex
//std::cout<<"Veuillez choisir le nom de votre fichier de sauvegarde"<<std::endl;
//std::cin>> nom;
//nom2=nom+".txt";

std::ofstream fichier ("sauvegarde.txt");
///parcourir la map de vertex et la retranscrire dans un fichier
        fichier<<m_vertices.size()<<std::endl;
        fichier<<m_edges.size()<<std::endl;
        for (const auto & elem : m_vertices )
        {

           fichier<<elem.first<< ' '<<elem.second.m_value<< ' '<<elem.second.m_interface->m_top_box.get_posx()<< ' '<< elem.second.m_interface->m_top_box.get_posy()<< ' ' << elem.second.m_interface->m_img.get_pic_name()<<std::endl;
        }
/// ajouter une ligne pour actualiser la position avec le chemin indique

///edge
        for (const auto & elem3 : m_edges )
        {

           fichier<<elem3.first<<' ' <<elem3.second.m_from<< ' ' << elem3.second.m_to<<' ' << elem3.second.m_weight<<std::endl;
        }
}
