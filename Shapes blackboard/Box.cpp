#include "Box.h"

bool Box::edit(const std::vector<int>& params) {
	if (params.size() != 2) {
		std::cout << "error: incorrect number of parameters\n";
		return false;
	}
	if (params[0] <= 0 || params[1] <= 0) {
		std::cout << "error: incorrect parameters\n";
		return false;
	}
	side1 = params[0];
	side2 = params[1];
	return true;
}

std::string Box::getInfo() {
	return getName() + " " + getColor() + " " + getCoordinates() + " sides: " +
		std::to_string(side1) + " " + std::to_string(side2);
}

std::vector<std::pair<int, int>> Box::calcDots() {
	std::vector<std::pair<int, int>> dots;
	int x = getX(), y = getY();
	double ratio = 2.0;

	int width = static_cast<int>(side1 * ratio);
	int height = side2;

	for (int py = y; py < y + height; py++) {
		for (int px = x; px < x + width; px++) {
			bool isBorder = (py == y || py == y + height - 1 ||
				px == x || px == x + width - 1);

			if (getIsFilled() || isBorder) {
				dots.push_back({ px, py });
			}
		}
	}
	return dots;
}

std::string Box::serialization() {
	std::string code = "B";
	return code + " " + std::to_string(getID()) + " " + std::to_string(getX()) + " " +
		std::to_string(getY()) + " " + getColor() + " " + std::to_string(getIsFilled())
		+ " " + std::to_string(side1) + " " + std::to_string(side2);
}

std::unique_ptr<Shape> Box::clone() {
	return std::make_unique<Box>(getID(), getColor(), getIsFilled(), getX(), getY(), side1, side2);
}