#ifndef TETRAMINO_H
#define TETRAMINO_H
#include <vector>
#include "Point.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>


//Enumerations
enum TetColor {
	RED,
	ORANGE,
	YELLOW,
	GREEN,
	BLUE_LIGHT,
	BLUE_DARK,
	PURPLE
};
enum TetShape {
	SHAPE_S,
	SHAPE_Z,
	SHAPE_L,
	SHAPE_J,
	SHAPE_O,
	SHAPE_I,
	SHAPE_T
};


class Tetramino
{
	//so that TestSuite has access to private methods of vars
	friend class TestSuite;

public:
	//constructor
	Tetramino();
	//return the color
	TetColor getColor();
	//return the shape
	TetShape getShape();
	//change the shape and associated color
	void setShape(TetShape shape);
	//rotate the object
	void rotateCW();
	//print the shape of the object to console
	void printToConsole();
	//hold the 4 points of the object
	std::vector<Point> blockLocs;
	//set Console Color
	void setConsoleColor();
	static TetShape getRandomShape();

private:
	TetColor color;
	TetShape shape;
	
};
#endif