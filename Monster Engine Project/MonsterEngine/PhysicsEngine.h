
#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include <vector>

#include "PhysicsInfo.h"

namespace Engine
{
	class PhysicsEngine
	{
	public:
		PhysicsEngine(Vector3 i_gravity);
		~PhysicsEngine();

		bool AddPhysicsObject(Actor * i_Actor, const float i_mass, const float i_drag, const float i_gravityScalar);
	
		void Update(const double i_frameTime);
		void ApplyPhysics(PhysicsInfo * i_object, const double i_frameTime);
	
	private:
		std::vector<PhysicsInfo *> _physicsObjects;
		Vector3 _gravity;
	
	
	};

}// namespace Engine
#endif //PHYSICS_ENGINE_H
