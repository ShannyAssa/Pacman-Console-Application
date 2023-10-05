#include "Fruit.h"

// Constructor 
Fruit::Fruit()
	: GameObject(PrintingFigures::FIVE)
{
	setValue();
	updateFigure();
}

// The following function updates the fruit's figure accoring to its value
void Fruit::updateFigure() 
{
	char fig = 0;

	switch (value)
	{
	case 5:
		fig = static_cast<char>(PrintingFigures::FIVE);
		break;
	case 6:
		fig = static_cast<char>(PrintingFigures::SIX);
		break;
	case 7:
		fig = static_cast<char>(PrintingFigures::SEVEN);
		break;
	case 8:
		fig = static_cast<char>(PrintingFigures::EIGHT);
		break;
	case 9:
		fig = static_cast<char>(PrintingFigures::NINE);
		break;
	}

	GameObject::updateFigure(fig);
}

// The following function moves the fruit randomly.
void Fruit:: move() 
{
	if (showFruit)
	{
		updatePosByDirection();

		countSteps++;

		//if the fruit did "STEPS1" moves in the same direction/ the direstion is stay- change it
		if (countSteps % STEPS1 == 0 || getDirection() == Direction::STAY) 
			changeDirectionRandomly();
		
		if (countSteps % STEPS2 == 0)
		{
			setShowFruit(0); //make it disappear
			setValue();
		}
	}
	else
		setAppearanceRandomly();

}

// The following function sets randomly whether a fruit would appear on screen or not.
void Fruit:: setAppearanceRandomly()
{
	showFruit = rand() % APPEAR;
}

// The following function sets a random value to the fruit.
void Fruit::setValue()
{
	value = (rand() % RAND_VALUE) + RAND_VALUE;
	updateFigure();
	countSteps = 0;
}

// The following function manually sets whether a fruit would appear on screen or not.
void Fruit::setShowFruit(int _showFruit)
{
	showFruit = _showFruit;
}

// The following function returns a fruit's value.
const int Fruit::getValue()const
{
	return value;
}

// The following function returns whether a fruit should appear on the screen in the current moment.
const int Fruit::getShowFruit() const
{
	return showFruit;
}
