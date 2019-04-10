#ifndef MESH_H
#define MESH_H

#include <vector>
#include <fstream>

#include "vertex.hpp"
#include "face.hpp"

class Mesh
{
  public:
    Mesh();

    void importOFF(std::string filename);
    void exportOFF();
    void exportOBJ();

    const std::vector<Face *> &faces() { return m_faces; }
    const std::vector<Vertex *> &vertices() { return m_vertices; }

  private:

    void append_vertex(Vertex *vertex);
    void append_face(Face *face);
    
    std::vector<Face *> m_faces;
    std::vector<Vertex *> m_vertices;
};

#endif