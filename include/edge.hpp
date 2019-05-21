#ifndef EDGE_HPP
#define EDGE_HPP

#include <vector>
#include <ostream>
#include "utils.hpp"

class Vertex;
class Face;

class Edge
{
public:
    typedef enum
    {
        MESH,
        VIRTUAL,
        BONE
    } EdgeType;

    Edge();
    Edge(Vertex *v1, Vertex *v2);

    std::vector<Edge *> getConnectedEdges();
    Vertex *getMeanPosition();

    void addFace(ID f);
    void removeFace(ID f);
    void addFaceATL(std::size_t id);

    static void resetId();
    glm::vec3 getNormal();

    void v1(Vertex *v1);
    void v2(Vertex *v2);
    void type(EdgeType type);
    void area(float area);

    Vertex *v1() const;
    Vertex *v2() const;
    std::vector<ID> &faces();
    std::vector<ID> &ATL();
    EdgeType type() const;
    float cost() const;
    ID id() const;

    static bool compEdgeId(Edge *a, Edge *b);
    static bool compEdgeCost(Edge *a, Edge *b);
    static bool compEdgeArea(Edge *a, Edge *b);

    friend std::ostream& operator<<(std::ostream &o, Edge &e);

private:
    void computeCost();
    void init();

    ID m_id;
    Vertex *m_v1, *m_v2;
    std::vector<ID> m_faces;
    std::vector<ID> m_ATL;
    EdgeType m_type;
    float m_cost;
    float m_area;

    static ID m_gid;
};

#endif // EDGE_HPP
