
#ifndef PHYSICS_INFO_H
#define PHYSICS_INFO_H

#include "Actor.h"
#include "SmartPointer.h"

namespace Engine
{
	class PhysicsInfo
	{
	public:
		PhysicsInfo(SmartPointer<Actor> i_Actor, const float i_mass, const float i_friction, const float i_drag, const float i_gravityScalar);
		~PhysicsInfo();
		//inline Actor * const getActorPtr() const;
		inline SmartPointer<Actor> getActorPtr();
		inline const float Mass() const;
		inline const float Friction() const;
		inline const float Drag() const;
		inline const float GravityScalar() const;

	private:
		//Actor * _pActor;
		SmartPointer<Actor> _pActor;
		float _mass;
		float _friction;
		float _drag;
		float _gravityScalar;
	};

}// namespace Engine

#include "PhysicsInfo-inl.h"

#endif //PHYSICS_INFO_H
