#define GLM_ENABLE_EXPERIMENTAL

#include "edge.hpp"
#include "vertex.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

Edge::Edge(Vertex *first, Vertex* last)
{
    m_v1 = first;
    m_v2 = last;
}

float Edge::cost()
{
    return glm::length2(m_v2->pos()-m_v1->pos());
}


void Edge::merge(Edge* e, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* Copy data to L[] and R[] */
    Edge * L = (Edge*)malloc(n1*sizeof(Edge));
    Edge * R = (Edge*)malloc(n2*sizeof(Edge));

    for (i = 0; i < n1; i++)
        L[i] = e[l + i];

    for (i = 0; i < n2; i++)
        R[i] = e[m + 1 + i];

    // Merge tmp arrays in e[l..r]
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        //compare the cost
        if (L[i].cost() <= R[j].cost())
        {
            e[k] = L[i];
            i++;
        }
        else
        {
            e[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[] and R[], if there are any
    while (i < n1)
    {
        e[k] = L[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        e[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}


void Edge::mergeSort(Edge * e, int l, int r)
{
    if (l < r)
    {
        // Sort the two half of the array
        int m = (l+r)/2;
        mergeSort(e, l, m);
        mergeSort(e, m+1, r);

        // Merge the two arrays
        merge(e, l, m, r);
    }
}

Vertex *Edge::getMeanPosition() {
	return new Vertex( glm::vec3((m_v1->pos().x + m_v2->pos().x )/2,
	(m_v1->pos().y + m_v2->pos().y)/2,
	(m_v1->pos().z + m_v2->pos().z)/2));
}

void Edge::addFace(Face *face)
{
    m_faces.push_back(face);
}

//getter
Vertex *Edge::v1() { return m_v1; }
Vertex *Edge::v2() { return m_v2; }

// setter
void Edge::v1(Vertex *v1) { m_v1 = v1; }
void Edge::v2(Vertex *v2) { m_v2 = v2; }
