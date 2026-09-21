#pragma once
#include "Shapes.h"

class Circle : public Shape {
private: int radius;
public:
	Circle(int id, const std::string& color, bool isFilled, int x, int y, int r)
		: Shape(id, color, isFilled, x, y, "circle"), radius(r) {
	}

	bool edit(const std::vector<int>& params) override;

	std::string getInfo() override;

	std::vector<std::pair<int, int>> calcDots() override;

	std::string serialization() override;

	std::unique_ptr<Shape> clone() override;
};
