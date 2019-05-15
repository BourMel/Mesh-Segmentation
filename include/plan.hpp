#ifndef PLAN_H
#define PLAN_H

#include <glm/vec3.hpp>
#include <glm/glm.hpp>

class Plan
{
	public:
		Plan(glm::vec3 p, glm::vec3 v);

		glm::vec3 point();
		glm::vec3 vecNorm();

		void point(glm::vec3 p);
		void vecNorm(glm::vec3 v);

		void nextPoint();
		int relativePostion(glm::vec3 v);
	private:
		glm::vec3 m_point;
		glm::vec3 m_vecteurNormale;
};

#endif
