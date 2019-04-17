#ifndef MESH_H
#define MESH_H

#include <vector>
#include <fstream>

#include "vec3.hpp"
#include "vertex.hpp"
#include "face.hpp"

class Mesh
{
  public:
    Mesh();

    void importOFF(std::string filename);
    void exportOFF(std::string filename);
    void exportOBJ(std::string filename);

    /*!
    * \brief Delete edges present in both lists (because they link
    * the 2 same vertices) and delete the flattened faces 
    *
    * \param l1 : list of edges
    * \param l2 : list of edges
    * \return List of incident faces that were flattened
    * \author Méline BL
    */
    static std::vector<Face*> cleanDouble(std::vector<Edge*> l1, std::vector<Edge*> l2);


    const std::vector<Face *> &faces() { return m_faces; }
    const std::vector<Vertex *> &vertices() { return m_vertices; }

  private:

    void append_vertex(Vertex *vertex);
    void append_face(Face *face);

    std::vector<Face *> m_faces;
    std::vector<Vertex *> m_vertices;
};

#endif
