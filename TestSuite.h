#pragma once

#include "MyGraph.h"
#include <iostream>
#include <string>
#include <limits>

inline void RunAllTests()
{
    std::cout << "\nRunning tests...\n";

    {
        // Тест 1: Добавление вершин и рёбер, проверка раскраски (числовые вершины)
        MyGraph<int, double> testGraph;
        testGraph.InsertVertex(10);
        testGraph.InsertVertex(20);
        testGraph.InsertVertex(30);
        testGraph.ConnectNodes(10, 20, 5.5);
        testGraph.ConnectNodes(20, 30, 7.2);
        testGraph.ConnectNodes(10, 30, 3.3);

        auto colors = testGraph.GraphColoring();
        bool atLeastTwoColorsUsed = false;
        for (int i = 0; i < colors.GetLength(); i++)
        {
            if (colors.GetElem(i) > 0)
            {
                atLeastTwoColorsUsed = true;
                break;
            }
        }

        if (!atLeastTwoColorsUsed)
            std::cout << "Test1 Failed: Coloring should use more than one color.\n";
        else
            std::cout << "Test1 Passed.\n";
    }

    {
        // Тест 2: Проверка кратчайших расстояний (простейший пример, числовые вершины)
        MyGraph<int, double> testGraph;
        testGraph.InsertVertex(1);
        testGraph.InsertVertex(2);
        testGraph.InsertVertex(3);
        testGraph.ConnectNodes(1, 2, 10.0);
        testGraph.ConnectNodes(2, 3, 5.0);

        auto dist = testGraph.MinimalDistances(1);

        bool testPassed = true;
        if (dist.GetElem(0).distance != 0 || dist.GetElem(1).distance != 10 || dist.GetElem(2).distance != 15)
        {
            std::cout << "Test2 Failed: Distances are incorrect.\n";
            testPassed = false;
        }
        else
        {
            std::cout << "Test2 Passed.\n";
        }

        if (testPassed)
        {
            std::cout << "Distances from " << 1 << ":\n";
            for (int i = 0; i < dist.GetLength(); i++)
            {
                int d = dist.GetElem(i).distance;
                if (d == -1)
                    std::cout << "Node " << testGraph.GetVertex(i) << " -> unreachable\n";
                else
                    std::cout << "Node " << testGraph.GetVertex(i) << " -> distance " << d << "\n";
            }
        }
    }

    {
        // Тест 3: Добавление строковых вершин и рёбер, проверка раскраски
        MyGraph<std::string, double> testGraph;
        testGraph.InsertVertex("A");
        testGraph.InsertVertex("B");
        testGraph.InsertVertex("C");
        testGraph.InsertVertex("D");
        testGraph.ConnectNodes("A", "B", 1.5);
        testGraph.ConnectNodes("B", "C", 2.5);
        testGraph.ConnectNodes("C", "D", 3.5);
        testGraph.ConnectNodes("A", "D", 4.5);

        
        auto colors = testGraph.GraphColoring();
        bool atLeastTwoColorsUsed = false;
        for (int i = 0; i < colors.GetLength(); i++)
        {
            if (colors.GetElem(i) > 0)
            {
                atLeastTwoColorsUsed = true;
                break;
            }
        }

        if (!atLeastTwoColorsUsed)
            std::cout << "Test3 Failed: Coloring should use more than one color.\n";
        else
            std::cout << "Test3 Passed.\n";
    }

    {
        // Тест 4: Проверка кратчайших расстояний (простейший пример, строковые вершины)
        MyGraph<std::string, double> testGraph;
        testGraph.InsertVertex("Start");
        testGraph.InsertVertex("Middle");
        testGraph.InsertVertex("End");
        testGraph.ConnectNodes("Start", "Middle", 2.0);
        testGraph.ConnectNodes("Middle", "End", 3.0);

        
        auto dist = testGraph.MinimalDistances("Start");
        bool testPassed = true;
        if (dist.GetElem(0).distance != 0 || dist.GetElem(1).distance != 2 || dist.GetElem(2).distance != 5)
        {
            std::cout << "Test4 Failed: Distances are incorrect.\n";
            testPassed = false;
        }
        else
        {
            std::cout << "Test4 Passed.\n";
        }

        if (testPassed)
        {
            std::cout << "Distances from Start:\n";
            for (int i = 0; i < dist.GetLength(); i++)
            {
                int d = dist.GetElem(i).distance;
                if (d == -1)
                    std::cout << "Node " << testGraph.GetVertex(i) << " -> unreachable\n";
                else
                    std::cout << "Node " << testGraph.GetVertex(i) << " -> distance " << d << "\n";
            }
        }
    }

    
    std::cout << "Tests completed.\n\n";
}
