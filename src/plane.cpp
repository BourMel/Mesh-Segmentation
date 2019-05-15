#include "plane.hpp"

Plane::Plane(glm::vec3 p, glm::vec3 v) {
	m_point = p;
	m_vecteurNormale = v;
}

glm::vec3 Plane::point() {
	return m_point;
}

glm::vec3 Plane::vecNorm(){
	return m_vecteurNormale;
}

void Plane::point(glm::vec3 p){
	m_point = p;
}

void Plane::vecNorm(glm::vec3 v){
	m_vecteurNormale = v;
}

void Plane::nextPoint(){
	m_point += m_vecteurNormale;
}

int Plane::relativePosition(glm::vec3 v){
	float dotProduct = glm::dot(m_point-v, m_vecteurNormale);
	if(dotProduct < -0.001)
		return -1;
	else if(dotProduct > 0.001)
		return 1;
	else
		return 1;
}
