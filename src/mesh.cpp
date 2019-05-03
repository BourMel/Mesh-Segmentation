#include "mesh.hpp"
#include "edge.hpp"
#include "face.hpp"
#include "utils.hpp"
#include "output.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <glm/gtx/string_cast.hpp>

Mesh::Mesh()
{
}

void Mesh::skeletonization()
{
    bool stop = false;
    while(!stop)
    {
        std::size_t i = 0;

        std::cout <<"sorting"<< "..." << std::endl;
        std::sort(m_edges.begin(), m_edges.end(), Edge::compEdgePtr);

        while(i < m_edges.size())
        {
            Edge *e = m_edges[i];
            if(e->type() == Edge::MESH)
            {
                if(e->faces().size() > 0) // if an edge is connect to at least one face
                {
                    dissolveEdge(e);
                    //m_edges.erase(find(m_edges, e));
                    std::cout << "erase" << std::endl;
                    if(e->v1()->locked() == false) {m_vertices.erase(find(m_vertices,e->v1()));}
                    if(e->v2()->locked() == false) {m_vertices.erase(find(m_vertices,e->v2()));}
                    m_edges.erase(find(m_edges,e));
                    std::cout << "erase done" << std::endl;
                    break;              
                }
                else // if an edge is not connected to any face, lock it
                {
                    //std::cout << *e <<"to bone" << std::endl;
                    e->type(Edge::BONE);
                    e->v1()->lock(true);
                    e->v2()->lock(true);
                }
            }
            else
            {
                //std::cout << "is bone, skip" << std::endl;
            }

            i++;
        }
        if(i >= m_edges.size()-1)
        {
            stop = true;
        }
    }
    debug();
}

void Mesh::segmentation() {}

// edge: the edge to collapse
void Mesh::dissolveEdge(Edge *edge)
{
    Vertex *mean = edge->getMeanPosition(); // the mean position on the edge
    m_vertices.push_back(mean);
    std::cout << "trying to collapse " << *edge << " @ " << glm::to_string(mean->pos()) << std::endl;

    auto edges = edge->getConnectedEdges(); //   all the edges connected
    //std::cout << edges.size() << " edges connected." << std::endl;

    for(Edge* e : edges)
    {
        if(e->isLocked() == false)
        {
            if(e->v1() == edge->v1() || e->v1() == edge->v2()) // e.v1 is connected to edge
            {
                e->v1()->edges().erase(find(e->v1()->edges(),e));
                e->v1(mean);
                mean->addEdge(e);
            }
            else if(e->v2() == edge->v1() || e->v2() == edge->v2()) // e.v2
            {
                e->v2()->edges().erase(find(e->v2()->edges(),e));
                e->v2(mean);
                mean->addEdge(e);
            }
            else
            {
                // erreur
                std::cerr << "edge not connected to the collapsing edge!" << std::endl;
            }

        }
        else // edge is locked so is bone
        {
            Edge *ve = new Edge;   
            if(e->v1() == edge->v1() || e->v1() == edge->v2()) // e.v1 is connected to edge
            {
                ve->v1(e->v1());
                ve->v2(mean);
            }
            else if(e->v2() == edge->v1() || e->v2() == edge->v2()) // e.v2
            {
                ve->v1(mean);
                ve->v2(e->v2());
            }
            ve->v1()->lock(true);
            ve->v2()->lock(true);
            ve->type(Edge::VIRTUAL);
            m_edges.push_back(ve);
            std::cout << "new virtual edge:" << *ve << std::endl;
            // virtual edge ?
        }
    }

    // all free edges are now connected to mean
    // go through all edges of mean to detect duplicates and update ATL
    for(auto e1 = mean->edges().begin(); e1 != mean->edges().end(); e1++) // iterator
    {
        for(auto e2 = mean->edges().begin(); e2 != mean->edges().end(); e2++) // iterator
        {
            // if two edges have same vertices and are not the same edge
            if((((*e1)->v1() == (*e2)->v1() && (*e1)->v2() == (*e2)->v2()) || ((*e1)->v1() == (*e2)->v2() && (*e1)->v2() == (*e2)->v1())) && ((*e1) !=(*e2)))
            {
                Face *f = faceInCommon(*e1, *e2);
                if(f != nullptr)
                {
                    // on enlève la face commune de e1 et e2
                    (*e1)->removeFace(f);
                    (*e2)->removeFace(f);
                    // on la met en ATL à (*e1)
                    (*e1)->addFaceATL(f);
                    // si e2 était adjacent à une 2e face, l'ajouter à la liste de e1
                    if((*e2)->faces()[0] != f)
                    {
                        (*e1)->addFace((*e2)->faces()[0]);
                    }

                    // on supprime e2 de la liste des edges de l'extremité qui n'est pas mean
                    if((*e2)->v1() != mean)
                    {
                        (*e2)->v1()->removeEdge(*e2);
                    }
                    else
                    {
                        (*e2)->v2()->removeEdge(*e2);
                    }

                }
                
                // si la face commune était la seule face adjacente aux deux edges, c'est mtn une BONE
                if((*e1)->faces().size() <= 0)
                {
                    (*e1)->v1()->lock(true);
                    (*e1)->v2()->lock(true);
                    (*e1)->type(Edge::BONE);
                }

                // on supprime le doublon de la liste globale
                m_edges.erase(find(m_edges,*e2));
                // on supprime le doublon de la liste de mean
                mean->edges().erase(e2);
                break; // cannot have another match
            }
        }
    }
}

