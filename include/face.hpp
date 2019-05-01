#ifndef FACE_H
#define FACE_H

#include <ostream>
#include <vector>

class Edge;

class Face
{
  public:
    Face();

    void addEdge(Edge *e);

    std::vector<Edge*> &edges();

    int id() const;

    //debug
    friend std::ostream &operator<<(std::ostream &o, Face &e);

  private:
    std::vector<Edge*> m_edges;
    int m_id;
};

#endif