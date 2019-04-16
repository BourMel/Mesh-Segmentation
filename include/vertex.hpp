#ifndef VERTEX_H
#define VERTEX_H

#include <vector>

#include "vec3.hpp"

class Edge;

class Vertex
{
  public:
    Vertex(Vec3f pos, Edge *edge = nullptr);

    // TODO
    // getAdjFaces()

    // getter
    const Vec3f& pos() const;
    int id() const;
    Edge * edge() const;
    
    //setter
    void edge(Edge *e);
    void pos(Vec3f p);

  private:
    std::vector<Vertex *> getNeighbours(); // to public ?
    int m_id;
    Vec3f m_pos;
    Edge * m_edge;
};

#endif