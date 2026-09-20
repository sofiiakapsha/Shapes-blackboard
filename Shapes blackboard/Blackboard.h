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
    Shape* getSelected() { return selected; }
    bool selectByCoordinates(int x, int y);
    bool moveSelected(int newX, int newY);
    void list();
    bool remove();
    void clear();
    bool editSelected(const std::vector<int>& params);
    bool save(std::string path);
    static std::unique_ptr<Blackboard> load(std::string path);
    bool draw();
    void all_shapes();
};