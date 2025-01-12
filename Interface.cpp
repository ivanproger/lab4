#include "Interface.h"


void interface()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    std::cout << "Select the type of graph vertices:\n"
        << "1. Integer vertices\n"
        << "2. String vertices\n"
        << "Enter choice (1 or 2): ";
    int vertexTypeChoice;
    std::cin >> vertexTypeChoice;

    if (vertexTypeChoice == 1) {
        MyGraph<int, double> graph;
        GraphMenu(graph);
    }
    else if (vertexTypeChoice == 2) {
        MyGraph<std::string, double> graph;
        GraphMenu(graph);
    }
    else {
        std::cout << "Invalid choice. Exiting.\n";
    }
}
