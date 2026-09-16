#include "Shapes.h"
#include "Blackboard.h"

bool Blackboard::add(int px, int py, int shape, std::string color,
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

void Blackboard::list() {
    int num = shapes.size();
    for (int i = 0; i < num; i++) {
        Shape* curShape = shapes[i].get();
        std::cout << curShape->getInfo();
    }
}

bool Blackboard::select(int id) {
    if (!shapes[id - 1]) {
        std::cout << "error: figure does not exist\n";
        return false;
    }

    selected = shapes[id - 1].get();
    selected->makeSelected();
    return true;
}

bool Blackboard::remove() {
    if (!selected) {
        std::cout << "error: figure does not exist\n";
        return false;
    }

    int id = selected->getID();
    shapes.erase(shapes.begin() + id);

    std::cout << "remove " + std::to_string(selected->getID()) + " " + selected->getName();
    return true;
}

void Blackboard::clear() {
    shapes.clear();
    std::cout << "board is clear";
}

bool Blackboard::draw() {

}