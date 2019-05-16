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
     * @return Error code
     * @author Nathan R.
     */
    int importOFF(std::string filename);

    /**
     * @brief Import OBJ file
     *
     * @param filename The path to the file
     * @return Error code
     * @author Nathan R.
     */
    int importOBJ(std::string filename);

    /**
     * @brief Export skeleton to Obj file
     *
     * @param filename The path to the file
     * @author Tiphaine R.
     */
    void exportOBJ(std::string filename);

    /**
     * @brief Export mesh to Obj file
     *
     * @param filename The path to the file
     * @param meshes Meshes to export
     * @author Tiphaine R.
     */
    void exportMesh(std::string filename, std::vector<Mesh *> meshes);


    /**
    * @brief Delete edges present in both lists (because they link
    * the 2 same vertices) and delete the flattened faces
    *
    * @param l1 : list of edges
    * @param l2 : list of edges
    * @return List of incident faces that were flattened
    * @author Méline BL
    */
    std::vector<Face*> cleanDouble(std::vector<Edge*> l1, std::vector<Edge*> l2);

    /**
    * @brief Delete an edge (in the mesh)
    * @param edge : pointer
    * @author Méline BL
    */
    void deleteEdge(Edge* edge);
    /**
    * @brief Delete a face (in the mesh)
    * @param face : pointer
    * @author Méline BL
    */
    void deleteFace(Face* face);
    
    /**
    * @brief Adds an edge to the mesh
    * @param edge : pointer
    * @author Méline BL & Morgane R.
    */
    void addEdge(Edge* e);
    /**
    * @brief Adds a face to the mesh
    * @param face : pointer
    * @author Méline BL & Morgane R.
    */
    void addFace(Face* f);

    /**
    * @brief Construct faces of a polygon with its edges only
    * @author Méline BL
    */
    void constructFaces();

    /**
     * @brief skeletonization of a mesh
     * @author Nathan R.
     */
    void skeletonization();
    /**
     * @brief segmentation of a mesh, using its skeleton
     * @author Méline BL & Morgane R.
     */
    void segmentation();

    void dissolveEdge(Edge* edge);

    const std::vector<Face *> &faces() { return m_faces; }
    const std::vector<Edge *> &edges() { return m_edges; }
    const std::vector<Vertex *> &vertices() { return m_vertices; }

    void debug() const;

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

    std::vector<Edge *> m_bones;
    std::vector<Vertex *> m_bone_vertices;
    int m_nbBV;
};

#endif
