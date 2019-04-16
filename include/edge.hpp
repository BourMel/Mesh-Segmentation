#ifndef EDGE_H
#define EDGE_H

class Face;
class Vertex;

class Edge
{
  public:

    typedef enum {MESH, BONE, VIRTUAL} EdgeType;

    Edge(Vertex *first, Vertex* last);

    // The edge's squared length
    float cost();

    // Associated Triangle List, the list of all faces around first and last
    std::vector<Face*> getATL();
    // WARN: the ATL represent faces to be deleted (get iteraor ?)

    // setter
    void first(Vertex *v);
    void last(Vertex *v);
    void faceRight(Face *f);
    void faceLeft(Face *f);
    void previousEdgeLeft(Edge *he);
    void previousEdgeRight(Edge *he);
    void nextEdgeLeft(Edge *he);
    void nextEdgeRight(Edge *he);

    // getter
    Vertex *first() const;
    Vertex *last() const;
    Face *faceLeft() const;
    Face *faceRight() const;
    Edge *previousEdgeLeft() const;
    Edge *previousEdgeRight() const;
    Edge *nextEdgeLeft() const;
    Edge *nextEdgeRight() const;

  private:
    Vertex *m_first, *m_last;
    Face *m_faceL, *m_faceR;
    Edge *m_nextR, *m_nextL, *m_prevR, *m_prevL;

    EdgeType m_type;
    bool isProtected;
};

#endif