#include "Ghost.h"

// Constructor
Ghost::Ghost()
	: GameObject(PrintingFigures::GHOST)
{
}

// The following function erases ghosts from the screen and prints fig instead.
void Ghost::erase(char fig)const
{
	GameObject::erase(fig);
}

// The following function moves a ghost according to the game level.
void Ghost::move(Board& b, Position dest)
{
	switch (movementLevel)
	{
	case Level::NOVICE:
		moveRandomly();
		break;
	case Level::GOOD :
		if (countSteps <= STEPS1)
		{
			setDirection(SmartGhost(getPosition(),dest, b));
			updatePosByDirection();
		}
		else
		{
			moveRandomly();
			if (countSteps == STEPS2)
				countSteps = 0;
		}
		break;
	case Level::BEST:
		if (countSteps % STEPS3 == 0) //meant to seperate the ghosts routes in case they merged
		{
			moveRandomly();
			countSteps = 0;
		}
		else
		{
			setDirection(SmartGhost(getPosition(), dest, b));
			updatePosByDirection();
		}
		
		break;
		
	}
	countSteps++;
}

 //The following function moves a ghost randomly.
void Ghost:: moveRandomly() 
{

	updatePosByDirection();

	//if the ghost did "STEPS"(number) steps in the same direction or bumped into a wall we change the direction
	if (countSteps % STEPS1 == 0 || getDirection() == Direction::STAY) 
	{
		changeDirectionRandomly();
		countSteps = 0;
	}
}

// The following function sets the game's level so a ghost would be moving accordingly.
void Ghost::setLevel(Level _lev)
{
	movementLevel = _lev;
}

