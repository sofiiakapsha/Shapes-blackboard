#pragma once
#include "Shapes.h"

class Line : public Shape {
private:
	int length;
	int angle;

public:
	Line(int id, const std::string& color, bool isFilled, int x, int y, int len, int ang)
		: Shape(id, color, isFilled, x, y, "line"), length(len), angle(ang) {
	}

	bool edit(const std::vector<int>& params) override;

	std::string getInfo() override;

	std::vector<std::pair<int, int>> calcDots() override;

	std::string serialization() override;

	std::unique_ptr<Shape> clone() override;
};