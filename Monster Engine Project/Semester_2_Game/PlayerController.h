//Class declaration for Player Controller class

#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "Actor.h"
#include "IActorController.h"

class PlayerController : public IActorController
{
public:
	~PlayerController();
	bool Move(Actor &);
	void Print(const Actor &);
	void updatePosition(Actor &);
private:

};

#endif //PLAYERCONTROLLER_H