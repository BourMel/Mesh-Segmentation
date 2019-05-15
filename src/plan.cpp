#include "plan.hpp"

Plan::Plan(glm::vec3 p, glm::vec3 v) {
	m_point = p;
	m_vecteurNormale = v;
}

glm::vec3 Plan::point() {
	return m_point;
}

glm::vec3 Plan::vecNorm(){
	return m_vecteurNormale;
}

void Plan::point(glm::vec3 p){
	m_point = p;
}

void Plan::vecNorm(glm::vec3 v){
	m_vecteurNormale = v;
}

void Plan::nextPoint(){
	m_point += m_vecteurNormale;
}

int Plan::relativePostion(glm::vec3 v){
	float dotProduct = glm::dot(m_point-v, m_vecteurNormale);
	if(dotProduct < -0.001)
		return -1;
	else if(dotProduct > 0.001)
		return 1;
	else
		return 1;
}
