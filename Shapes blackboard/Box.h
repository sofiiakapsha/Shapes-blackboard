#pragma once
#include "Shapes.h"

class Box : public Shape {
private:
	int side1, side2;
public:
	Box(int id, const std::string& color, bool isFilled, int x, int y, int s1, int s2)
		: Shape(id, color, isFilled, x, y, "box"), side1(s1), side2(s2) {
	}

	bool edit(const std::vector<int>& params) override;

	std::string getInfo() override;

	std::vector<std::pair<int, int>> calcDots() override;

	std::string serialization() override;

	std::unique_ptr<Shape> clone() override;
};