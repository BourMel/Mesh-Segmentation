#include "mesh.hpp"
#include "edge.hpp"
#include "vertex.hpp"
#include "face.hpp"
#include "plane.hpp"

#include <iostream>

void Mesh::segmentation(Mesh* initial, float step = 1.0f)
{
	m_bones = m_edges;
	m_edges = initial->edges();

	Plane *sweepPlane;
  	std::vector<Edge*> candidates;
  	Mesh *intersectionWithPlane = new Mesh();
  	Mesh *crossSection = new Mesh();

  	// some operations made only after a given number of iterations
	int countIterations = 0;
	// 5 last values of F (area or perimeter of the cross section)
	float F1 = 0.0f;
	float F2 = 0.0f;
	float F3 = 0.0f;
	float F4 = 0.0f;
	float F5 = 0.0f;

	// dérivations
	float d1 = 0.0f;
	float d2 = 0.0f;
	float previousD3 = 0.0f;
	float currentD3 = 0.0f;
	float nextD3 = 0.0f;

	// H : nb of simple polygons in the cross section
	int currentH = 0;
	int previousH = 0;
	float perimeter;

	int topology;

	std::vector<Mesh *> components;
	Mesh* currentComponent = new Mesh();

	// compute all bones area
	for(unsigned int i=0; i<m_bones.size(); i++) {
    	float currentArea = 0.0f;

    	for(unsigned int j=0; j<m_bones.at(i)->ATL().size(); j++) {
        	currentArea += m_faces.at( m_bones.at(i)->ATL().at(j) )->area();
    	}

    	m_bones.at(i)->area(currentArea);
  	}


}