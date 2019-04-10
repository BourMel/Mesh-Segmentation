#ifndef EDGE_H
#define EDGE_H

class Face;
class Vertex;

class Edge
{
  public:
    Edge(Vertex *first, Vertex* last);

    // setter
    void first(Vertex *v);
    void last(Vertex *v);
    void faceRight(Face *f);
    void faceLeft(Face *f);
    void previousEdge1(Edge *he);
    void previousEdge2(Edge *he);
    void nextEdge1(Edge *he);
    void nextEdge2(Edge *he);

    // getter
    Vertex *first() const;
    Vertex *last() const;
    Face *faceLeft() const;
    Face *faceRight() const;
    Edge *previousEdge1() const;
    Edge *previousEdge2() const;
    Edge *nextEdge1() const;
    Edge *nextEdge2() const;

  private:
    Vertex *m_first, *m_last;
    Face *m_left, *m_right;
    Edge *m_next1, *m_next2, *m_prev1, *m_prev2;
};

#endif