void Mesh::importOFF(std::string filename)
{

    std::ifstream file;   // input file
    std::string line;     // current line
    std::stringstream ss; // stream to read content of line

    int numLine = 0;       // number of the current line
    int numVert, numFaces; // number of vertices/faces

    float x, y, z;

    // open file
    file.open(filename);
    if (!file.is_open())
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
    else
    {
        std::cout << "filename: " << filename << std::endl;
    }

    // detect OFF tag
    std::getline(file, line);
    numLine++;
    ss.str(line);
    if (!(ss.str() == "OFF"))
    {
        std::cerr << "file token is not OFF" << std::endl;
        return;
    }

    std::getline(file, line);
    numLine++;

    // get the number of vertices end faces indicated in the off file
    ss.str(line);
    ss >> numVert >> numFaces;

    // read vertices info
    for (int i = 0; i < numVert; i++)
    {
        std::getline(file, line);
        numLine++;

        std::istringstream iss(line);

        // tries to read at least 3 consecutive numbers as float
        if (iss >> x >> y >> z)
        {
            // add the new vertex to the mesh
            m_vertices.push_back(new Vertex({x, y, z}));
        }
        else
        {
            std::cout << "Wrong vertex format (at " << numLine << ")" << std::endl;
            return;
        }
    }

    // read face composition
    for (int i = 0; i < numFaces; i++)
    {
        std::getline(file, line);
        numLine++;

        int nbv, id;
        std::istringstream iss(line);
        std::vector<int> indices;

        // read number of vertices in face
        if (!(iss >> nbv))
        {
            std::cerr << "Unable to read vertex count of face! (at " << numLine << ")" << std::endl;
            return;
        }

        // read indices of vertices connected to the face
        // expects to read at least nbv indices
        for (int i = 0; i < nbv; i++)
        {
            if (iss >> id)
            {
                indices.push_back(id);
            }
            else
            {
                std::cerr << "Unable to read vertex coordinate! (at " << numLine << ")" << std::endl;
                return;
            }
        }

        Face *f = new Face;

        // sort indices list in ascending order
        bubbleSort(indices);

        // gen edge couple [0,1] [1,2] [0,2]
        // we only want edges with v1.id < v2.id
        std::vector<EdgeCouple> edgeCouples;
        edgeCouples.push_back(EdgeCouple(m_vertices[indices[0]], m_vertices[indices[1]]));
        edgeCouples.push_back(EdgeCouple(m_vertices[indices[1]], m_vertices[indices[2]]));
        edgeCouples.push_back(EdgeCouple(m_vertices[indices[0]], m_vertices[indices[2]]));

        // for every edge couples
        for (auto ec : edgeCouples)
        {
            // try to find if the edge already exists
            Edge *e = findEdge(ec.v1, ec.v2);
            if (!e) // if it does't exists, create the edge and insert it in the map
            {
                e = new Edge(ec.v1, ec.v2);
                ec.v1->addEdge(e);
                ec.v2->addEdge(e);

                m_edge_map.insert({{e->v1()->id(), e->v2()->id()}, e});
            }

            // add the current face to the edge
            e->addFace(f);
            // add the edge to the face
            f->addEdge(e);
        }
        // add the face to the mesh
        m_faces.push_back(f);
    }
    // end
    std::cout << "Found " << m_vertices.size() << "vertices and " << m_faces.size() << " faces." << std::endl;

    // put all element of map into a vector
    for(auto it = m_edge_map.begin(); it != m_edge_map.end() ; ++it)
        m_edges.push_back(it->second);
}

void Mesh::exportOBJ(std::string filename)
{
    std::ofstream file;   // output file
    glm::vec3 v1 = glm::vec3();
    glm::vec3 v2 = glm::vec3();
    int k = 1;

    //open file
    file.open(filename);
    if (!file.is_open())
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }

    for(unsigned int i=0; i<m_edges.size(); i++)
    {
        // get vertices
        v1 = m_edges[i]->v1()->pos();
        v2 = m_edges[i]->v2()->pos();

        file << "v " << v1.x << " " << v1.y << " " << v1.z << std::endl;
        file << "v " << v2.x << " " << v2.y << " " << v2.z << std::endl;

        // virtual edges
        if(m_edges[i]->type() == Edge::VIRTUAL)
        {
            file << "g virtual" << std::endl;
            file << "usemtl blue" << std::endl;
        }
        // bones
        else if(m_edges[i]->type() == Edge::BONE)
        {
            file << "g bone" << std::endl;
            file << "usemtl red" << std::endl;
        }

        // line between the vertices
        file << "l " << k << " " << k+1 << std::endl;
        k+=2;
    }

    file.close();
}

