#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <glm/glm.hpp>
#include <string>

class Edge;
class Vertex;
class Face;

class Mesh
{
public:
    Mesh();
    Mesh(std::string filename);

    int importOBJ(std::string filename);
    int importOFF(std::string filename);
    int exportOBJ(std::string filename);

    void skeletonization();
    void dissolveEdge(Edge *e);

    void removeEdge(Edge *e);
    void removeVertex(Vertex *v);

    void debug();

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

    std::vector<Edge*> m_edges;
    std::vector<Vertex*> m_vertices;

    std::vector<glm::vec3> m_raw_vertices;
    std::vector<Face*> m_faces;
};

#endif // MESH_HPP
