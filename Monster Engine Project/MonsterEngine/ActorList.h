////Header file declaring the interface for the ActorList class
#include "Actor.h"
#include "FixedBlockAllocator.h"
#include "HeapAllocator.h"
#include "IActorController.h"
#include "Vector3.h"

#ifndef ACTORLIST_H
#define ACTORLIST_H

class ActorList
{
public:
	ActorList(Engine::FixedBlockAllocator * i_pHeap);
	~ActorList();
	void Add(IActorController * i_controller);
	void PrintActors();
	void MoveActors();
	void CollideActors(Vector3);
	void Remove(Actor *);
	bool ClearList();
	Actor * NextActor();
private:
	Actor * firstActorPtr;
	Actor * lastActorPtr;
	Actor * currentActorPtr;
	Engine::FixedBlockAllocator * pHeap;
	int numberOfActors;
	int numberOfActorsCreated;
};


#endif //ACTORLIST_H