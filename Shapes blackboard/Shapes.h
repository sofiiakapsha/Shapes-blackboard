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

	int getID();
	std::string getColor();
	void changeColor(std::string newColor);
	std::string getCoordinates();
	std::string getName();
	virtual std::vector<std::pair<int, int>> calcDots() = 0;
	virtual std::string serialization() = 0;

	bool getIsFilled();
	void move(int newX, int newY);

	virtual bool edit(const std::vector<int>& params) = 0;
	virtual std::string getInfo() = 0;
	virtual ~Shape() = default;
	virtual std::unique_ptr<Shape> clone() = 0;

	std::vector<std::pair<int, int>> getDots();

	int getX();
	int getY();
};