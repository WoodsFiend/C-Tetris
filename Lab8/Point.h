#ifndef POINT_H
#define POINT_H
#include <string>
class Point
{
	friend class TestSuite;
public:
	//constructor
	Point();
	//constructor with passable x and y points
	Point(int newX, int newY);
	//returns the x value
	int getX() const;
	//returns y value
	int getY() const;
	//changes the x value
	void setX(int x);
	//changes the y value
	void setY(int y);
	//changes both values
	void setXY(int x, int y);
	//swap the values
	void swapXY();
	//multipy x by a factor
	void multiplyX(int factor);
	//multiply y by a factor
	void multiplyY(int factor);
	//return the values as a string
	std::string toString() const;
private:
	int x;
	int y;
};
#endif


