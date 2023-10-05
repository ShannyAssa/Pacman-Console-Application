#include "GameObject.h"

GameObject::GameObject(PrintingFigures _figure)
{
	objColor = Color::GREY;
	figure = (char)_figure; 
}

const Position GameObject::getStartingPosition() const
{
	return startingPosition;
}

void GameObject::setStartingPosition(Position startingPos)
{
	startingPosition.updatePosition(startingPos.getX(), startingPos.getY());
	pos.updatePosition(startingPos.getX(), startingPos.getY());
}

// The following function returns Pacman's current position.
const Position GameObject::getPosition() const
{
	return pos;
}

// The following functions updates Pacman's position
void GameObject::updatePos(int x, int y)
{
	pos.updatePosition(x, y);
}

// The following function updates a figure. 
void GameObject::updateFigure(char fig)
{
	figure = fig;
}

// The following function updates the obj's position to be the "next move" towards its current direction.
void GameObject::updatePosByDirection()
{
	switch (direction) // for STAY we don't need to update the position
	{
	case Direction::UP:
		updatePos(pos.getX(),pos.getY() - 1);
		break;
	case Direction::DOWN:
		updatePos(pos.getX(), pos.getY() + 1);
		break;
	case Direction::LEFT:
		updatePos(pos.getX() - 1, pos.getY());
		break;
	case Direction::RIGHT:
		updatePos(pos.getX() + 1, pos.getY());
		break;
	}
}

// The following function changes the object's direction randomly.
void GameObject::changeDirectionRandomly()
{
	int dir = rand() % static_cast<int>(Direction::DOWN) + static_cast<int>(Direction::LEFT);

	switch (dir)
	{
	case static_cast<int>(Direction::LEFT):
		direction=Direction::LEFT;
		break;

	case static_cast<int>(Direction::RIGHT):
		direction = Direction::RIGHT;
		break;

	case static_cast<int>(Direction::UP):
		direction = Direction::UP;
		break;

	case static_cast<int>(Direction::DOWN):
		direction = Direction::DOWN;
		break;
	}
}

// The following function returns the object's current direction.
const Direction GameObject::getDirection() const
{
	return direction;
}

// The following function sets the object's direction.
void GameObject::setDirection(const Direction _direction)
{
	direction = _direction;
}

// The following function sets the object's color.
void GameObject:: setObjColor(Color _objColor)
{
	objColor = _objColor;
}

// The following function draws game object in its current position.
void GameObject::draw() const
{
	if (objColor != Color::GREY)
		setColor();

	pos.draw(figure);

	if (objColor != Color::GREY)
		setGrey();
}

// The following function erases the game object from the screen, drawing fig instead.
void GameObject::erase(char fig) const
{
	pos.draw(fig);
}

// The following function gets the game object to its initial position.
void GameObject::reinitializePos()
{
	updatePos(startingPosition.getX(), startingPosition.getY());
}

// The following function sets printing color to be the object's current color.
inline void GameObject::setColor() const
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (int)objColor);
}

// The following function sets printing color to be grey.
inline void GameObject::setGrey() const
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (int)Color::GREY);
}

// The following function checks whether another game object intersects with this current one. (positions intersection)
int GameObject::isIntersecting(GameObject obj)
{
	if (pos.isEqual(obj.getPosition()))
		return 1;

	return 0;
}