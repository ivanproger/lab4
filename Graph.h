#pragma once
#include "DynamicArray.h"
#include "HashTable.h"
#include "WeightedEdge.h"
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>
#include <cstdlib>
#include <type_traits>
#include <functional>

template <typename TKey>
struct PathInfo {
    int distance;
    DynamicArray<TKey> path;
};

template <typename TKey, typename WeightType = double>
class Graph {
private:
    DynamicArray<TKey> Nodes;
    HashTable<TKey, DynamicArray<MyWeightedEdge<TKey, WeightType>>> AdjacencyData;

public:
    Graph() : Nodes(), AdjacencyData(11) {}

    int FindNodeIndex(const TKey& node) const {
        for (int i = 0; i < Nodes.GetLength(); i++) {
            if (Nodes.GetElem(i) == node)
                return i;
        }
        return -1;
    }

    int GetNodeCount() const {
        return Nodes.GetLength();
    }

    TKey GetVertex(int index) const {
        if (index < 0 || index >= Nodes.GetLength())
            throw std::out_of_range("GetVertex index out of range");
        return Nodes[index];
    }

    DynamicArray<MyWeightedEdge<TKey, WeightType>> GetAdjacentVertices(const TKey& vertex) const {
        if (!AdjacencyData.exist(vertex)) {
            DynamicArray<MyWeightedEdge<TKey, WeightType>> emptySet;
            return emptySet;
        }
        return AdjacencyData.get(vertex);
    }

    bool HasVertex(const TKey& vertex) const {
        return AdjacencyData.exist(vertex);
    }

    void InsertVertex(const TKey& vertex) {
        if (!AdjacencyData.exist(vertex)) {
            DynamicArray<MyWeightedEdge<TKey, WeightType>> edges;
            AdjacencyData.insert(vertex, edges);
            Nodes.Append(vertex);
        }
    }

    void EraseVertex(const TKey& vertex) {
        if (!AdjacencyData.exist(vertex))
            return;

        DynamicArray<MyWeightedEdge<TKey, WeightType>> edgesToRemove = AdjacencyData.get(vertex);
        for (int i = 0; i < edgesToRemove.GetLength(); i++) {
            TKey neighbor = edgesToRemove[i].GetNode();
            if (AdjacencyData.exist(neighbor)) {
                auto theirEdges = AdjacencyData.get(neighbor);
                for (int j = 0; j < theirEdges.GetLength(); j++) {
                    if (theirEdges[j].GetNode() == vertex) {
                        theirEdges.RemoveAt(j);
                        break;
                    }
                }
                AdjacencyData.insert(neighbor, theirEdges);
            }
        }

        AdjacencyData.remove(vertex);

        for (int i = 0; i < Nodes.GetLength(); i++) {
            if (Nodes[i] == vertex) {
                Nodes.RemoveAt(i);
                break;
            }
        }
    }

    void ConnectNodes(const TKey& from, const TKey& to, WeightType weight) {
        if (!AdjacencyData.exist(from) || !AdjacencyData.exist(to)) {
            return;
        }
        {
            auto edgesFrom = AdjacencyData.get(from);
            for (int i = 0; i < edgesFrom.GetLength(); i++) {
                if (edgesFrom[i].GetNode() == to) {
                    return;
                }
            }
            edgesFrom.Append(MyWeightedEdge<TKey, WeightType>(to, weight));
            AdjacencyData.insert(from, edgesFrom);
        }

        {
            auto edgesTo = AdjacencyData.get(to);
            for (int i = 0; i < edgesTo.GetLength(); i++) {
                if (edgesTo[i].GetNode() == from) {
                    return;
                }
            }
            edgesTo.Append(MyWeightedEdge<TKey, WeightType>(from, weight));
            AdjacencyData.insert(to, edgesTo);
        }
    }

    void DisconnectNodes(const TKey& from, const TKey& to) {
        if (!AdjacencyData.exist(from) || !AdjacencyData.exist(to)) {
            return;
        }
        {
            auto edgesFrom = AdjacencyData.get(from);
            for (int i = 0; i < edgesFrom.GetLength(); i++) {
                if (edgesFrom[i].GetNode() == to) {
                    edgesFrom.RemoveAt(i);
                    break;
                }
            }
            AdjacencyData.insert(from, edgesFrom);
        }
        {
            auto edgesTo = AdjacencyData.get(to);
            for (int i = 0; i < edgesTo.GetLength(); i++) {
                if (edgesTo[i].GetNode() == from) {
                    edgesTo.RemoveAt(i);
                    break;
                }
            }
            AdjacencyData.insert(to, edgesTo);
        }
    }

