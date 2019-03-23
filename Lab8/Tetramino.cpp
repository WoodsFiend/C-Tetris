#include "Tetramino.h"


using namespace std;

Tetramino::Tetramino() {
	shape = TetShape(0);
	color = RED;
	blockLocs = { Point(0,0),Point(0,0),Point(0,0),Point(0,0) };
	setShape(shape);
}

//----------------------------------public methods----------------------------
TetColor Tetramino::getColor() {
	return color;
}

TetShape Tetramino::getShape() {
	return shape;
}

// set the shape
//  -clear any blockLocs set previously
//  -set the blockLocs for the shape
void Tetramino::setShape(TetShape shape) {
	this->shape = shape;
	blockLocs.clear();
	if (shape == SHAPE_O) {
		blockLocs.push_back(Point(0, 1));
		blockLocs.push_back(Point(1, 1));
		blockLocs.push_back(Point(0, 0));
		blockLocs.push_back(Point(1, 0));
		color = YELLOW;
	}
	else if (shape == SHAPE_I) {
		blockLocs.push_back(Point(0, -1));
		blockLocs.push_back(Point(0, 0));
		blockLocs.push_back(Point(0, 1));
		blockLocs.push_back(Point(0, 2));
		color = BLUE_LIGHT;
	}
	else if (shape == SHAPE_S) {
		blockLocs.push_back(Point(-1, 0));
		blockLocs.push_back(Point(0, 0));
		blockLocs.push_back(Point(0, 1));
		blockLocs.push_back(Point(1, 1));
		color = RED;
	}
	else if (shape == SHAPE_Z) {
		blockLocs.push_back(Point(-1, 1));
		blockLocs.push_back(Point(0, 1));
		blockLocs.push_back(Point(0, 0));
		blockLocs.push_back(Point(1, 0));
		color = GREEN;
	}
	else if (shape == SHAPE_L) {
		blockLocs.push_back(Point(0, 1));
		blockLocs.push_back(Point(0, 0));
		blockLocs.push_back(Point(0, -1));
		blockLocs.push_back(Point(1, -1));
		color = ORANGE;
	}
	else if (shape == SHAPE_J) {
		blockLocs.push_back(Point(-1, -1));
		blockLocs.push_back(Point(0, -1));
		blockLocs.push_back(Point(0, 0));
		blockLocs.push_back(Point(0, 1));
		color = BLUE_DARK;
	}
	else if (shape == SHAPE_T) {
		blockLocs.push_back(Point(-1, 0));
		blockLocs.push_back(Point(0, 0));
		blockLocs.push_back(Point(1, 0));
		blockLocs.push_back(Point(0, -1));
		color = PURPLE;
	}
}
//turn the object clock wise
void Tetramino::rotateCW() {
	//loop through all values in the blockLoc
	for (int i = 0; i < blockLocs.size(); i++)
	{
		blockLocs[i].swapXY();
		blockLocs[i].multiplyY(-1);
	}
}
//print the object to console
void Tetramino::printToConsole() {
	std::string toPrint = ". ";
	setConsoleColor();
	//loop through lines/rows/y values
	for (int y = 3; y >= -3; y--)
	{
		//loop through dots/columns/x values
		for (int x = -3; x <= 3; x++) {
			for(auto &point : blockLocs) {
				//if x and y are same as current position
				if (point.getX() == x && point.getY() == y) {
					//set value to be printed to x
					toPrint = "x ";
					break;
				}
				//no point at current position
				else
				{
					toPrint = ". ";
				}
			}
			std::cout << toPrint;
		}
		//end of this line
		std::cout << std::endl;
	}
}
//every draw it tracks the last draw and only adds 1 of it to the vector
//other shapes have 3 of each resulting in:
//chance to draw same shape is 5.2%
//chance to draw each other shape is 15.8%
TetShape Tetramino::getRandomShape() {
	static int lastDraw = -1;
	std::vector<int> random;
	//3 of each shape
	for (int i = 0; i < 21; i++) {
		//stops all last draw shapes from being added to bag
		if (i % 7 != lastDraw) {
			random.push_back(i % 7);
		}
	}
	//adds only one of last draw shapes to the vector 5% chance
	if (lastDraw != -1) {
		random.push_back(lastDraw);
	}
	//random generator
	static auto rng = std::default_random_engine((unsigned)time(NULL));
	//shuffle vector
	std::shuffle(std::begin(random), std::end(random), rng);
	//debug
	/*for (int num : random) {
		std::cout << num << ", ";
	}
	std::cout << std::endl;/**/
	int randomNum = random[random.size()-1];
	lastDraw = randomNum;
	random.pop_back();
	return TetShape(randomNum);
}
//change the console colors
void Tetramino::setConsoleColor() {
	if (getColor() == RED) {
		system("Color 04");
	}
	else if (getColor() == GREEN) {
		system("Color 0A");
	}
	else if (getColor() == ORANGE) {
		system("Color 0C");
	}
	else if (getColor() == BLUE_DARK) {
		system("Color 01");
	}
	else if (getColor() == YELLOW) {
		system("Color 0E");
	}
	else if (getColor() == BLUE_LIGHT) {
		system("Color 09");
	}
	else if (getColor() == PURPLE) {
		system("Color 0D");
	}
	else system("Color  ");
}