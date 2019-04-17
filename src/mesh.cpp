#include "vec3.hpp"

#include "mesh.hpp"
#include "edge.hpp"
#include "face.hpp"


#include <iostream>
#include <string>
#include <sstream>

Mesh::Mesh()
{
}

void Mesh::importOFF(std::string filename)
{
    std::cout << "filename: " << filename << std::endl;
    std::ifstream file;
    std::string line;
    std::stringstream ss;

    int numLine = 0;
    int numVert, numFaces;
    std::string str;

    // vars


    file.open(filename);
    if (!file.is_open())
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }

    // detect OFF
    std::getline(file, line);
    ss.str(line);
    if (!(ss >> str && str == "OFF"))
    {
        std::cerr << "file token is not OFF" << std::endl;
        return;
    }

    std::getline(file, line);
    file >> numVert >> numFaces;

    std::cout << numVert << " " << numFaces << std::endl;

    float x, y, z;
    // read vertices info
    while (std::getline(file, line))
    {
        std::istringstream iss(line);

        if (!(iss >> x >> y >> z))
        {
            std::cout << "wrong format" << std::endl;
        }
        std::cout << x << y << z << std::endl;
        //Vec3f v = Vec3f(x,y,z);
        //append_vertex(new Vertex({x,y,z}));
    }

    // read face composition
    while (std::getline(file, line))
    {
        int nbv, id;
        std::istringstream iss(line);
        if (!(iss >> nbv))
        {
            // erreur
        }
        // read number of vertices in face
        for (int i = 0; i < nbv; i++)
        {
            if (!(iss >> id))
            {
                std::cerr << "Unable to read vertex coordinate!" << std::endl;
            }
            else
            {
                // gen face & edge
            }
        }
    }
}

void exportOFF(std::string filename)
{

}

void exportOBJ(std::string filename)
{

}

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

void Mesh::append_vertex(Vertex *vertex)
{
    if (vertex)
    {
        m_vertices.push_back(vertex);
    }
}

void Mesh::append_face(Face *face)
{
    if (face)
    {
        m_faces.push_back(face);
    }
}