    void GenerateGraph(int nodeCount, int edgeCount, WeightType minWeight, WeightType maxWeight) {
        ClearGraph();
        if (nodeCount < 0) nodeCount = 0;
        if (edgeCount < 0) edgeCount = 0;
        if (minWeight < static_cast<WeightType>(1)) minWeight = static_cast<WeightType>(1);
        if (maxWeight < minWeight) maxWeight = minWeight;

        for (int i = 0; i < nodeCount; i++) {
            if constexpr (std::is_same<TKey, std::string>::value) {
                InsertVertex("Node" + std::to_string(i));
            }
            else {
                InsertVertex(i);
            }
        }

        int addedEdges = 0;
        while (addedEdges < edgeCount) {
            if (nodeCount <= 1) break;
            int a = std::rand() % nodeCount;
            int b = std::rand() % nodeCount;
            if (a == b) continue;
            TKey from = Nodes[a];
            TKey to = Nodes[b];

            auto edgesFrom = AdjacencyData.get(from);
            bool exists = false;
            for (int i = 0; i < edgesFrom.GetLength(); i++) {
                if (edgesFrom[i].GetNode() == to) {
                    exists = true;
                    break;
                }
            }
            if (exists) continue;

            WeightType w = minWeight + (static_cast<WeightType>(std::rand()) / RAND_MAX) * (maxWeight - minWeight);

            ConnectNodes(from, to, w);
            addedEdges++;
        }
    }

    void ClearGraph() {
        AdjacencyData.Clear();
        Nodes.Clear();
    }

    std::string ToString() const {
        std::string res;
        for (int i = 0; i < Nodes.GetLength(); i++) {
            TKey node = Nodes[i];
            res += "Vertex: " + KeyToString(node) + " -> [";
            if (AdjacencyData.exist(node)) {
                auto edges = AdjacencyData.get(node);
                for (int j = 0; j < edges.GetLength(); j++) {
                    res += KeyToString(edges[j].GetNode()) +
                        "(w=" + std::to_string(static_cast<double>(edges[j].GetWeight())) +
                        ")";
                    if (j < edges.GetLength() - 1)
                        res += ", ";
                }
            }
            res += "]\n";
        }
        return res;
    }

    void SaveToFile(const char* filename) const {
        std::ofstream outFile(filename);
        if (!outFile.is_open()) {
            throw std::runtime_error("Failed to open file for saving.");
        }
        outFile << Nodes.GetLength() << "\n";
        for (int i = 0; i < Nodes.GetLength(); i++) {
            outFile << KeyToString(Nodes[i]) << "\n";
        }
        for (int i = 0; i < Nodes.GetLength(); i++) {
            TKey node = Nodes[i];
            if (AdjacencyData.exist(node)) {
                auto edges = AdjacencyData.get(node);
                for (int j = 0; j < edges.GetLength(); j++) {
                    outFile << KeyToString(node)
                        << " " << KeyToString(edges[j].GetNode())
                        << " " << edges[j].GetWeight() << "\n";
                }
            }
        }
        outFile.close();
    }

    void LoadFromFile(const char* filename) {
        ClearGraph();
        std::ifstream inFile(filename);
        if (!inFile.is_open()) {
            throw std::runtime_error("Failed to open file for loading.");
        }
        int nodeCount = 0;
        inFile >> nodeCount;

        for (int i = 0; i < nodeCount; i++) {
            TKey vertex;
            inFile >> vertex;
            InsertVertex(vertex);
        }

        TKey v1, v2;
        WeightType w;
        while (inFile >> v1 >> v2 >> w) {
            ConnectNodes(v1, v2, w);
        }
        inFile.close();
    }

private:
    std::string KeyToString(const TKey& key) const {
        if constexpr (std::is_same<TKey, std::string>::value) {
            return key;
        }
        else {
            return std::to_string(key);
        }
    }
};