#pragma once
#include <iostream>
#include "gotoxy.h"
#include "Color.h"

using namespace std;

class Position {

	int x;
	int y;

public:

	Position(int new_x = 0, int new_y = 0);
	void updatePosition(int new_x, int new_y);
	const int getX() const;
	const int getY() const;
	void draw(const char fig) const;
	int isEqual(Position p2) const;
};