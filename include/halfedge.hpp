#ifndef HALFEDGE_H
#define HALFEDGE_H

class Face;
class Vertex;

class HalfEdge
{
  public:
    HalfEdge();

    // setter
    void source(Vertex *v);
    void face(Face *f);
    void previous_edge(HalfEdge *he);
    void next_edge(HalfEdge *he);
    void adjacent_edge(HalfEdge *he);

    // getter
    Vertex *source() const;
    Face *face() const;
    HalfEdge *previous_edge() const;
    HalfEdge *next_edge() const;
    HalfEdge *adjacent_edge() const;

  private:
    Vertex *m_source;
    Face *m_face;
    HalfEdge *m_next, *m_prev, *m_adj;
};

#endif