#include "Line.h"

bool Line::edit(const std::vector<int>& params) {
	if (params.size() != 2) {
		std::cout << "error: invalid argument count\n";
		return false;
	}
	if (params[0] <= 0 || params[1] < 0 || params[1] > 180) {
		std::cout << "error: invalid parameters\n";
		return false;
	}
	length = params[0];
	angle = params[1];
	return true;
}

std::string Line::getInfo() {
	return getName() + " " + getColor() + " " + getCoordinates() + " length and angle: " +
		std::to_string(length) + " " + std::to_string(angle);
}

std::vector<std::pair<int, int>> Line::calcDots() {
	std::vector<std::pair<int, int>> dots;
	int x0 = getX(), y0 = getY();

	double rad = angle * (std::numbers::pi / 180.0);
	double dx = std::cos(rad);
	double dy = std::sin(rad);

	for (int i = 0; i <= length; i++) {
		int px = x0 + static_cast<int>(std::round(dx * i));
		int py = y0 + static_cast<int>(std::round(dy * i));
		dots.push_back({ px, py });
	}
	return dots;
}

std::string Line::serialization() {
	std::string code = "L";
	return code + " " + std::to_string(getID()) + " " + std::to_string(getX()) + " " +
		std::to_string(getY()) + " " + getColor() + " " + std::to_string(getIsFilled()) +
		" " + std::to_string(length) + " " + std::to_string(angle);
}

std::unique_ptr<Shape> Line::clone() {
	return std::make_unique<Line>(getID(), getColor(), getIsFilled(), getX(), getY(), length, angle);
}