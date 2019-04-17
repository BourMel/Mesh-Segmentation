#ifndef VERTEX_H
#define VERTEX_H

#include <vector>

//#include "vec3.hpp"
#include <glm/vec3.hpp>
#include <glm/vector_relational.hpp>

class Edge;

class Vertex
{
  public:
    Vertex(glm::vec3 pos, Edge *edge = nullptr);

    // TODO
    // getAdjFaces()

    /*!
     * \brief Search the edges incident to the vertex
     * \return A vector of pointers
     * \author Méline BL
     */
    std::vector<Edge*> getIncidentEdges();

	/*!
	 * \brief Remplace the vertex from edgeToDelete by the current vertex in toBind
	 * \param toBind : vector of edges that are incident of one vertex from edgeToDelete
	 * \parma edgeToDelete  
	 * \author Morgane R.
	 */
	void bindVertexEdges(std::vector<Edge*> toBind, Edge* edgeToDelete);

    // getter
    const glm::vec3& pos() const;
    int id() const;
    Edge * edge() const;

    //setter
    void edge(Edge *e);
    void pos(glm::vec3 p);

  private:
    std::vector<Vertex *> getNeighbours(); // to public ?
    int m_id;
    glm::vec3 m_pos;
    Edge * m_edge;
};

#endif
