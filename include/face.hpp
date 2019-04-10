#ifndef FACE_H
#define FACE_H

class HalfEdge;

class Face
{
  public:
    Face(HalfEdge *edge);

  private:
    int m_id;
    HalfEdge *m_edge;
};

#endif