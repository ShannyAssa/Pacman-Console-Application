#include "SmartMovement.h"

// Constructor
SmartMovement::SmartMovement()
{
}

// The following function copies the board to "VisitedMat" so we can keep track of it.
void SmartMovement::copyBoard(Board& b)
{
	actualCols = b.getCOLS();
	actualRows = b.getROWS();

	for(int i=0;i<actualRows;i++)
		for (int j = 0;j < actualCols;j++)
			visitedMat[i][j] = b.getValue(i, j);
}

int SmartMovement::checkNextVisitedMatValue(Direction dir, int x, int y)
{
	MatValue matValue = MatValue::VISITED;
	Position p(x, y);

	switch (dir)
	{
	case Direction::UP:
		p.updatePosition(x, y - 1);
		break;
	case Direction::DOWN:
		p.updatePosition(x, y + 1);
		break;
	case Direction::LEFT:
		p.updatePosition(x - 1, y);
		break;
	case Direction::RIGHT:
		p.updatePosition(x + 1, y);
		break;
	}

	if (isPosValid(p))
		matValue = visitedMat[p.getY()][p.getX()];
	else
		return 0; // if this position is not valid then we cannot do further checking
	
	if (matValue != MatValue::VISITED && matValue != MatValue::WALL)
		return 1;

	return 0;
}

// The following function uses bfs algorithm to return the minimal path length from the inital position to the destination. 
int SmartMovement::bfs(Position from, Position to, Board& b, int LastMin)
{
	QNode source;
	source.curr = from;
	source.distance = 0;
	copyBoard(b);

	// applying BFS on matrix cells starting from source
	queue<QNode> toVisit;
	toVisit.push(source);
	visitedMat[source.curr.getY()][source.curr.getX()] = MatValue::VISITED;

	while (!toVisit.empty()) {
		QNode temp = toVisit.front();
		toVisit.pop();

		// Destination found;
		if (temp.curr.isEqual(to)) 
			return temp.distance;
		
		// Otherwise:
		QNode toPush;
		if (checkNextVisitedMatValue(Direction::UP, temp.curr.getX(), temp.curr.getY()))
		{
			toPush.curr = FurtherChecking(Direction::UP, temp.curr);
			toPush.distance = temp.distance + 1;
			toVisit.push(toPush);
			visitedMat[toPush.curr.getY()][toPush.curr.getX()] = MatValue::VISITED;

		}
		if (checkNextVisitedMatValue(Direction::DOWN, temp.curr.getX(), temp.curr.getY()))
		{
			toPush.curr = FurtherChecking(Direction::DOWN, temp.curr);
			toPush.distance = temp.distance + 1;
			toVisit.push(toPush);
			visitedMat[toPush.curr.getY()][toPush.curr.getX()] = MatValue::VISITED;

		}
		if (checkNextVisitedMatValue(Direction::LEFT, temp.curr.getX(), temp.curr.getY()))
		{
			toPush.curr = FurtherChecking(Direction::LEFT, temp.curr);
			toPush.distance = temp.distance + 1;
			toVisit.push(toPush);
			visitedMat[toPush.curr.getY()][toPush.curr.getX()] = MatValue::VISITED;

		}
		if (checkNextVisitedMatValue(Direction::RIGHT, temp.curr.getX(), temp.curr.getY()))
		{
			toPush.curr = FurtherChecking(Direction::RIGHT, temp.curr);
			toPush.distance = temp.distance + 1;
			toVisit.push(toPush);
			visitedMat[toPush.curr.getY()][toPush.curr.getX()] = MatValue::VISITED;

		}
	}
	
	return LastMin; //if no way is found return the last minimum value
}

// The following function calculates (using bfs) 4 directions' minimal length ane returns the 
// one direction which will require the minimal further steps towards the destination.
Direction SmartMovement::SmartGhost(Position source, Position dest, Board& b)
{
	// the first MinDistance would always be smaller than all the board's squares number
	int MinDistance = bfs(source, dest, b, b.getCOLS()*b.getROWS());
	int currentBFS = 0;

	Direction NextDir = Direction::UP;
	if (!isWall(b, source.getX(), source.getY() - 1) && !isEdge(source.getX(), source.getY(),Direction::UP))
	{
		currentBFS = bfs(FurtherChecking(Direction::UP, source), dest, b, MinDistance);
		if (currentBFS < MinDistance)
		{
			NextDir = Direction::UP;
			MinDistance = currentBFS;
		}
	}
	if (!isWall(b, source.getX(), source.getY() + 1) && !isEdge(source.getX(), source.getY(), Direction::DOWN))
	{
		currentBFS = bfs(FurtherChecking(Direction::DOWN, source), dest, b, MinDistance);
		if (currentBFS < MinDistance)
		{
			NextDir = Direction::DOWN;
			MinDistance = currentBFS;
		}
	}
	if (!isWall(b, source.getX() - 1, source.getY()) && !isEdge(source.getX(), source.getY(), Direction::LEFT))
	{
		currentBFS = bfs(FurtherChecking(Direction::LEFT, source), dest, b, MinDistance);
		if (currentBFS < MinDistance)
		{
			NextDir = Direction::LEFT;
			MinDistance = currentBFS;
		}
	}
	if (!isWall(b, source.getX() + 1, source.getY()) && !isEdge(source.getX(), source.getY(), Direction::RIGHT))
	{
		currentBFS = bfs(FurtherChecking(Direction::RIGHT, source), dest, b, MinDistance);
		if (currentBFS < MinDistance)
		{
			NextDir = Direction::RIGHT;
			MinDistance = currentBFS;
		}
	}
	return NextDir;
}

Position SmartMovement::FurtherChecking(Direction dir, Position curr)
{
	switch (dir)
	{
	case Direction::UP:
		curr.updatePosition(curr.getX(), curr.getY() - 1);
		break;
	case Direction::DOWN:
		curr.updatePosition(curr.getX(), curr.getY() + 1);
		break;
	case Direction::LEFT:
		curr.updatePosition(curr.getX() - 1, curr.getY());
		break;
	case Direction::RIGHT:
		curr.updatePosition(curr.getX() + 1, curr.getY());
		break;
	}
	
	return curr;
}

// The following function checks whether a position is within the board's limits or not.
int SmartMovement:: isPosValid(Position p) const
{
	if (p.getX() < actualCols && p.getY() < actualRows && p.getX() >= 0 && p.getY() >= 0)
		return 1;

	return 0;
}

// The following function checks whether a certain spot is a wall.
int SmartMovement::isWall(Board& b, int x, int y) const
{
	if (b.getValue(y, x) == MatValue::WALL)
		return 1;
	return 0;
}

// The following function checks whether a certain spot is an edge (not a wall).
int SmartMovement::isEdge(int x, int y, Direction dir) const
{
	if (dir == Direction::UP && y == 0)
		return 1;
	if (dir == Direction::DOWN && y == actualRows - 1)
		return 1;
	if (dir == Direction::LEFT && x == 0)
		return 1;
	if (dir == Direction::RIGHT && x == actualCols - 1)
		return 1;
	
	return 0;
}