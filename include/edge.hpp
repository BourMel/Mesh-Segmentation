#ifndef EDGE_H
#define EDGE_H

#include <vector>
#include <ostream>

class Face;
class Vertex;

class Edge
{
  public:
    typedef enum
    {
        MESH,
        BONE,
        VIRTUAL
    } EdgeType;

    Edge();
    Edge(Vertex *first, Vertex *last);

    /**
     * @brief The cost of an edge is defined by its length.
     * We use squared length to optimize the process as
     * we only need the cost to sort the edges.
     *
     * @return The edge's square length
     */
    float cost() const;

    /*!
	* \brief calculate the position of the middle of the edge and return this vertex
	* \author Morgane R.
	*/
    Vertex *getMeanPosition();

    /**
     * @brief Returns true if there's an intersection between 2 edges
     * @author Méline BL
     */
    bool intersectWith(Edge* e);

    /**
     * @brief Get the Connected Edges object
     *
     * @return std::vector<Edge*>
     * @author Nathan R.
     */
    std::vector<Edge*> getConnectedEdges();

    /**
     * @brief Compute an edge's area (with ATL list) and save it in the object
     * @author Méline BL
     */
    void computeArea();

    void addFace(Face *face);
    void removeFace(Face *face);
    void addFaceATL(Face *face);

    //getter
    int id() const;
    Vertex *v1() const;
    Vertex *v2() const;
    std::vector<Face *> &faces();
    std::vector<Face *> &ATL();
    EdgeType type() const;
    bool isLocked() const;
    float area() const;

    // setter
    void v1(Vertex *v1);
    void v2(Vertex *v2);
    void type(EdgeType type);
    void isLocked(bool b);

    // debug
    friend std::ostream &operator<<(std::ostream &o, Edge &e);

    static bool compEdgePtr(Edge *a, Edge *b) {return a->m_cost < b->m_cost;}
    bool operator< (const Edge &other) const {
        return m_cost < other.m_cost;
    }

  private:
    void init();
    void computeCost();

    Vertex *m_v1, *m_v2;
    std::vector<Face *> m_faces;
    std::vector<Face *> m_ATL;

    EdgeType m_type;
    bool m_isLocked;
    float m_cost;
    float m_area;
    int m_id;
};

#endif
