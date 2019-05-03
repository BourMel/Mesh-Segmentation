#ifndef VERTEX_H
#define VERTEX_H

#include <vector>
#include <ostream>
#include <glm/vec3.hpp>
#include <glm/vector_relational.hpp>

class Edge;

class Vertex
{
  public:
    Vertex(glm::vec3 pos);

    // TODO
    // getAdjFaces()

	/*!
	 * \brief Remplace the vertex from edgeToDelete by the current vertex in toBind
	 * \param toBind : vector of edges that are incident of one vertex from edgeToDelete
	 * \parma edgeToDelete  
	 * \author Morgane R.
	 */
	void bindVertexEdges(std::vector<Edge*> toBind, Edge* edgeToDelete);

  void bindEdges(std::vector<Edge*> toBind, Edge* toDelete);

  void addEdge(Edge *e);
  void removeEdge(Edge *e);

    // getter
    const glm::vec3& pos() const;
    int id() const;
    std::vector<Edge *> &edges();
    bool locked();

    //setter
    void pos(glm::vec3 p);
    void lock(bool l);

    // debug
    friend std::ostream &operator<<(std::ostream &o, Vertex &v);

  private:
    std::vector<Edge *> m_edges;
    int m_id;
    glm::vec3 m_pos;
    bool m_locked;
};

#endif