void Mesh::exportMesh(std::string filename, std::vector<Mesh *> meshes) {
    std::ofstream file;
    glm::vec3 v1 = glm::vec3();
    glm::vec3 v2 = glm::vec3();
    std::vector<Face*> faces;
    std::vector<Edge*> edges;

    int m =  meshes.size();
    int f, e;
    int i, j, k, l;
    int c;

    file.open(filename);
    if (!file.is_open())
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }

    for (i = 0; i < m; i++)
    {
        //get faces of the current mesh
        faces = meshes[i]->faces();
        f = faces.size();

        file << "g " << m << std::endl;

        for(j = 0; j < f; j++)
        {
            //get edges of the current face
            edges = faces[j]->edges();
            e = edges.size();

            for(k = 0; k< e; k++)
            {
                //add vertices to the file
                v1 = edges[i]->v1()->pos();
                v2 = edges[i]->v2()->pos();

                file << "v " << v1.x << " " << v1.y << " " << v1.z << std::endl;
                file << "v " << v2.x << " " << v2.y << " " << v2.z << std::endl;
            }

            //offset
            c = j*(2*e+2) + m*(f*(2*e+2));

            //add faces to the file
            file << "f ";
            for(l = c; l < 2*k+2 + c; i++)
                file << l << " ";

        }
        file << std::endl;
   }
   file.close();
}



Edge *Mesh::findEdge(Vertex *a, Vertex *b)
{
    std::map<std::pair<int, int>, Edge *>::iterator it = m_edge_map.find({a->id(), b->id()});
    if (it == m_edge_map.end())
        return nullptr;
    else
        return it->second;
}

std::vector<Face*> Mesh::cleanDouble(std::vector<Edge*> l1, std::vector<Edge*> l2) {
    std::vector<Face*> ATL;
    Face* shared;
    bool foundSharedFace = false;

    int faceIndex;
    std::vector<Face*>::iterator faceInList;

    for(unsigned int i=0; i<l1.size(); i++) {
        for(unsigned int j=0; j<l2.size(); j++) {

            // if 2 edges have the same vertices
            if(
                ((l1[i]->v1() == l2[j]->v1())
                && (l1[i]->v2() == l2[j]->v2()))
            ||
                ((l1[i]->v1() == l2[j]->v2())
                && (l1[i]->v2() == l2[j]->v1()))
            ) {

                // we find the face they shared when they were not flattened
                for(unsigned int k=0; k<l1[i]->faces().size() && !foundSharedFace; k++) {
                    for(unsigned int l=0; l<l2[j]->faces().size() && !foundSharedFace; l++) {
                        if(l1[i]->faces()[k] == l2[j]->faces()[l]) {
                            shared = l1[i]->faces()[k];

                            // remove the flattened face from l1 and l2 lists
                            faceInList = std::find(l1[i]->faces().begin(), l1[i]->faces().end(), shared);
                            faceIndex = std::distance(l1[i]->faces().begin(), faceInList);
                            l1[i]->faces().erase(l1[i]->faces().begin() + faceIndex);

                            faceInList = std::find(l2[j]->faces().begin(), l2[j]->faces().end(), shared);
                            faceIndex = std::distance(l2[j]->faces().begin(), faceInList);
                            l2[j]->faces().erase(l2[j]->faces().begin() + faceIndex);

                            // quit the 2 loops
                            foundSharedFace = true;
                        }
                    }
                }

                // we delete the last edge of the shared (and flattened) face (not l1, not l2)
                for(unsigned int k=0; k<shared->edges().size(); k++) {
                    if((shared->edges()[k] != l1[i] && (shared->edges()[k] != l2[j]))) {
                        deleteEdge(shared->edges()[k]);
                        delete shared->edges()[k];
                        break;
                    }
                }

                // and define l1's new construction (linked faces)
                l1[i]->faces().insert(l1[i]->faces().end(), l2[j]->faces().begin(), l2[j]->faces().end());

                delete l2[j];
                ATL.push_back(shared);
                deleteFace(shared);
            }
        }
    }
    return ATL;
}

void Mesh::deleteEdge(Edge* edge) {
    int index;
    std::vector<Edge*>::iterator edgeInMesh;

    edgeInMesh = std::find(m_edges.begin(), m_edges.end(), edge);
    index = std::distance(m_edges.begin(), edgeInMesh);
    m_edges.erase(m_edges.begin() + index);
}

void Mesh::deleteFace(Face* face) {
    int index;
    std::vector<Face*>::iterator faceInMesh;

    faceInMesh = std::find(m_faces.begin(), m_faces.end(), face);
    index = std::distance(m_faces.begin(), faceInMesh);
    m_faces.erase(m_faces.begin() + index);
}

void Mesh::debug() const
{
    for(auto v: m_vertices)
    {
        std::cout << *v << std::endl;
    }
    for(auto e: m_edges)
    {
        std::cout << *e << std::endl;
    }
    for(auto f: m_faces)
    {
        std::cout << *f << std::endl;
    }
}
