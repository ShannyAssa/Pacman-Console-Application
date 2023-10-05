#pragma once
#include "Position.h"
#include "Board.h"
#include "Direction.h"
#include <ctime>
#include <cstdlib>

class GameObject
{
	Position startingPosition;
	Position pos;
	Color objColor;
	char figure;
	Direction direction = Direction::STAY;

public:
	GameObject(PrintingFigures _figure);

	//getter & setters
	const Position getStartingPosition() const;
	void setStartingPosition(Position startingPos);
	const Position getPosition() const;
	void updatePos(int x, int y);
	void updateFigure(char fig);
	const Direction getDirection() const; 
	void setDirection(const Direction _direction); 
	void setObjColor(Color _objColor);
	void updatePosByDirection();
	void changeDirectionRandomly();

	//drawing & colors
	inline void setColor()const;
	inline void setGrey()const;
	void draw()const;
	void erase(char fig) const;

	//others
	void reinitializePos();
	int isIntersecting(GameObject obj);
};

