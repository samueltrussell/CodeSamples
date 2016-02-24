//Class definition and member function declarations for IActorController Interface class

#ifndef IACTORCONTROLLER_H
#define IACTORCONTROLLER_H

class Actor;

class IActorController
{
public:
	virtual bool Move(Actor &) = 0;
	virtual void Print(const Actor &) = 0;
	virtual void updatePosition(Actor &) = 0;
};

#endif //IACTORCONTROLLER_H