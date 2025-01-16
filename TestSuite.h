#pragma once
#include <cassert>
#include <iostream>
#include "GraphUtils.h"

inline void RunAllTests()
{
    using std::cout;
    using std::endl;

    cout << "\nRunning tests...\n";

    {
        Graph<int, double> g;
        assert(g.GetNodeCount() == 0);
        g.EraseVertex(100);
        assert(g.GetNodeCount() == 0);
        auto c = GraphColoring(g);
        assert(c.GetLength() == 0);
        auto d = MinDistances(g, 0);
        assert(d.GetLength() == 0);
        cout << "Test: empty graph -> Passed.\n";
    }

    {
        Graph<int, double> g;
        g.InsertVertex(42);
        assert(g.GetNodeCount() == 1);
        assert(g.GetVertex(0) == 42);
        g.EraseVertex(100);
        assert(g.GetNodeCount() == 1);
        auto c = GraphColoring(g);
        assert(c.GetLength() == 1);
        assert(c[0] == 0);
        auto d = MinDistances(g, 42);
        assert(d.GetLength() == 1);
        assert(d[0].distance == 0);
        assert(d[0].path.GetLength() == 1);
        assert(d[0].path[0] == 42);
        cout << "Test: single-vertex -> Passed.\n";
    }

    {
        Graph<int, double> g;
        g.InsertVertex(1);
        g.InsertVertex(2);
        assert(g.GetNodeCount() == 2);
        g.ConnectNodes(1, 2, 5.0);
        {
            auto n1 = g.GetAdjacentVertices(1);
            assert(n1.GetLength() == 1 && n1[0].GetNode() == 2 && n1[0].GetWeight() == 5.0);
            auto n2 = g.GetAdjacentVertices(2);
            assert(n2.GetLength() == 1 && n2[0].GetNode() == 1 && n2[0].GetWeight() == 5.0);
        }
        auto c = GraphColoring(g);
        assert(c.GetLength() == 2);
        assert(c[0] != c[1]);
        auto d1 = MinDistances(g, 1);
        assert(d1.GetLength() == 2);
        assert(d1[0].distance == 0);
        assert(d1[1].distance == 5);
        auto d2 = MinDistances(g, 2);
        assert(d2.GetLength() == 2);
        assert(d2[1].distance == 0);
        assert(d2[0].distance == 5);
        g.DisconnectNodes(1, 2);
        {
            auto n1 = g.GetAdjacentVertices(1);
            auto n2 = g.GetAdjacentVertices(2);
            assert(n1.GetLength() == 0 && n2.GetLength() == 0);
        }
        g.EraseVertex(1);
        assert(g.GetNodeCount() == 1 && g.GetVertex(0) == 2);
        cout << "Test: two-vertex -> Passed.\n";
    }

    {
        Graph<int, double> g;
        for (int i = 0; i < 10; i++) g.InsertVertex(i);
        assert(g.GetNodeCount() == 10);
        g.ConnectNodes(0, 1, 2.0);
        g.ConnectNodes(1, 2, 2.0);
        g.ConnectNodes(2, 3, 2.0);
        g.ConnectNodes(5, 9, 5.0);
        g.ConnectNodes(4, 5, 1.0);
        g.ConnectNodes(7, 8, 1.5);
        auto c = GraphColoring(g);
        assert(c.GetLength() == 10);
        auto d = MinDistances(g, 0);
        assert(d.GetLength() == 10);
        assert(d[0].distance == 0);
        assert(d[1].distance == 2);
        assert(d[2].distance == 4);
        assert(d[3].distance == 6);
        bool reachedOthers = false;
        for (int i = 4; i < 10; i++) {
            if (d[i].distance != -1) { reachedOthers = true; break; }
        }
        assert(!reachedOthers);
        cout << "Test: 10-vertex partial -> Passed.\n";
    }

    {
        Graph<int, double> testGraph;
        testGraph.InsertVertex(10);
        testGraph.InsertVertex(20);
        testGraph.InsertVertex(30);
        testGraph.ConnectNodes(10, 20, 5.5);
        testGraph.ConnectNodes(20, 30, 7.2);
        testGraph.ConnectNodes(10, 30, 3.3);
        auto c = GraphColoring(testGraph);
        bool multiColor = false;
        for (int i = 0; i < c.GetLength(); i++) {
            if (c[i] > 0) { multiColor = true; break; }
        }
        assert(multiColor == true);
        auto d = MinDistances(testGraph, 10);
        assert(d.GetLength() == 3);
        cout << "Test: numeric multi-edge -> Passed.\n";
    }

    {
        Graph<int, double> testGraph;
        testGraph.InsertVertex(1);
        testGraph.InsertVertex(2);
        testGraph.InsertVertex(3);
        testGraph.ConnectNodes(1, 2, 10.0);
        testGraph.ConnectNodes(2, 3, 5.0);
        auto dist = MinDistances(testGraph, 1);
        assert(dist.GetLength() == 3);
        assert(dist[0].distance == 0);
        cout << "Test: distances numeric -> Passed.\n";
    }

    {
        Graph<std::string, double> testGraph;
        testGraph.InsertVertex("A");
        testGraph.InsertVertex("B");
        testGraph.InsertVertex("C");
        testGraph.InsertVertex("D");

        testGraph.ConnectNodes("A", "B", 1.5);
        testGraph.ConnectNodes("B", "C", 2.5);
        testGraph.ConnectNodes("C", "D", 3.5);
        testGraph.ConnectNodes("A", "D", 4.5);

        auto c = GraphColoring(testGraph);
        assert(c.GetLength() == 4);

        std::cout << "Colors: ";
        for (int i = 0; i < c.GetLength(); i++) {
            std::cout << "vertex=" << testGraph.GetVertex(i)
                << " color=" << c[i] << "; ";
        }
        std::cout << "\n";

        for (int i = 0; i < testGraph.GetNodeCount(); i++) {
            auto v = testGraph.GetVertex(i);
            auto edges = testGraph.GetAdjacentVertices(v);
            for (int e = 0; e < edges.GetLength(); e++) {
                auto neighbor = edges[e].GetNode();
                int idxV = testGraph.FindNodeIndex(v);
                int idxNeigh = testGraph.FindNodeIndex(neighbor);
                assert(c[idxV] != c[idxNeigh]);
            }
        }
        std::cout << "Detailed check: no two adjacent vertices share the same color.\n";

        bool usedMoreThanOne = false;
        for (int i = 0; i < c.GetLength(); i++) {
            if (c[i] > 0) {
                usedMoreThanOne = true;
                break;
            }
        }
        assert(usedMoreThanOne);
        std::cout << "Test: coloring string, verified by adjacency edges -> Passed.\n";
    }

    {
        Graph<std::string, double> testGraph;
        testGraph.InsertVertex("Start");
        testGraph.InsertVertex("Middle");
        testGraph.InsertVertex("End");

        testGraph.ConnectNodes("Start", "Middle", 2.0);
        testGraph.ConnectNodes("Middle", "End", 3.0);

        auto dist = MinDistances(testGraph, std::string("Start"));
        assert(dist.GetLength() == 3);

        {
            int idxStart = testGraph.FindNodeIndex("Start");
            int idxMiddle = testGraph.FindNodeIndex("Middle");
            int idxEnd = testGraph.FindNodeIndex("End");
            assert(idxStart != -1);
            assert(idxMiddle != -1);
            assert(idxEnd != -1);

            assert(dist[idxStart].distance == 0);
            assert(dist[idxMiddle].distance == 2);
            assert(dist[idxEnd].distance == 5);

            // Start -> path=[Start]
            assert(dist[idxStart].path.GetLength() == 1);
            assert(dist[idxStart].path[0] == "Start");
            // Middle -> path=[Start, Middle]
            assert(dist[idxMiddle].path.GetLength() == 2);
            assert(dist[idxMiddle].path[0] == "Start");
            assert(dist[idxMiddle].path[1] == "Middle");
            // End -> path=[Start, Middle, End]
            assert(dist[idxEnd].path.GetLength() == 3);
            assert(dist[idxEnd].path[0] == "Start");
            assert(dist[idxEnd].path[1] == "Middle");
            assert(dist[idxEnd].path[2] == "End");
        }
        std::cout << "Test: distances string with detailed path checks -> Passed.\n";
    }

    cout << "All tests Passed.\n\n";
}