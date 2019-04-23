#ifndef VERTEX_H
#define VERTEX_H

#include <vector>

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

  void addEdge(Edge *e);

    // getter
    const glm::vec3& pos() const;
    int id() const;
    std::vector<Edge *> &edges();

    //setter
    void pos(glm::vec3 p);

  private:
    std::vector<Edge *> m_edges;
    int m_id;
    glm::vec3 m_pos;
};

#endif
