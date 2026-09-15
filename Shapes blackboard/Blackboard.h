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
        const std::vector<int>& params, bool isFilled);

    bool select(int id);
    void list();
    bool remove();
    void clear() {};
    bool save(std::string path);
    bool load(std::string path);
};