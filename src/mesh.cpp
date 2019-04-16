#include "vec3.hpp"

#include "mesh.hpp"
#include "edge.hpp"


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