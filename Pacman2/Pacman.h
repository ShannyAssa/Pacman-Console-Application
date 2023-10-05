#pragma once
#include "GameObject.h"

class Pacman : public GameObject
{
	int lives;

	//consts
	static const int MAX_LIVES = 3;
	static const int DEAD = 0;

public:
	Pacman();
	const int getLives() const;
	void updateLives();
	int isDead() const;
	void resetLives();
};