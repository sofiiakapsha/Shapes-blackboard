#include "Shapes.h"
#include "Line.h"
#include "Box.h"
#include "Circle.h"
#include "Triangle.h"
#include "Blackboard.h"
#include <sstream>
#include <limits>
#include <memory>

void addNew(std::istringstream& com, std::unique_ptr<Blackboard>& board) {

    std::string fillMode, color, shapeName;
    com >> fillMode >> color >> shapeName;

    bool isFilled = (fillMode == "fill");

    int shapeType = 0;
    if (shapeName == "circle") shapeType = 1;
    else if (shapeName == "triangle") shapeType = 2;
    else if (shapeName == "box") shapeType = 3;
    else if (shapeName == "line") shapeType = 4;
    else {
        std::cout << "error: unknown shape type\n";
        return;
    }

    int x, y;
    if (!(com >> x >> y)) {
        std::cout << "error: missing coordinates\n";
        return;
    }

    std::vector<int> params;
    int p;
    while (com >> p) {
        params.push_back(p);
    }

    board->add(x, y, shapeType, color, params, isFilled);
}

void selectedInt(std::istringstream& com, std::unique_ptr<Blackboard>& board) {
    std::string arg1, arg2;
    com >> arg1;

    if (com >> arg2) {
        int x = std::stoi(arg1);
        int y = std::stoi(arg2);
        board->selectByCoordinates(x, y);
    }
    else {
        int id = std::stoi(arg1);
        board->select(id);
    }
}

void paint(std::istringstream& com, std::unique_ptr<Blackboard>& board) {
    std::string color;
    com >> color;
    if (board->getSelected() == nullptr) {
        std::cout << "error: no selected figure\n";
        return;
    }
    board->getSelected()->changeColor(color);
    std::cout << std::to_string(board->getSelected()->getID()) + " " +
        board->getSelected()->getName() + " " + color + "\n";
}

void edit(std::istringstream& com, std::unique_ptr<Blackboard>& board) {
    std::vector<int> params;
    int p;
    while (com >> p) {
        params.push_back(p);
    }
    if (board->getSelected() == nullptr) {
        std::cout << "error: no selected figure\n";
        return;
    }
    board->editSelected(params);
}

void runBoard(std::unique_ptr<Blackboard>& board) {
    bool isIn = true;
    while (isIn) {
        std::string choices;

        std::cout << "Menu Board:\n1. draw 2. list 3. shapes\n";
        std::cout << "4. add 5. select 6. remove\n";
        std::cout << "7. paint 8. move 9. clear\n";
        std::cout << "10. save 11. edit 12. load 13. exit\n";

        std::getline(std::cin, choices);
        if (choices.empty()) {
            continue;
        }

        std::istringstream com(choices);

        std::string command;
        com >> command;

        if (command == "draw") { board->draw(); }
        else if (command == "list") { board->list(); }
        else if (command == "shapes") { board->all_shapes(); }
        else if (command == "add") { addNew(com, board); }
        else if (command == "select") { selectedInt(com, board); }
        else if (command == "remove") {
            if (board->getSelected() == nullptr) {
                std::cout << "error: no selected figure\n";
                continue;
            }
            board->remove();
        }
        else if (command == "paint") {
            paint(com, board);
        }
        else if (command == "move") {
            int x, y;
            com >> x >> y;
            if (board->getSelected() == nullptr) {
                std::cout << "error: no selected figure\n";
                continue;
            }
            board->moveSelected(x, y);
        }
        else if (command == "edit") {
            edit(com, board);
        }
        else if (command == "clear") { board->clear(); }
        else if (command == "save") {
            std::string path;
            com >> path;
            board->save(path);
        }
        else if (command == "load") {
            std::string path;
            com >> path;

            std::unique_ptr<Blackboard> loaded = Blackboard::load(path);
            if (!loaded) {
                std::cout << "error: failed to load board\n";
                continue;
            }
            board = std::move(loaded);
            std::cout << "Board loaded\n";
        }
        else if (command == "exit") {
            isIn = false;
        }
        else {
            std::cout << "error: unknown command\n";
        }
    }
}

int main()
{
    bool isOut = true;
    int number = 0;
    std::unique_ptr<Blackboard> board = nullptr;

    while (isOut) {
        std::cout << "Menu:\n1. new\n2. continue\n3. exit\n";
        std::string choice;
        std::getline(std::cin, choice);

        if (choice.empty()) {
            continue;
        }

        if (choice == "new") {
            number++;
            std::cout << "Enter width and height:\n";
            std::string dim;
            std::getline(std::cin, dim);
            std::istringstream dimStream(dim);

            int width, height;
            if (!(dimStream >> width >> height)) {
                std::cout << "error: invalid dimensions\n";
                continue;
            }

            board = std::make_unique<Blackboard>(number, width, height);
            std::cout << "New Board\n";
        }
        else if (choice == "continue") {
            if (!board) {
                std::cout << "error: no board created yet\n";
                continue;
            }
            runBoard(board);
        }
        else if (choice == "exit") {
            isOut = false;
        }
        else {
            std::cout << "error: unknown command\n";
        }
    }
    return 0;
}