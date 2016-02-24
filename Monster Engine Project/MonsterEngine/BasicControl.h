
#ifndef BASIC_CONTROL_H
#define BASIC_CONTROL_H

#include "Actor.h"
#include "SmartPointer.h"
#include "Vector3.h"

namespace Engine
{
	namespace Control
	{
		float magnitude;
		SmartPointer<Actor> pControlledActor;

		void RegisterPawn(SmartPointer<Actor> & i_pActor, float i_magnitudeOfForce)
		{
			pControlledActor = i_pActor;
			magnitude = i_magnitudeOfForce;

		}

		void UnRegisterPawn()
		{
			pControlledActor = SmartPointer<Actor>();
		}

		void ShutDown()
		{
			UnRegisterPawn();
		}

		void ApplyForce(SmartPointer<Actor> & i_pActor, const Vector3 & i_force)
		{
			i_pActor->externalForces += i_force;
		}

		void KeyEventCallback(unsigned int i_VKeyID, bool bWentDown)
		{
			Vector3 unitVector;

			switch (i_VKeyID) {

			case(0x41) :
				//thrust left
				unitVector.SetParameters(-1.0, 0.0, 0.0);
				break;
			case(0x57) :
				//thrust up
				unitVector.SetParameters(0.0, 1.0, 0.0);
				break;
			case(0x44) :
				//thrust right
				unitVector.SetParameters(1.0, 0.0, 0.0);
				break;
			case(0x53) :
				//thrust down
				unitVector.SetParameters(0.0, -1.0, 0.0);
				break;
			default:
				break;
			}

			float mag;

			if (bWentDown) {
				mag = magnitude;
			}
			else{
				mag = -magnitude;
			}

			ApplyForce(pControlledActor, unitVector*mag);
			
		}

	}

}

#endif //BASIC_CONTROL_H
