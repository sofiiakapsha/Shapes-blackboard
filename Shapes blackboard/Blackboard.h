#pragma once
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "Shapes.h"

class Blackboard {
private:
    int idBoard;
    std::vector<std::unique_ptr<Shape>> shapes;
    Shape* selected;
    int width, height;
    int nextId = 1;

public:
    Blackboard(int idBoard, int width, int height)
        : idBoard(idBoard), width(width), height(height), selected(nullptr) {
    }

    bool add(int px, int py, int shape, std::string color,
        const std::vector<int>& params, bool isFilled) {

        std::unique_ptr<Shape> newShape;

        if (shape == 1) {
            if (params.size() != 1) {
                std::cout << "error: invalid argument count\n";
                return false;
            }
            newShape = std::make_unique<Circle>(nextId, color, isFilled, px, py, params[0]);
        }
        else if (shape == 2) {
            if (params.size() != 3) {
                std::cout << "error: invalid argument count\n";
                return false;
            }
            newShape = std::make_unique<Triangle>(nextId, color, isFilled, px, py,
                params[0], params[1], params[2]);
        }
        else if (shape == 3) {
            if (params.size() != 2) {
                std::cout << "error: invalid argument count\n";
                return false;
            }
            newShape = std::make_unique<Box>(nextId, color, isFilled, px, py, params[0], params[1]);
        }
        else {
            std::cout << "error: unknown shape type\n";
            return false;
        }

        nextId++;
        shapes.push_back(std::move(newShape));
        std::cout << (nextId - 1) << " added\n";
        return true;
    }
};