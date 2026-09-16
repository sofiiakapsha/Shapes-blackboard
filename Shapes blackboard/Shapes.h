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
	std::vector<std::pair<int, int>> dots;
public:
	Shape(int id, const std::string& color, bool isFilled, int x, int y)
		: id(id), color(color), isFilled(isFilled), x(x), y(y) {
		isSelected = false;
	}

	int getID() { return id; }
	std::string getColor(){ return color; }
	void makeSelected() { isSelected = true; }
	bool isSelect() { return isSelected; }
	void changeColor(std::string newColor) { color = newColor; }
	std::string getCoordinates() { return std::to_string(x) + " " + std::to_string(y); }
	std::string getName() { return name; }
	virtual std::vector<std::pair<int, int>> calcDots() = 0;

	bool getIsFilled() { return isFilled; }


	void move(int newX, int newY) {
		x = newX;
		y = newY;
	}

	virtual bool edit(const std::vector<int>& params) = 0;
	virtual std::string getInfo() = 0;
	virtual ~Shape() = default;

	std::vector<std::pair<int, int>> getDots() {
		return dots;
	}

	int getX() { return x; }
	int getY() { return y; }

};

class Circle : public Shape {
private: int radius;
	   std::string name = "circle";
public:
	Circle(int id, const std::string& color, bool isFilled, int x, int y,
		int r) : Shape(id, color, isFilled, x, y), radius(r) {};

	bool edit(const std::vector<int>& params) override {
		if (params.size() != 1) {
			std::cout << "error: invalid argument count\n";
			return false;
		}

		radius = params[0];
		return true;
	}

	std::string getInfo() override {
		return name + " " + getColor() + " " + getCoordinates() + " radius: " +
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
};

class Triangle : public Shape {
private: int side1, side2, side3;
	   std::string name = "triangle";
public:
	Triangle(int id, const std::string& color, bool isFilled, int x, int y,
		int s1, int s2, int s3)
		: Shape(id, color, isFilled, x, y), side1(s1), side2(s2), side3(s3) {
	}

	bool edit(const std::vector<int>& params) override {
		if (params.size() != 3) {
			std::cout << "error: invalid argument count\n";
			return false;
		}

		if (params[0] + params[1] < params[2]) {
			std::cout << "error: invalid argument for side3\n";
			return false;
		}

		side1 = params[0];
		side2 = params[1];
		side3 = params[2];
		return true;
	}

	std::string getInfo() override {
		return name + " " + getColor() + " " + getCoordinates() + " sides: " +
			std::to_string(side1) + " " + std::to_string(side2) + " " + std::to_string(side3);
	}

};

class Box : public Shape {
private: int side1, side2;
	   std::string name = "box";
public:
	Box(int id, const std::string& color, bool isFilled, int x, int y,
		int s1, int s2)
		: Shape(id, color, isFilled, x, y), side1(s1), side2(s2) {
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
		return name + " " + getColor() + " " + getCoordinates() + " sides: " +
			std::to_string(side1) + " " + std::to_string(side2);
	}


};