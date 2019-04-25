#include "mesh.hpp"
#include "edge.hpp"
#include "face.hpp"
#include "utils.hpp"

#include <iostream>
#include <string>
#include <sstream>

Mesh::Mesh()
{
}

void Mesh::skeletonization()
{
    mergeSort(m_edges, 0, m_edges.size()/2);
    for(std::size_t i = 0; i < m_edges.size(); i++)
    {
        Edge *e = m_edges[i];
        if(e->faces().size() > 0) // if an edge is connect to at least one face
        {
            Vertex *mean = e->getMeanPosition();
            std::vector<Edge*> connected = e->getConnectedEdges();
            //mean->bindVertexEdges(connected, e);
            //mean->cleanDoubles();
        }
        else // if an edge is not connected to any face, lock it
        {
            e->type(Edge::BONE);
            e->v1()->lock(true);
            e->v2()->lock(true);
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
    std::cout << "Found " << m_vertices.size() << " and " << m_faces.size() << " faces." << std::endl;
    
    // put all element of map into a vector
    for(auto it = m_edge_map.begin(); it != m_edge_map.end() ; ++it)
        m_edges.push_back(it->second);
}

Edge *Mesh::findEdge(Vertex *a, Vertex *b)
{
    std::map<std::pair<int, int>, Edge *>::iterator it = m_edge_map.find({a->id(), b->id()});
    if (it == m_edge_map.end())
        return nullptr;
    else
        return it->second;
}

/*
std::vector<Face*> Mesh::cleanDouble(std::vector<Edge*> l1, std::vector<Edge*> l2) {
    std::vector<Face*>::iterator sharedInMesh;
    int index;
    std::vector<Face*> ATL;
    Face* shared;

    for(unsigned int i=0; i<l1.size(); i++) {
        for(unsigned int j=0; j<l2.size(); j++) {

            // if 2 edges have the same vertices
            if(
                ((l1[i]->first()->id() == l2[j]->first()->id())
                && (l1[i]->last()->id() == l2[j]->last()->id()))
            ||
                ((l1[i]->first()->id() == l2[j]->last()->id())
                && (l1[i]->last()->id() == l2[j]->first()->id()))
            ) {

                if(l1[i]->faceLeft()->id() == l2[j]->faceLeft()->id()) {
                    // we find the face they shared when they were not flattened
                    shared = l1[i]->faceLeft();

                    // we delete the last edge of the shared (and flattened) face (not l1, not l2)
                    if(l1[i]->nextEdgeLeft()->id() == l2[j]->id()) {
                        delete l1[i]->previousEdgeLeft();
                    } else {
                        delete l1[i]->nextEdgeLeft();
                    }

                    // and we define l1's new construction (linked edges and faces)
                    l1[i]->faceLeft(l2[j]->faceRight());
                    l1[i]->previousEdgeLeft(l2[j]->previousEdgeRight());
                    l1[i]->nextEdgeLeft(l2[j]->nextEdgeRight());

                } else if(l1[i]->faceLeft()->id() == l2[j]->faceRight()->id()) {
                    shared = l1[i]->faceLeft();

                    if(l1[i]->nextEdgeLeft()->id() == l2[j]->id()) {
                        delete l1[i]->previousEdgeLeft();
                    } else {
                        delete l1[i]->nextEdgeLeft();
                    }

                    l1[i]->faceLeft(l2[j]->faceLeft());
                    l1[i]->previousEdgeLeft(l2[j]->previousEdgeLeft());
                    l1[i]->nextEdgeLeft(l2[j]->nextEdgeLeft());

                } else if(l1[i]->faceRight()->id() == l2[j]->faceRight()->id()) {
                    shared = l1[i]->faceRight();

                    if(l1[i]->nextEdgeRight()->id() == l2[j]->id()) {
                        delete l1[i]->previousEdgeRight();
                    } else {
                        delete l1[i]->nextEdgeRight();
                    }

                    l1[i]->faceRight(l1[i]->faceLeft());
                    l1[i]->previousEdgeRight(l1[i]->previousEdgeLeft());
                    l1[i]->nextEdgeRight(l1[i]->nextEdgeLeft());

                    l1[i]->faceLeft(l2[j]->faceLeft());
                    l1[i]->previousEdgeLeft(l2[j]->previousEdgeLeft());
                    l1[i]->nextEdgeLeft(l2[j]->nextEdgeLeft());

                } else if(l1[i]->faceRight()->id() == l2[j]->faceLeft()->id()) {
                    shared = l1[i]->faceRight();

                    if(l1[i]->nextEdgeRight()->id() == l2[j]->id()) {
                        delete l1[i]->previousEdgeRight();
                    } else {
                        delete l1[i]->nextEdgeRight();
                    }

                    l1[i]->faceRight(l2[j]->faceRight());
                    l1[i]->previousEdgeRight(l2[j]->previousEdgeRight());
                    l1[i]->nextEdgeRight(l2[j]->nextEdgeRight());

                } else {
                    std::cerr << "Edge::cleanDouble : 2 edges with the same vertices don't share a face" << std::endl;
                    return ATL;
                }

                delete l2[j];
                ATL.push_back(shared);

                // delete the shared face in the mesh
                sharedInMesh = std::find(m_faces.begin(), m_faces.end(), shared);
                index = std::distance(m_faces.begin(), sharedInMesh);
                m_faces.erase(m_faces.begin() + index);
                delete shared;
            }
        }
    }
    return ATL;
}
*/
