#pragma once
#include "GameObject.h"


class Fruit : public GameObject
{
	//consts
	static const int RAND_VALUE = 5;
	static const int STEPS1 = 5;
	static const int STEPS2 = 15;
	static const int APPEAR = 2;

	//variables
	int value;
	int countSteps = 0;
	int showFruit = 1;

public:
	Fruit();

	void updateFigure();
	void move();
	void setAppearanceRandomly();

	//getters & setters
	void setShowFruit(int _showFruit);
	void setValue();
	const int getValue()const;
	const int getShowFruit()const;
};

