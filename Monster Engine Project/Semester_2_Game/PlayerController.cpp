//Member function definitions for PlayerController Class

#include "PlayerController.h"

#include <conio.h>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

PlayerController::~PlayerController()
{

}

bool PlayerController::Move(Actor & i_Actor)
{
	
	return false;
}

void PlayerController::Print(const Actor & i_Actor)
{
//	printf("Actor: %s, is at x = %1.4f, y = %1.4f\n", i_Actor.name, i_Actor.position.GetX(), i_Actor.position.GetY());
}

void PlayerController::updatePosition(Actor & i_Actor)
{

}