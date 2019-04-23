#ifndef EDGE_H
#define EDGE_H

#include <vector>

class Face;
class Vertex;

class Edge
{
  public:
    typedef enum
    {
        MESH,
        BONE,
        VIRTUAL
    } EdgeType;

    Edge(Vertex *first, Vertex *last);

    /**
     * @brief The cost of an edge is defined by its length.
     * We use squared length to optimize the process as
     * we only need the cost to sort the edges.
     * 
     * @return The edge's square length
     */
    float cost();

    /*!
    * \brief sort and merge two subarrays of Edge
    *
    * \param e : array of Edge
    * \param l : left index of the first subarray
    * \param m : right index of the first subarray, left index of the second subarray
    * \param r : right index of the second subarray
    * \author Tiphaine R.
    */
    void merge(Edge *e, int l, int m, int r);

    /*!
    * \brief merge sort of an array of Edge
    *
    * must copy e before executing the function or it will be overwriten
    * initialization : l = 0, r = e.size() -1
    *
    * \param e : array of Edge
    * \param l : left index of the first subarray
    * \param r : right index of the second subarray
    * \author Tiphaine R.
    */
    void mergeSort(Edge *e, int l, int r);

    /*!
	* \brief calculate the position of the middle of the edge and return this vertex
	* \author Morgane R.
	*/
    Vertex *getMeanPosition();

    // Associated Triangle List, the list of all faces around first and last
    std::vector<Face *> getATL();
    // WARN: the ATL represent faces to be deleted (get iteraor ?)

    void addFace(Face *face);

    //getter
    Vertex *v1();
    Vertex *v2();

    // setter
    void v1(Vertex *v1);
    void v2(Vertex *v2);

  private:
    Vertex *m_v1, *m_v2;
    std::vector<Face *> m_faces;

    EdgeType m_type;
    bool isProtected;
};

#endif
