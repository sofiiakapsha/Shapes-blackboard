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

        if (params[0] <= 0) {
            std::cout << "error: invalid parameters\n";
            return false;
        }

        newShape = std::make_unique<Circle>(nextId, color, isFilled, px, py, params[0]);
    }
    else if (shape == 2) {
        if (params.size() != 2) {
            std::cout << "error: invalid argument count\n";
            return false;
        }

        if (params[0] <= 0 || params[1] <= 0 || params[1] >= 180) {
            std::cout << "error: invalid parameters\n";
            return false;
        }

        newShape = std::make_unique<Triangle>(nextId, color, isFilled, px, py,
            params[0], params[1]);

    }
    else if (shape == 3) {
        if (params.size() != 2) {
            std::cout << "error: invalid argument count\n";
            return false;
        }

        if (params[0] <= 0 || params[1] <= 0) {
            std::cout << "error: invalid parameters\n";
            return false;
        }

        newShape = std::make_unique<Box>(nextId, color, isFilled, px, py, params[0], params[1]);
    }
    else if (shape == 4) {
        if (params.size() != 2) {
            std::cout << "error: invalid argument count\n";
            return false;
        }

        if (params[0] <= 0 || params[1] < 0 || params[1] > 180) {
            std::cout << "error: invalid parameters\n";
            return false;
        }

        newShape = std::make_unique<Line>(nextId, color, isFilled, px, py, params[0], params[1]);

    }
    else {
        std::cout << "error: unknown shape type\n";
        return false;
    }

    auto dots = newShape->getDots();

    if (dots.empty()) {
        std::cout << "error: invalid shape\n";
        return false;
    }

    int minX = dots[0].first, maxX = dots[0].first;
    int minY = dots[0].second, maxY = dots[0].second;
    bool anyOnBoard = false;

    for (auto& [dx, dy] : dots) {
        minX = std::min(minX, dx);
        maxX = std::max(maxX, dx);
        minY = std::min(minY, dy);
        maxY = std::max(maxY, dy);
        if (dx >= 0 && dx < width && dy >= 0 && dy < height) {
            anyOnBoard = true;
        }
    }

    int shapeWidth = maxX - minX + 1;
    int shapeHeight = maxY - minY + 1;

    if (shapeWidth > width || shapeHeight > height) {
        std::cout << "error: shape is bigger than the board\n";
        return false;
    }

    if (!anyOnBoard) {
        std::cout << "error: shape is outside of the board\n";
        return false;
    }

    for (auto& existing : shapes) {
        if (existing->getName() == newShape->getName() &&
            existing->getX() == newShape->getX() &&
            existing->getY() == newShape->getY() &&
            existing->getInfo().substr(existing->getInfo().find(':') + 1) ==
            newShape->getInfo().substr(newShape->getInfo().find(':') + 1)) {
            std::cout << "error: identical shape already exists at this position\n";
            return false;
        }
    }

    std::cout << std::to_string(nextId) + " " + newShape->getName() + " " + newShape->getColor();
    for (int p : params) {
        std::cout << " " + std::to_string(p);
    }
    std::cout << "\n";

    nextId++;
    shapes.push_back(std::move(newShape));
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
            std::cout << "select " + selected->getInfo() + "\n";
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
    std::cout << std::to_string(id) + " " + name + " removed\n";
    return true;
}

void Blackboard::clear() {
    shapes.clear();
    std::cout << "board is clear\n";
}


void Blackboard::all_shapes() {
    std::cout << "circle: radius\n";
    std::cout << "triangle: height corner\n";
    std::cout << "box: width height\n";
    std::cout << "line: length angle\n";
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
            int heightT, cornerT;
            lines >> heightT >> cornerT;
            if (lines.fail()) {
                std::cout << "error: invalid file format\n";
                return nullptr;
            }
            shape = std::make_unique<Triangle>(id, color, isFilled, x, y, heightT, cornerT);
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
        else if (mark == "L") {
            int len, ang;
            lines >> len >> ang;
            if (lines.fail()) {
                std::cout << "error: invalid file format\n";
                return nullptr;
            }
            shape = std::make_unique<Line>(id, color, isFilled, x, y, len, ang);
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
            int screenY = height - 1 - py;
            if (px >= 0 && px < width && screenY >= 0 && screenY < height) {
                grid[screenY][px] = symbol;
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


bool Blackboard::selectByCoordinates(int x, int y) {
    for (auto it = shapes.rbegin(); it != shapes.rend(); ++it) {
        Shape* shape = it->get();
        for (auto& [px, py] : shape->getDots()) {
            if (px == x && py == y) {
                selected = shape;
                return true;
            }
        }
    }
    std::cout << "shape was not found\n";
    selected = nullptr;
    return false;
}

bool Blackboard::moveSelected(int newX, int newY) {
    if (!selected) {
        std::cout << "error: no selected figure\n";
        return false;
    }

    selected->move(newX, newY);

    for (size_t i = 0; i < shapes.size(); i++) {
        if (shapes[i].get() == selected) {
            std::unique_ptr<Shape> temp = std::move(shapes[i]);
            shapes.erase(shapes.begin() + i);
            shapes.push_back(std::move(temp));
            break;
        }
    }

    std::cout << selected->getID() << " " << selected->getName() << " moved\n";
    return true;

}

bool Blackboard::editSelected(const std::vector<int>& params) {
    if (!selected) {
        std::cout << "error: no selected figure\n";
        return false;
    }

    int x = selected->getX();
    int y = selected->getY();

    if (!selected->edit(params)) {
        return false;
    }

    auto dots = selected->getDots();
    if (dots.empty()) {
        std::cout << "error: invalid shape\n";
        return false;
    }

    int minX = dots[0].first, maxX = dots[0].first;
    int minY = dots[0].second, maxY = dots[0].second;

    for (auto& [dx, dy] : dots) {
        minX = std::min(minX, dx);
        maxX = std::max(maxX, dx);
        minY = std::min(minY, dy);
        maxY = std::max(maxY, dy);
    }

    int shapeWidth = maxX - minX + 1;
    int shapeHeight = maxY - minY + 1;

    if (shapeWidth > width || shapeHeight > height) {
        std::cout << "error: shape will go out of the board\n";
        return false;
    }

    std::cout << "size of " + selected->getName() + " changed\n";
    return true;
}