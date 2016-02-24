
#include "PhysicsEngine.h"

#include "DebugLog.h"

namespace Engine
{
	PhysicsEngine::PhysicsEngine(Vector3 i_gravity) :
		_gravity(i_gravity)
	{

	}

	PhysicsEngine::~PhysicsEngine()
	{
		for (int i = 0; i < _physicsObjects.size(); i++)
		{
			delete _physicsObjects[i];
		}
	}

	bool PhysicsEngine::AddPhysicsObject(Actor * i_Actor, const float i_mass, const float i_drag, const float i_gravityScalar)
	{
		PhysicsInfo * newInfo = new PhysicsInfo(i_Actor, i_mass, i_drag, i_gravityScalar);

		_physicsObjects.push_back(newInfo);

		return true;
	}

	void PhysicsEngine::ApplyPhysics(PhysicsInfo * i_object, double i_frameTime)
	{
		Actor * actor = i_object->getActorPtr();
		//Sum up the forces on the object

		//gravity
		Vector3 force = _gravity * i_object->Mass() * i_object->GravityScalar();
		//drag
		force -= i_object->Drag() * i_object->getActorPtr()->velocity;
		//Applied Forces
		force += i_object->getActorPtr()->externalForces;

		//Compute Acceleration for the frame
		// A = f/m
		Vector3 acceleration = force * (1.0f/i_object->Mass());
		
		//Update Velocity
		// V = V0 + a*T
		Vector3 velocity = actor->velocity + acceleration * i_frameTime;

		//Update Position
		// P = P0 + V0*T + 1/2*a*T^2;
		Vector3 position = actor->position + actor->velocity * i_frameTime + (acceleration * i_frameTime * i_frameTime) * .5;

		//Push updated position and velocity back into the Actor
		actor->position = position;
		actor->velocity = velocity;
		
	}

	void PhysicsEngine::Update(const double i_frameTime)
	{
		for (int i = 0; i < _physicsObjects.size(); i++) 
		{
			ApplyPhysics(_physicsObjects[i], i_frameTime);
		}
	}

}//namespace Engine