#include "Shapes.h"
#include "Blackboard.h"
#include <sstream>
#include <limits>
#include <memory>

void runBoard(Blackboard& board) {
    bool isIn = true;
    while (isIn) {
        std::string choices;

        std::cout << "Menu Board:\n1. draw 2. list 3. shapes\n";
        std::cout << "4. add 5. select 6. remove\n";
        std::cout << "7. paint 8. move 9. clear\n";
        std::cout << "10. save 11. edit 12. exit\n";
        std::getline(std::cin, choices);
        std::istringstream com(choices);

        std::string command;
        com >> command;

        if (command == "draw") { board.draw(); }
        else if (command == "list") { board.list(); }
        else if (command == "shapes") { board.all_shapes(); }
        else if (command == "add") {
            std::string fillMode, color, shapeName;
            com >> fillMode >> color >> shapeName;

            bool isFilled = (fillMode == "fill");

            int shapeType = 0;
            if (shapeName == "circle") shapeType = 1;
            else if (shapeName == "triangle") shapeType = 2;
            else if (shapeName == "box") shapeType = 3;
            else {
                std::cout << "error: unknown shape type\n";
                continue;
            }

            std::vector<int> params;
            int p;
            while (com >> p) {
                params.push_back(p);
            }

            std::cout << "Enter coordinates (x y):\n";
            int x, y;
            std::cin >> x >> y;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            board.add(x, y, shapeType, color, params, isFilled);
        }
        else if (command == "select") {
            std::string arg1, arg2;
            com >> arg1;

            if (com >> arg2) {
                int x = std::stoi(arg1);
                int y = std::stoi(arg2);
            }
            else {
                int id = std::stoi(arg1);
                board.select(id);
            }
        }
        else if (command == "remove") {
            if (board.getSelected() == nullptr) {
                std::cout << "error: no selected figure\n";
                continue;
            }
            board.remove();
        }
        else if (command == "paint") {
            std::string color;
            com >> color;
            if (board.getSelected() == nullptr) {
                std::cout << "error: no selected figure\n";
                continue;
            }
            board.getSelected()->changeColor(color);
        }
        else if (command == "move") {
            int x, y;
            com >> x >> y;
            if (board.getSelected() == nullptr) {
                std::cout << "error: no selected figure\n";
                continue;
            }
            board.getSelected()->move(x, y);
        }
        else if (command == "edit") {
            std::vector<int> params;
            int p;
            while (com >> p) {
                params.push_back(p);
            }
            if (board.getSelected() == nullptr) {
                std::cout << "error: no selected figure\n";
                continue;
            }
            board.getSelected()->edit(params);
        }
        else if (command == "clear") {
            board.clear();
        }
        else if (command == "save") {
            std::string path;
            com >> path;
            board.save(path);
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
        std::string choice;
        std::cout << "Menu:\n1. new\n2. load\n3. continue\n4. exit\n";
        std::cin >> choice;

        if (choice == "new") {
            number++;

            int width, height;
            std::cout << "Enter width:\n";
            std::cin >> width;
            std::cout << "Enter height:\n";
            std::cin >> height;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            board = std::make_unique<Blackboard>(number, width, height);
            std::cout << "New Board\n";
        }
        else if (choice == "load") {
            std::string path;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Enter file path:\n";
            std::getline(std::cin, path);

            auto loaded = Blackboard::load(path);
            if (!loaded) {
                std::cout << "error: failed to load board\n";
                continue;
            }
            board = std::move(loaded);
            std::cout << "Board loaded\n";
        }
        else if (choice == "continue") {
            if (!board) {
                std::cout << "error: no board created yet\n";
                continue;
            }
            runBoard(*board);
        }
        else if (choice == "exit") {
            isOut = false;
        }
    }
    return 0;
}