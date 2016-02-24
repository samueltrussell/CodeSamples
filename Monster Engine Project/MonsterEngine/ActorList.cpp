//Member Function Definitions for ActorList class

//DEBUG TOOLS
#include "DebugLog.h"

#include "ActorList.h"
#include "Allocators.h"
#include "FixedBlockAllocator.h"
#include "HeapAllocator.h"
#include <iostream>

ActorList::ActorList(Engine::FixedBlockAllocator * i_pHeap) : 
	pHeap(i_pHeap)
{
	numberOfActors = 0;
	numberOfActorsCreated = 0;
	firstActorPtr = NULL;
}

ActorList::~ActorList()
{
	//COMMENT OUT BETWEEN THESE NOTES TO SEE LEAK DUMP IN OUTPUT AFTER TERMINATION
	//Actor * pActor = firstActorPtr;
	//while (pActor != NULL){
	//	Actor * temp = pActor;
	//	pActor = pActor->NextActor;
	//	delete temp;
	//}
	//COMMENT OUT BETWEEN THESE NOTES TO SEE LEAK DUMP IN OUTPUT AFTER TERMINATION
	ClearList();
	//DEBUG_LOG("ActorList Deconstructed\n");
}

void ActorList::Add(IActorController * i_controller){

	float position[2] = { rand() % 100, rand() % 100 };
	numberOfActorsCreated++;

	const size_t lenTempName = 16;

	char TempName[lenTempName];

	sprintf_s(TempName, sizeof(TempName), "Monster %i", numberOfActorsCreated);

	Actor * newActorPtr = new (pHeap) Actor(position, TempName);

	newActorPtr->controller = i_controller;

	if (numberOfActors == 0){
		firstActorPtr = lastActorPtr = newActorPtr;
	}
	else{
		lastActorPtr->NextActor = newActorPtr;
		lastActorPtr = newActorPtr;
	}
	numberOfActors++;
}

void ActorList::PrintActors(){

	currentActorPtr = firstActorPtr;
	
	while(currentActorPtr != NULL){
		currentActorPtr->controller->Print(*currentActorPtr);
		currentActorPtr = currentActorPtr->NextActor;
	}

	printf("\n");
}

void ActorList::MoveActors(){
	
	currentActorPtr = firstActorPtr;

	while (currentActorPtr != NULL){
		currentActorPtr->controller->Move(*currentActorPtr);
		currentActorPtr = currentActorPtr->NextActor;
	}
}

void ActorList::CollideActors(Vector3 const playerPositionVector){
	currentActorPtr = firstActorPtr;
	Actor * checkActorPtr;
	Actor * tempPtr;

	//Check for Actor-Player Collisions
	while (currentActorPtr != NULL){

		//Debug: force all the monsters to be hit by the player
		//currentActorPtr->position = playerPositionVector;

		if (currentActorPtr->position == playerPositionVector){

			tempPtr = currentActorPtr->NextActor; //if the player hits a monster, kill it
			Remove(currentActorPtr);
			currentActorPtr = tempPtr;
		}
		else{
			currentActorPtr = currentActorPtr->NextActor;
		}
	}

	//Check for Actor-Actor Collisions
	currentActorPtr = firstActorPtr;
	while (currentActorPtr != NULL){
		checkActorPtr = currentActorPtr->NextActor;

		while (checkActorPtr != NULL){

			if (currentActorPtr->position == checkActorPtr->position){

				Add(currentActorPtr->controller); //if two monsters collide, add a new one
			}
			
			checkActorPtr = checkActorPtr->NextActor;
			
			
		}

		currentActorPtr = currentActorPtr->NextActor;
	}
}

void ActorList::Remove(Actor * removeActorPtr){
	
	if (removeActorPtr == firstActorPtr){ //First Actor?
		
		firstActorPtr = removeActorPtr->NextActor;
		removeActorPtr->~Actor();
		operator delete(removeActorPtr, pHeap);
	}
	else if (firstActorPtr == lastActorPtr){ //Last Actor?
		int test = 0;
	}
	else{ //Any other Actor
		
		//obtain previous monster pointer // Last Actor?
		currentActorPtr = firstActorPtr;
		while (currentActorPtr->NextActor != removeActorPtr){
			currentActorPtr = currentActorPtr->NextActor;
		}

		if (removeActorPtr == lastActorPtr){
			lastActorPtr = currentActorPtr;
		}

		currentActorPtr->NextActor = removeActorPtr->NextActor;
		currentActorPtr->~Actor();
		operator delete (removeActorPtr, pHeap);
	}

}

Actor * ActorList::NextActor(){
	return currentActorPtr->NextActor;
}

bool ActorList::ClearList()
{
	Actor * pActor = firstActorPtr;
	while (pActor != NULL) {
		Actor * temp = pActor;
		pActor = pActor->NextActor;
		Remove(temp);
	}

	return true;
}

