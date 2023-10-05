#include "Pacman.h"

// Constructor
Pacman::Pacman()
	: GameObject(PrintingFigures::PACMAN)
{
	lives = MAX_LIVES;
}

// The following function decreases Pacman's lives by one.
void Pacman::updateLives()
{
	lives--;
}

// The following function returns pacman's current lives.
const int Pacman::getLives() const
{
	return lives;
}

// The following function checks if Pacman died.
int Pacman::isDead() const
{
	if (lives == DEAD)
		return 1;
	else
		return 0;
}

// The following function reinitializes the lives to be the max amount of lives.
void Pacman:: resetLives()
{
	lives = MAX_LIVES;
}
