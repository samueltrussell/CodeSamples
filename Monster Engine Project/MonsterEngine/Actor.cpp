//Member function definitions for the Actor class
#include "Actor.h"

#include <iostream>
#include <iomanip>

#include "HashedString.h"
#include "StringPool.h"

extern Engine::StringPool * g_pStringPool;

namespace Engine {

	Actor::Actor(Vector3 i_position, Vector3 i_velocity, const char * i_pName, HashedString i_Type) :
		position(i_position),
		velocity(i_velocity),
		_pName(g_pStringPool->add(i_pName)),
		_Type(i_Type)
	{
		externalForces.SetParameters(0.0, 0.0, 0.0);
	}

	Actor::~Actor()
	{
		
	}

	bool Actor::IsType(HashedString i_Type) {
		return (_Type == i_Type);
	}


}//namespace Engine