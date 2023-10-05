#include "Position.h"

// Constructor
Position::Position(int new_x, int new_y)
{
	updatePosition(new_x, new_y);
}

// The following function updates a position according to x and y.
void Position::updatePosition(int new_x, int new_y)
{
	x = new_x;
	y = new_y;
}

// The following function returns X value.
const int Position::getX() const
{
	return x;
}

// The following function returns Y value.
const int Position::getY() const
{
	return y;
}

// The following function draws fig in the current position.
void Position::draw(char fig) const
{
	gotoxy(x, y);
	cout << fig;
}

// The following function checks whether another position is equal to this one.
int Position:: isEqual(Position p2) const
{
	if (x == p2.getX() && y == p2.getY())
		return 1;

	return 0;
}
