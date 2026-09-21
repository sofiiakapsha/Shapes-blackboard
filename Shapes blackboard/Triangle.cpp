#include "Triangle.h"

bool Triangle::edit(const std::vector<int>& params) {
	if (params.size() != 2) {
		std::cout << "error: invalid argument count\n";
		return false;
	}

	if (params[0] <= 0 || params[1] <= 0 || params[1] >= 180) {
		std::cout << "error: invalid parameters\n";
		return false;
	}

	height = params[0];
	corner = params[1];
	return true;
}

std::string Triangle::getInfo() {
	return getName() + " " + getColor() + " " + getCoordinates() + " height and corner: " +
		std::to_string(height) + " " + std::to_string(corner);
}

std::vector<std::pair<int, int>> Triangle::calcDots() {
	std::vector<std::pair<int, int>> dots;
	int x = getX(), y = getY();

	double halfAngleRad = (corner / 2.0) * (std::numbers::pi / 180.0);
	double tanHalfAngle = std::tan(halfAngleRad);

	for (int i = 0; i < height; i++) {
		int halfWidth = static_cast<int>(std::round(i * tanHalfAngle));
		int py = y - i;

		bool isLastRow = (i == height - 1);

		for (int dx = -halfWidth; dx <= halfWidth; dx++) {
			int px = x + dx;

			bool isBorder = isLastRow || (dx == -halfWidth) || (dx == halfWidth);

			if (getIsFilled() || isBorder) {
				dots.push_back({ px, py });
			}
		}
	}
	return dots;
}

std::string Triangle::serialization() {
	std::string code = "T";
	return code + " " + std::to_string(getID()) + " " + std::to_string(getX()) + " " +
		std::to_string(getY()) + " " + getColor() + " " + std::to_string(getIsFilled()) +
		" " + std::to_string(height) + " " + std::to_string(corner);
}

std::unique_ptr<Shape> Triangle::clone() {
	return std::make_unique<Triangle>(getID(), getColor(), getIsFilled(), getX(), getY(), height, corner);
}