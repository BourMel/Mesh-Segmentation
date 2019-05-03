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
    /**
     * @brief Test if the face is a simple polygon (no intersection between edges)
     * @author Méline BL
     */
    bool isSimple();

    std::vector<Edge*> &edges();

    int id() const;

    //debug
    friend std::ostream &operator<<(std::ostream &o, Face &e);

  private:
    std::vector<Edge*> m_edges;
    int m_id;
};

#endif
