//class definition and member function declarations for the Actor class

#ifndef ACTOR_H
#define ACTOR_H

#include "HashedString.h"
#include "IActorController.h"
#include "Vector3.h"


namespace Engine {
	
	class Actor
	{
	public:
		Actor(Vector3 i_position, Vector3 i_velocity, const char * i_name, HashedString i_Type);
		~Actor();
		Vector3 position;
		Vector3 velocity;
		Vector3 externalForces;
		IActorController * controller;
		bool IsType(HashedString i_Type);

	private:
		const char * _pName;
		HashedString _Type;

	};

}//namespace Engine
#endif //ACTOR_H