#include "Gameboard.h"
#include <iostream>
#include <windows.h>

//constructor
Gameboard::Gameboard() {
	grid[MAX_X][MAX_Y];
	spawnLoc;
	empty();
}

//----------------------------------public methods-----------------------------------
int Gameboard::getContent(Point p) const {
	return grid[p.getX()][p.getY()];
}

int Gameboard::getContent(int x, int y) const {
	return grid[x][y];
}

void Gameboard::setContent(Point pt, int content) {
	grid[pt.getX()][pt.getY()] = content;
}

void Gameboard::setContent(int x, int y, int content) {
	grid[x][y] = content;
}

void Gameboard::setContent(std::vector<Point> locs, int content) {
	for (auto &loc : locs) {
		setContent(loc, content);
	}
}
Point Gameboard::getSpawnLoc() const {
	return spawnLoc;
}
//checks if the list of points given is empty
bool Gameboard::areLocsEmpty(std::vector<Point> locs) const {
	//loop through locations
	for (auto& loc : locs) {
		//check if each location is empty
		if (getContent(loc.getX(),loc.getY()) != EMPTY_BLOCK) {
			return false;
		}
	}
	return true;
	
}
//removes any rows that are completed
int Gameboard::removeCompletedRows() {
	std::vector<int> rowsComplete = getCompletedRowIndices();
	removeRows(rowsComplete);
	//returns number of rows completed and removed
	return rowsComplete.size();
}
//empties the gameboard
void Gameboard::empty() {
	//loops through rows
	for (int i = 0; i < MAX_Y; i++)
	{
		fillRow(i, EMPTY_BLOCK);
	}
}
//prints the game board and its contents
void Gameboard::printToConsole() const {
	std::string toPrint = ". ";
	//loop through lines/rows/y values
	for (int y = 0; y < MAX_Y; y++)
	{
		setConsoleColor(-1);
		std::cout << "|";
		//loop through dots/columns/x values
		for (int x = 0; x < MAX_X; x++) {
			if (getContent(x,y) > EMPTY_BLOCK) {
				toPrint = "[_]";
			}
			else {
				toPrint = "   ";
			}
			//set this text color
			setConsoleColor(getContent(x,y));
			//print the character
			std::cout << toPrint;
		}
		//end of this line
		setConsoleColor(-1);
		std::cout << "|" << std::endl;
	}
	setConsoleColor(-1);
	std::cout << "--------------------------------" << std::endl;
}
//changes the console text color before a character is printed
void Gameboard::setConsoleColor(int color) const {
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	//red
	if (color == 0) {
		SetConsoleTextAttribute(hConsole, 4);
	}
	//orange
	else if (color == 1) {
		SetConsoleTextAttribute(hConsole, 12);
	}
	//yellow
	else if (color == 2) {
		SetConsoleTextAttribute(hConsole, 14);
	}
	//green
	else if (color == 3) {
		SetConsoleTextAttribute(hConsole, 10);
	}
	//blue light
	else if (color == 4) {
		SetConsoleTextAttribute(hConsole, 11);
	}
	//blue dark
	else if (color == 5) {
		SetConsoleTextAttribute(hConsole, 1);
	}
	//purple
	else if (color == 6) {
		SetConsoleTextAttribute(hConsole, 13);
	}
	else SetConsoleTextAttribute(hConsole, 15);
}

//----------------------------------------private methods-----------------------------------
//checks if a row is full
bool Gameboard::isRowCompleted(int rowIndex) const {
	//loops through given row
	for (int x = 0; x < MAX_X; x++) {
		//returns false if any EMPTY_BLOCKS are found
		if (getContent(x,rowIndex) == EMPTY_BLOCK) {
			return false;
		}
	}
	//otherwise all blocks were filled
	return true;
}
//returns a list of rows that are full
std::vector<int> Gameboard::getCompletedRowIndices() const {
	std::vector<int> completedRows;
	//loops through rows
	for (int i = 0; i < MAX_Y; i++) {
		//checks if the row is done
		if (isRowCompleted(i)) {
			completedRows.push_back(i);
		}
	}
	return completedRows;
}
//remove a single row and drop all rows above it down one
void Gameboard::removeRow(int rowIndex) {
	//loops through rows above given index
	for (int y = rowIndex; y >= 0; y--) {
			//all other rows cause row above to "fall"
			if (y > 0) {
				//copy row from above
				copyRowIntoRow(y-1, y);
			}
			//row zero set to empty
			else {
				//changes each to EMPTY_BLOCK
				fillRow(0, EMPTY_BLOCK);
			}
	}
}
//remove a list of rows
void Gameboard::removeRows(std::vector<int> rowIndices) {
	//loop through columns the given rows
	for (auto& row : rowIndices) {
		//empties the row
		removeRow(row);
	}
}
//fills the passed in row with content
void Gameboard::fillRow(int rowIndex, int content) {
	//loops through columns in given row
	for (int x = 0; x < MAX_X; x++) {
		//sets the column at row to contain given content
		setContent(x, rowIndex, content);
	}
}
//copies one row to another row
void Gameboard::copyRowIntoRow(int sourceRowIndex, int targetRowIndex) {
	//loops through columns in given rows
	for (int x = 0; x < MAX_X; x++) {
		//copies over the source row to target row
		setContent(x, targetRowIndex, getContent(x, sourceRowIndex));
	}
}