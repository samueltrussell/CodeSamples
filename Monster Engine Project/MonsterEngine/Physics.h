
#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>

#include "PhysicsInfo.h"
#include "Vector3.h"

namespace Engine
{
	namespace Physics
	{
		Vector3 * gravity;
		std::vector<PhysicsInfo *> * physicsObjects;
		
		void Initialize(Vector3 & i_gravity) 
		{
			gravity = new Vector3(i_gravity);
			physicsObjects = new std::vector<PhysicsInfo *>;
		}

		void ShutDown()
		{
			for (int i = 0; i < physicsObjects->size(); i++)
			{
				delete (*physicsObjects)[i];
			}

			delete physicsObjects;
			delete gravity;
		}

		bool AddPhysicsObject(SmartPointer<Actor> & i_Actor, const float i_mass, const float i_friction, const float i_drag, const float i_gravityScalar)
		{
			PhysicsInfo * newInfo = new PhysicsInfo(i_Actor, i_mass, i_friction, i_drag, i_gravityScalar);

			(*physicsObjects).push_back(newInfo);

			return true;
		}

		void ApplyPhysics(PhysicsInfo * i_object, double i_frameTime)
		{
			SmartPointer<Actor> actor = i_object->getActorPtr();
			//Sum up the forces on the object

			//gravity
			Vector3 force = *gravity * i_object->Mass() * i_object->GravityScalar();
			//drag
			force -= i_object->Friction() * (actor->velocity) + i_object->Drag() * (actor->velocity ^ 2.0f);
			//Applied Forces
			force += actor->externalForces;

			//Compute Acceleration for the frame
			// A = f/m
			Vector3 acceleration = force * (1.0f / i_object->Mass());

			//Update Velocity
			// V = V0 + a*T
			Vector3 velocity = actor->velocity + acceleration * (float)i_frameTime;

			//Update Position
			// P = P0 + V0*T + 1/2*a*T^2;
			Vector3 position = actor->position + actor->velocity * (float)i_frameTime + (0.5f * acceleration * (float)pow(i_frameTime, 2)); //(i_frameTime * i_frameTime));

			//Push updated position and velocity back into the Actor
			actor->position = position;
			actor->velocity = velocity;

		}

		void Update(const double i_frameTime)
		{
			for (int i = 0; i < (*physicsObjects).size(); i++)
			{
				ApplyPhysics((*physicsObjects)[i], i_frameTime);
			}
		}
		
	}//namespace Physics

}//namespace Engine

#endif//PHYSICS_H
