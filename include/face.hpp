#ifndef FACE_H
#define FACE_H

#include <vector>

class Edge;

class Face
{
  public:
    Face();

    void addEdge(Edge *e);

    std::vector<Edge*> &edges();

    int id() const;

  private:
    std::vector<Edge*> m_edges;
    int m_id;
};

#endif