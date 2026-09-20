#pragma once
#include <string>
#include <cstdbool>
#include <vector>
#include <iostream>
#include <utility>

class Shape {
private:
	int id;
	std::string color;
	std::string name;
	int x, y;
	bool isFilled;
	bool isSelected;
public:
	Shape(int id, const std::string& color, bool isFilled, int x, int y, const std::string& name)
		: id(id), color(color), name(name), isFilled(isFilled), x(x), y(y) {
		isSelected = false;
	}

	int getID() { return id; }
	std::string getColor() { return color; }
	void makeSelected() { isSelected = true; }
	bool isSelect() { return isSelected; }
	void changeColor(std::string newColor) { color = newColor; }
	std::string getCoordinates() { return std::to_string(x) + " " + std::to_string(y); }
	std::string getName() { return name; }
	virtual std::vector<std::pair<int, int>> calcDots() = 0;
	virtual std::string serialization() = 0;

	bool getIsFilled() { return isFilled; }


	void move(int newX, int newY) {
		x = newX;
		y = newY;
	}

	virtual bool edit(const std::vector<int>& params) = 0;
	virtual std::string getInfo() = 0;
	virtual ~Shape() = default;

	std::vector<std::pair<int, int>> getDots() {
		return calcDots();
	}

	int getX() { return x; }
	int getY() { return y; }

};

class Circle : public Shape {
private: int radius;
public:
	Circle(int id, const std::string& color, bool isFilled, int x, int y, int r)
		: Shape(id, color, isFilled, x, y, "circle"), radius(r) {
	}

	bool edit(const std::vector<int>& params) override {
		if (params.size() != 1) {
			std::cout << "error: invalid argument count\n";
			return false;
		}

		radius = params[0];

		return true;
	}

	std::string getInfo() override {
		return getName() + " " + getColor() + " " + getCoordinates() + " radius: " +
			std::to_string(radius);
	}

	std::vector<std::pair<int, int>> calcDots() override {
		std::vector<std::pair<int, int>> dots;
		int cx = getX(), cy = getY();
		int upY = cy + radius, dY = cy - radius;
		int rX = cx + radius, lX = cx - radius;

		for (int py = dY; py <= upY; py++) {
			for (int px = lX; px <= rX; px++) {
				int dx = px - cx;
				int dy = py - cy;
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

	std::string serialization() override {
		std::string code = "C";
		return code + " " + std::to_string(getID()) + " " + std::to_string(getX()) + " " +
			std::to_string(getY()) + " " + getColor() + " " + std::to_string(getIsFilled()) + " " + std::to_string(radius);
	}
};

class Triangle : public Shape {
private:
	int height;
public:
	Triangle(int id, const std::string& color, bool isFilled, int x, int y, int h)
		: Shape(id, color, isFilled, x, y, "triangle"), height(h) {
	}

	bool edit(const std::vector<int>& params) override {
		if (params.size() != 1) {
			std::cout << "error: invalid argument count\n";
			return false;
		}

		height = params[0];
		return true;
	}

	std::string getInfo() override {
		return getName() + " " + getColor() + " " + getCoordinates() + " height: " +
			std::to_string(height);
	}

	std::vector<std::pair<int, int>> calcDots() override {
		std::vector<std::pair<int, int>> dots;
		int x = getX(), y = getY();

		for (int i = 0; i < height; i++) {
			int numStars = 2 * i + 1;
			int leftMost = x - i;
			int py = y + i;

			for (int j = 0; j < numStars; j++) {
				int px = leftMost + j;

				bool isBorder = (i == height - 1) || (j == 0) || (j == numStars - 1);

				if (getIsFilled() || isBorder) {
					dots.push_back({ px, py });
				}
			}
		}
		return dots;
	}

	std::string serialization() override {
		std::string code = "T";
		return code + " " + std::to_string(getID()) + " " + std::to_string(getX()) + " " +
			std::to_string(getY()) + " " + getColor() + " " + std::to_string(getIsFilled()) +" " + std::to_string(height);
	}

};

class Box : public Shape {
private:
	int side1, side2;
public:
	Box(int id, const std::string& color, bool isFilled, int x, int y, int s1, int s2)
		: Shape(id, color, isFilled, x, y, "box"), side1(s1), side2(s2) {
	}

	bool edit(const std::vector<int>& params) override {
		if (params.size() != 2) {
			return false;
		}
		if (params[0] <= 0 || params[1] <= 0) {
			return false;
		}
		side1 = params[0];
		side2 = params[1];
		return true;
	}

	std::string getInfo() override {
		return getName() + " " + getColor() + " " + getCoordinates() + " sides: " +
			std::to_string(side1) + " " + std::to_string(side2);
	}

	std::vector<std::pair<int, int>> calcDots() override {
		std::vector<std::pair<int, int>> dots;
		int x = getX(), y = getY();

		for (int py = y; py < y + side2; py++) {
			for (int px = x; px < x + side1; px++) {
				bool isBorder = (py == y || py == y + side2 - 1 ||
					px == x || px == x + side1 - 1);

				if (getIsFilled() || isBorder) {
					dots.push_back({ px, py });
				}
			}
		}
		return dots;
	}

	std::string serialization() override {
		std::string code = "B";
		return code + " " + std::to_string(getID()) + " " + std::to_string(getX()) + " " +
			std::to_string(getY()) + " " + getColor() + " " + std::to_string(getIsFilled())
			+ " " + std::to_string(side1) + " " + std::to_string(side2);
	}
};