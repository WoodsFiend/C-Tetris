#include "GridTetramino.h"

GridTetramino::GridTetramino() {
	gridLoc.setXY(0,0);
}

Point GridTetramino::getGridLoc() const {
	return gridLoc;
}
void GridTetramino::setGridLoc(int x, int y)  {
	gridLoc.setXY(x, y);
}
void GridTetramino::setGridLoc(Point point) {
	gridLoc.setXY(point.getX(), point.getY());
}

void GridTetramino::move(int xOffset, int yOffset) {
	gridLoc.setXY(gridLoc.getX() + xOffset, gridLoc.getY() + yOffset);
}
std::vector<Point> GridTetramino::getBlockLocsMappedToGrid() const {
	std::vector<Point> shape;
	for (Point point : blockLocs) {
		point.setXY(point.getX() + gridLoc.getX(), point.getY() + gridLoc.getY());
		shape.push_back(point);
	}
	return shape;
}