
#include "PhysicsInfo.h"

namespace Engine
{
	PhysicsInfo::PhysicsInfo(SmartPointer<Actor> i_Actor, const float i_mass, const float i_friction, const float i_drag, const float i_gravityScalar) :
		_pActor(i_Actor),
		_mass(i_mass),
		_friction(i_friction),
		_drag(i_drag),
		_gravityScalar(i_gravityScalar)
	{

	}

	PhysicsInfo::~PhysicsInfo()
	{

	}
}//namespace Engine