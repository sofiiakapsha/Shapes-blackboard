#include "Circle.h"

bool Circle::edit(const std::vector<int>& params) {
	if (params.size() != 1) {
		std::cout << "error: invalid argument count\n";
		return false;
	}

	if (params[0] <= 0) {
		std::cout << "error: invalid parameters\n";
		return false;
	}

	radius = params[0];

	return true;
}

std::string Circle::getInfo() {
	return getName() + " " + getColor() + " " + getCoordinates() + " radius: " +
		std::to_string(radius);
}

std::vector<std::pair<int, int>> Circle::calcDots() {
	std::vector<std::pair<int, int>> dots;
	int cx = getX(), cy = getY();
	double ratio = 2.0;

	int rY = radius;
	int rX = static_cast<int>(radius * ratio);

	for (int py = cy - rY; py <= cy + rY; py++) {
		for (int px = cx - rX; px <= cx + rX; px++) {
			double dx = (px - cx) / ratio;
			double dy = py - cy;
			int distSquared = dx * dx + dy * dy;

			if (getIsFilled()) {
				if (distSquared <= radius * radius) {
					dots.push_back({ px, py });
				}
			}
			else {
				if (distSquared <= radius * radius && distSquared > (radius - 1) * (radius - 1)) {
					dots.push_back({ px, py });
				}
			}
		}
	}
	return dots;
}

std::string Circle::serialization() {
	std::string code = "C";
	return code + " " + std::to_string(getID()) + " " + std::to_string(getX()) + " " +
		std::to_string(getY()) + " " + getColor() + " " + std::to_string(getIsFilled()) + " " + std::to_string(radius);
}

std::unique_ptr<Shape> Circle::clone() {
	return std::make_unique<Circle>(getID(), getColor(), getIsFilled(), getX(), getY(), radius);
}