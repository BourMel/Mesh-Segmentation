#ifndef EDGE_H
#define EDGE_H

#include <vector>

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

    Edge(Vertex *first, Vertex *last);

    /**
     * @brief The cost of an edge is defined by its length.
     * We use squared length to optimize the process as
     * we only need the cost to sort the edges.
     * 
     * @return The edge's square length
     */
    float cost();

    /*!
	* \brief calculate the position of the middle of the edge and return this vertex
	* \author Morgane R.
	*/
    Vertex *getMeanPosition();

    /**
     * @brief Get the Connected Edges object
     * 
     * @return std::vector<Edge*>
     * @author Nathan R.
     */
    std::vector<Edge*> getConnectedEdges();

    // Associated Triangle List, the list of all faces around first and last
    std::vector<Face *> getATL();
    // WARN: the ATL represent faces to be deleted (get iteraor ?)

    void addFace(Face *face);

    //getter
    Vertex *v1();
    Vertex *v2();
    std::vector<Face *> &faces();
    EdgeType type();

    // setter
    void v1(Vertex *v1);
    void v2(Vertex *v2);
    void type(EdgeType type);

  private:
    Vertex *m_v1, *m_v2;
    std::vector<Face *> m_faces;

    EdgeType m_type;
    bool isProtected;
    float m_cost;
};

#endif
