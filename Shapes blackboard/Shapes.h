#pragma once
#include <string>
#include <cstdbool>
#include <vector>
#include <iostream>

class Shape {
private:
	int id;
	std::string color;
	int x, y;
	bool isFilled;
	bool isSelected;

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

	void move(int newX, int newY) {
		x = newX;
		y = newY;
	}

	virtual bool edit(const std::vector<std::string>& params) = 0;
};

class Circle : public Shape {
private: int radius;
public:
	bool edit(const std::vector<std::string>& params) override {
		if (params.size() != 1) {
			std::cout << "error: invalid argument count\n";
			return false;
		}
	}
};