#pragma once
#include "Board.h"
#include <queue>
#include <list>
#include "Direction.h"

class SmartMovement 
{
	struct QNode
	{
		Position curr;
		int distance;
	};

	//consts
	static const int MAX_ROWS = 25;
	static const int MAX_COLS = 80;

	//variables
	int actualCols = 0, actualRows = 0;
	MatValue visitedMat[MAX_ROWS][MAX_COLS];

public:

	SmartMovement();
	
	//handling smart ghosts algorithm
	int bfs(Position from, Position to, Board& b, int LastMin);
	Position FurtherChecking(Direction dir, Position curr);
	Direction SmartGhost(Position source, Position dest, Board& b);
	void copyBoard(Board& b);
	int checkNextVisitedMatValue(Direction dir, int x, int y);

	//checking validation
	int isEdge(int x, int y, Direction dir) const;
	int isPosValid(Position p) const;
	int isWall(Board& b, int x, int y) const;
};

