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
    for (auto& shape : shapes) {
        if (shape->getID() == id) {
            selected = shape.get();
            selected->makeSelected();
            return true;
        }
    }
    std::cout << "error: figure does not exist\n";
    return false;
}

bool Blackboard::remove() {
    if (!selected) {
        std::cout << "error: figure does not exist\n";
        return false;
    }

    int id = selected->getID();
    std::string name = selected->getName();

    for (auto it = shapes.begin(); it != shapes.end(); ++it) {
        if (it->get() == selected) {
            shapes.erase(it);
            break;
        }
    }

    selected = nullptr;
    std::cout << "remove " + std::to_string(id) + " " + name + "\n";
    return true;
}

void Blackboard::clear() {
    shapes.clear();
    std::cout << "board is clear";
}

bool Blackboard::draw() {
    std::vector<std::vector<char>> grid(height, std::vector<char>(width, ' '));
    for (auto& shape : shapes) {
        char symbol = shape->getColor()[0];

        for (auto& [px, py] : shape->getDots()) {
            if (px >= 0 && px < width && py >= 0 && py < height) {
                grid[py][px] = symbol;
            }
        }
    }

    for (auto& row : grid) {
        for (char c : row) {
            std::cout << c;
        }
        std::cout << "\n";
    }
    return true;

}