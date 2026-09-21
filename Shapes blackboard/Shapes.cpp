#include "Shapes.h"

int Shape::getID() { return id; }
std::string Shape::getColor() { return color; }
void Shape::changeColor(std::string newColor) { color = newColor; }
std::string Shape::getCoordinates() { return std::to_string(x) + " " + std::to_string(y); }
std::string Shape::getName() { return name; }

bool Shape::getIsFilled() { return isFilled; }


void Shape::move(int newX, int newY) {
	x = newX;
	y = newY;
}

std::vector<std::pair<int, int>> Shape::getDots() {
	return calcDots();
}

int Shape::getX() { return x; }
int Shape::getY() { return y; }