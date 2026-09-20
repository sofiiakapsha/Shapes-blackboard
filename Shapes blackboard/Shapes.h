#pragma once
#include <string>
#include <cstdbool>
#include <vector>
#include <iostream>
#include <utility>
#include <cmath>
#include <numbers>

class Shape {
private:
	int id;
	std::string color;
	std::string name;
	int x, y;
	bool isFilled;
public:
	Shape(int id, const std::string& color, bool isFilled, int x, int y, const std::string& name)
		: id(id), color(color), name(name), isFilled(isFilled), x(x), y(y) {
	}

	int getID() { return id; }
	std::string getColor() { return color; }
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

	std::string serialization() override {
		std::string code = "C";
		return code + " " + std::to_string(getID()) + " " + std::to_string(getX()) + " " +
			std::to_string(getY()) + " " + getColor() + " " + std::to_string(getIsFilled()) + " " + std::to_string(radius);
	}
};

class Triangle : public Shape {
private:
	int height;
	int corner;
public:
	Triangle(int id, const std::string& color, bool isFilled, int x, int y, int h, int c)
		: Shape(id, color, isFilled, x, y, "triangle"), height(h), corner(c) {
	}

	bool edit(const std::vector<int>& params) override {
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

	std::string getInfo() override {
		return getName() + " " + getColor() + " " + getCoordinates() + " height and corner: " +
			std::to_string(height) + " " + std::to_string(corner);
	}

	std::vector<std::pair<int, int>> calcDots() override {
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

	std::string serialization() override {
		std::string code = "T";
		return code + " " + std::to_string(getID()) + " " + std::to_string(getX()) + " " +
			std::to_string(getY()) + " " + getColor() + " " + std::to_string(getIsFilled()) +
			" " + std::to_string(height) + " " + std::to_string(corner);
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

	std::string getInfo() override {
		return getName() + " " + getColor() + " " + getCoordinates() + " sides: " +
			std::to_string(side1) + " " + std::to_string(side2);
	}

	std::vector<std::pair<int, int>> calcDots() override {
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

	std::string serialization() override {
		std::string code = "B";
		return code + " " + std::to_string(getID()) + " " + std::to_string(getX()) + " " +
			std::to_string(getY()) + " " + getColor() + " " + std::to_string(getIsFilled())
			+ " " + std::to_string(side1) + " " + std::to_string(side2);
	}
};

class Line : public Shape {
private:
	int length;
	int angle;

public:
	Line(int id, const std::string& color, bool isFilled, int x, int y, int len, int ang)
		: Shape(id, color, isFilled, x, y, "line"), length(len), angle(ang) {
	}

	bool edit(const std::vector<int>& params) override {
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

	std::string getInfo() override {
		return getName() + " " + getColor() + " " + getCoordinates() + " length and angle: " +
			std::to_string(length) + " " + std::to_string(angle);
	}

	std::vector<std::pair<int, int>> calcDots() override {
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

	std::string serialization() override {
		std::string code = "L";
		return code + " " + std::to_string(getID()) + " " + std::to_string(getX()) + " " +
			std::to_string(getY()) + " " + getColor() + " " + std::to_string(getIsFilled()) +
			" " + std::to_string(length) + " " + std::to_string(angle);
	}
};