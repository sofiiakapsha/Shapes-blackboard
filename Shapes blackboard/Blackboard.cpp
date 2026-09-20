#include "Shapes.h"
#include "Blackboard.h"
#include <fstream>
#include <sstream>

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
        if (params.size() != 1) {
            std::cout << "error: invalid argument count\n";
            return false;
        }
        newShape = std::make_unique<Triangle>(nextId, color, isFilled, px, py,
            params[0]);
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
    for (auto& shape : shapes) {
        std::cout << shape->getID() << " " << shape->getInfo() << "\n";
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


void Blackboard::all_shapes() {
    std::cout << "circle: radius\n";
    std::cout << "triangle: height\n";
    std::cout << "box: width height\n";
}

bool Blackboard::save(std::string path) {
    std::ofstream out(path);
    if (!out.is_open()) {
        std::cout << "error: cannot open file\n";
        return false;
    }

    out << "D " + std::to_string(idBoard) + " " + std::to_string(width) + " " + std::to_string(height) + "\n";

    for (auto& shape : shapes) {
        out << shape->serialization() + "\n";
    }

    out.close();
    return true;
}

std::unique_ptr<Blackboard> Blackboard::load(std::string path) {
    std::ifstream in(path);
    if (!in.is_open()) {
        std::cout << "error: cannot open file\n";
        return nullptr;
    }

    std::string boardLine;
    std::getline(in, boardLine);
    std::istringstream boardStream(boardLine);
    std::string marker;
    int fileId, fileWidth, fileHeight;
    boardStream >> marker >> fileId >> fileWidth >> fileHeight;

    if (boardStream.fail() || marker != "D") {
        std::cout << "error: invalid file format\n";
        return nullptr;
    }

    std::vector<std::unique_ptr<Shape>> tempShapes;
    std::string line;

    while (std::getline(in, line)) {
        std::istringstream lines(line);
        std::string mark;

        int id, x, y, isFilledInt;
        std::string color;

        lines >> mark >> id >> x >> y >> color >> isFilledInt;
        bool isFilled = (isFilledInt != 0);

        if (lines.fail()) {
            std::cout << "error: invalid file format\n";
            return nullptr;
        }

        std::unique_ptr<Shape> shape;

        if (mark == "C") {
            int radius;
            lines >> radius;
            if (lines.fail()) {
                std::cout << "error: invalid file format\n";
                return nullptr;
            }
            shape = std::make_unique<Circle>(id, color, isFilled, x, y, radius);
        }
        else if (mark == "T") {
            int heightT;
            lines >> heightT;
            if (lines.fail()) {
                std::cout << "error: invalid file format\n";
                return nullptr;
            }
            shape = std::make_unique<Triangle>(id, color, isFilled, x, y, heightT);
        }
        else if (mark == "B") {
            int side1, side2;
            lines >> side1 >> side2;
            if (lines.fail()) {
                std::cout << "error: invalid file format\n";
                return nullptr;
            }
            shape = std::make_unique<Box>(id, color, isFilled, x, y, side1, side2);
        }
        else {
            std::cout << "error: incorrect type of figure\n";
            return nullptr;
        }

        tempShapes.push_back(std::move(shape));
    }

    in.close();

    auto board = std::make_unique<Blackboard>(fileId, fileWidth, fileHeight);
    board->shapes = std::move(tempShapes);

    int maxId = 0;
    for (auto& shape : board->shapes) {
        maxId = std::max(maxId, shape->getID());
    }
    board->nextId = maxId + 1;

    return board;
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