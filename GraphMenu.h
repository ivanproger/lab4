#pragma once

#include <iostream>
#include <string>
#include <limits>
#include "MyGraph.h"
#include "TestSuite.h"

template <typename TKey, typename WeightType>
void GraphMenu(MyGraph<TKey, WeightType>& graph) {
    bool exitFlag = false;
    while (!exitFlag)
    {
        std::cout << "\n1. Insert vertex\n"
            << "11. Erase vertex\n"
            << "2. Connect nodes\n"
            << "22. Disconnect nodes\n"
            << "3. Generate graph\n"
            << "4. Save graph to the file\n"
            << "5. Load graph from the file\n"
            << "6. Color graph\n"
            << "7. Calculate minimum distances\n"
            << "8. Print graph\n"
            << "9. Run tests\n"
            << "0. Exit\n"
            << "Enter command: ";

        int command;
        std::cin >> command;

        switch (command)
        {
        case 1:
        {
            std::cout << "Enter the vertex to insert: ";
            TKey v;
            std::cin >> v;
            graph.InsertVertex(v);
            std::cout << "Vertex inserted.\n";
            break;
        }
        case 11:
        {
            std::cout << "Enter the vertex to erase: ";
            TKey v;
            std::cin >> v;
            graph.EraseVertex(v);
            std::cout << "Vertex erased.\n";
            break;
        }
        case 2:
        {
            std::cout << "Enter two vertices and weight: ";
            TKey v1, v2;
            WeightType w;
            std::cin >> v1 >> v2 >> w;
            graph.ConnectNodes(v1, v2, w);
            std::cout << "Nodes connected.\n";
            break;
        }
        case 22:
        {
            std::cout << "Enter two vertices to disconnect: ";
            TKey v1, v2;
            std::cin >> v1 >> v2;
            graph.DisconnectNodes(v1, v2);
            std::cout << "Nodes disconnected.\n";
            break;
        }
        case 3:
        {
            std::cout << "Enter the number of vertices, the number of edges,\n"
                << "the minimum weight and the maximum weight: ";
            int n, e;
            WeightType minW, maxW;
            std::cin >> n >> e >> minW >> maxW;
            graph.GenerateGraph(n, e, minW, maxW);
            std::cout << "The graph is generated.\n";
            break;
        }
        case 4:
        {
            std::cout << "Enter file name to save: ";
            std::string fileName;
            std::cin >> fileName;
            try {
                graph.SaveToFile(fileName.c_str());
                std::cout << "Graph saved successfully.\n";
            }
            catch (const std::exception& ex) {
                std::cout << "Error saving graph: " << ex.what() << "\n";
            }
            break;
        }
        case 5:
        {
            std::cout << "Enter file name to load: ";
            std::string fileName;
            std::cin >> fileName;
            try {
                graph.LoadFromFile(fileName.c_str());
                std::cout << "Graph loaded successfully.\n";
            }
            catch (const std::exception& ex) {
                std::cout << "Error loading graph: " << ex.what() << "\n";
            }
            break;
        }
        case 6:
        {
            // Раскраска
            DynamicArray<int> colorResult = graph.GraphColoring();
            std::cout << "Coloring result:\n";
            for (int i = 0; i < colorResult.GetLength(); i++)
            {
                std::cout << "Node " << graph.GetVertex(i)
                    << " -> color " << colorResult.GetElem(i) << "\n";
            }
            break;
        }
        case 7:
        {
            std::cout << "Enter the start vertex for minimal distances: ";
            TKey startV;
            std::cin >> startV;
            DynamicArray<PathInfo<TKey>> paths = graph.MinimalDistances(startV);
            std::cout << "Distances and paths from " << startV << ":\n";
            for (int i = 0; i < paths.GetLength(); i++)
            {
                auto& pi = paths.GetElem(i);
                TKey node = graph.GetVertex(i);
                if (pi.distance == -1)
                    std::cout << "Node " << node << " -> unreachable\n";
                else
                {
                    std::cout << "Node " << node << " -> distance " << pi.distance << ", path: ";
                    for (int j = 0; j < pi.path.GetLength(); j++) {
                        std::cout << pi.path.GetElem(j);
                        if (j < pi.path.GetLength() - 1)
                            std::cout << " -> ";
                    }
                    std::cout << "\n";
                }
            }
            break;
        }
        case 8:
        {
            std::string gStr = graph.ToString();
            std::cout << gStr;
            break;
        }
        case 9:
        {
            RunAllTests();
            break;
        }
        case 0:
        {
            exitFlag = true;
            break;
        }
        default:
            std::cout << "Unknown command.\n";
            break;
        }
    }
}
