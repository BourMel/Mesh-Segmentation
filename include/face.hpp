#ifndef FACE_H
#define FACE_H

class Edge;

class Face
{
  public:
    Face(Edge *edge);

    void edge(Edge *edge);

    Edge *edge() const;
    int id() const;

  private:
    int m_id;
    Edge *m_edge;
};

#endif