#ifndef MESH_H
#define MESH_H

#include <vector>
#include <map>
#include <fstream>
#include <algorithm>

#include "vertex.hpp"
#include "face.hpp"

class Mesh
{
  public:
    Mesh();

    /**
     * @brief Import OFF file
     *
     * @param filename The path to the file
     * @author Nathan R.
     */
    void importOFF(std::string filename);
    //void exportOFF(std::string filename);

    /**
     * @brief Export Obj file
     *
     * @param filename The path to the file
     * @author Tiphaine R.
     */
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
    std::vector<Face*> cleanDouble(std::vector<Edge*> l1, std::vector<Edge*> l2);

    /*!
    * \brief Delete an edge (in the mesh)
    * \param edge : pointer
    * \author Méline BL
    */
    void deleteEdge(Edge* edge);
    /*!
    * \brief Delete a face (in the mesh)
    * \param face : pointer
    * \author Méline BL
    */
    void deleteFace(Face* face);

    void skeletonization();

    void dissolveEdge(Edge* edge);

    const std::vector<Face *> &faces() { return m_faces; }
    const std::vector<Edge *> &edges() { return m_edges; }
    const std::vector<Vertex *> &vertices() { return m_vertices; }

  private:

    typedef struct EdgeCouple
    {
        EdgeCouple(Vertex *a, Vertex *b)
        {
            v1 = a;
            v2 = b;
        }
        Vertex *v1;
        Vertex *v2;
    } EdgeCouple;

    /**
     * @brief Tries to find an existing edge in m_edges by vertices.
     *
     * @param a First vertex
     * @param b Second vertex
     * @return The pointer to the edge if it exists, nullptr otherwise.
     * @author Nathan R.
     */
    Edge *findEdge(Vertex *a, Vertex *b);

    std::vector<Face *> m_faces;
    std::vector<Edge *> m_edges;
    std::map<std::pair<int, int>, Edge *> m_edge_map;
    std::vector<Vertex *> m_vertices;
};

#endif
