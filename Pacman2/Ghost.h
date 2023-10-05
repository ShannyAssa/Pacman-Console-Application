#pragma once
#include "GameObject.h"
#include "Level.h"
#include "SmartMovement.h"


class Ghost : public GameObject , public SmartMovement
{
	//consts
	static const int STEPS1 = 20;
	static const int STEPS2 = 60;
	static const int STEPS3 = 10;

	//variables
	Level movementLevel=Level::NOVICE;
	int countSteps = 0, stay=0;

public:
	Ghost();
	void erase(char fig)const;
	void move(Board& b, Position dest);
	void moveRandomly();
	void setLevel(Level _lev);
};

