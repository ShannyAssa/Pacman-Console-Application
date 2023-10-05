#pragma once
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>

//#include <algorithm>

#include "MatValue.h"
#include "PrintingFigures.h"
#include "Position.h"
#include "Error.h"

using namespace std;

class Board
{
	//consts
	static const int MAX_ROWS = 25;
	static const int MAX_COLS = 80;
	static const int MAX_GHOSTS = 4;
	static const int suffix_len = 7;

	enum readingFigures { FOODIES = ' ', BORDER = '#', SPACE = '%', PACMAN = '@', GHOST = '$', LEGEND = '&', ENDL = '\n' };
	enum LegendBoundaries { WIDTH = 20, HEIGHT = 3 };
	enum Errors { NO_PACMAN = 1, TOO_MANY_GHOSTS = 2, NO_FILE = 3, OUT_OF_LIMITS = 4 };

	//matrix
	vector<string> screen_files;
	MatValue matrix[MAX_ROWS][MAX_COLS];
	int actualRows = MAX_ROWS, actualCols = MAX_COLS;

	// regarding future objects in the game
	Position startingLegend;
	Position startingPacman;
	Position startingGhosts[4];
	int numOfGhosts = 0;

public:
	Board();

	//files & matrix handling
	void readFilesNames();
	void sortFileNames();
	int	bulidMaze(string mazeName);
	void errorsMessages(Error err);
	void fillWithBreadcrumbs(int row, int col, int maxCols);
	int ghostsValidation() const;
	int checkingScreenLimits(int& rows, int& cols) const;
	void updateLegendOnBoard();
	void printMaze() const;
	void printColoredMaze() const;

	// handling objects on the board
	const int countFoodies() const;
	const char getFig(MatValue matValue)const;

	// getters and setters
	const MatValue getValue(int rowY, int colX) const; // check if we need this!!
	void setValue(const int rowY, const int colX, const MatValue val);
	const int getROWS() const;
	const int getCOLS() const;
	const int getNumOfGhosts()const;
	const Position getStartingLegend()const;
	const Position getStartingPacman()const;
	const Position getStartingGhost(int index) const;
	const string getFileName(int index) const;
	const int getAmountOfFiles()const;
};

