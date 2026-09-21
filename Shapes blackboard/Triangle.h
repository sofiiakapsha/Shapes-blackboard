#pragma once
#include "Shapes.h"

class Triangle : public Shape {
private:
	int height;
	int corner;
public:
	Triangle(int id, const std::string& color, bool isFilled, int x, int y, int h, int c)
		: Shape(id, color, isFilled, x, y, "triangle"), height(h), corner(c) {
	}

	bool edit(const std::vector<int>& params) override;
	std::string getInfo() override;

	std::vector<std::pair<int, int>> calcDots() override;

	std::string serialization() override;

	std::unique_ptr<Shape> clone() override;
};