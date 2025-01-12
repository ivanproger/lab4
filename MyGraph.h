#pragma once

#include "DynamicArray.h"
#include "HashTable.h"
#include "MyWeightedEdge.h"
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
class MyGraph {
private:
    DynamicArray<TKey> Nodes;
    HashTable<TKey, DynamicArray<MyWeightedEdge<TKey, WeightType>>> AdjacencyData;

public:
    MyGraph() : Nodes(), AdjacencyData(11) {}

    void InsertVertex(const TKey& vertex) {
        for (int i = 0; i < Nodes.GetLength(); i++) {
            if (Nodes.GetElem(i) == vertex) {
                return;
            }
        }
        DynamicArray<MyWeightedEdge<TKey, WeightType>> edges;
        AdjacencyData.insert(vertex, edges);
        Nodes.Append(vertex);
    }

    void EraseVertex(const TKey& vertex) {
        if (!AdjacencyData.exist(vertex))
            return;

        DynamicArray<MyWeightedEdge<TKey, WeightType>> edgesToRemove = AdjacencyData.get(vertex);
        for (int i = 0; i < edgesToRemove.GetLength(); i++) {
            TKey adjacentNode = edgesToRemove.GetElem(i).GetNode();
            if (AdjacencyData.exist(adjacentNode)) {
                DynamicArray<MyWeightedEdge<TKey, WeightType>> theirEdges = AdjacencyData.get(adjacentNode);
                for (int j = 0; j < theirEdges.GetLength(); j++) {
                    if (theirEdges.GetElem(j).GetNode() == vertex) {
                        theirEdges.RemoveAt(j);
                        break;
                    }
                }
                AdjacencyData.insert(adjacentNode, theirEdges);
            }
        }
        AdjacencyData.remove(vertex);

        for (int i = 0; i < Nodes.GetLength(); i++) {
            if (Nodes.GetElem(i) == vertex) {
                Nodes.RemoveAt(i);
                break;
            }
        }
    }

    void ConnectNodes(const TKey& from, const TKey& to, WeightType weight) {
        if (!AdjacencyData.exist(from) || !AdjacencyData.exist(to)) {
            return;
        }
        DynamicArray<MyWeightedEdge<TKey, WeightType>> edges1 = AdjacencyData.get(from);
        for (int i = 0; i < edges1.GetLength(); i++) {
            if (edges1.GetElem(i).GetNode() == to)
                return;
        }
        edges1.Append(MyWeightedEdge<TKey, WeightType>(to, weight));
        AdjacencyData.insert(from, edges1);

        DynamicArray<MyWeightedEdge<TKey, WeightType>> edges2 = AdjacencyData.get(to);
        for (int i = 0; i < edges2.GetLength(); i++) {
            if (edges2.GetElem(i).GetNode() == from)
                return;
        }
        edges2.Append(MyWeightedEdge<TKey, WeightType>(from, weight));
        AdjacencyData.insert(to, edges2);
    }

    void DisconnectNodes(const TKey& from, const TKey& to) {
        if (!AdjacencyData.exist(from) || !AdjacencyData.exist(to))
            return;

        DynamicArray<MyWeightedEdge<TKey, WeightType>> edges1 = AdjacencyData.get(from);
        for (int i = 0; i < edges1.GetLength(); i++) {
            if (edges1.GetElem(i).GetNode() == to) {
                edges1.RemoveAt(i);
                break;
            }
        }
        AdjacencyData.insert(from, edges1);

        DynamicArray<MyWeightedEdge<TKey, WeightType>> edges2 = AdjacencyData.get(to);
        for (int i = 0; i < edges2.GetLength(); i++) {
            if (edges2.GetElem(i).GetNode() == from) {
                edges2.RemoveAt(i);
                break;
            }
        }
        AdjacencyData.insert(to, edges2);
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
            if (nodeCount == 0) break;
            int a = std::rand() % nodeCount;
            int b = std::rand() % nodeCount;
            if (a == b) continue;
            WeightType w = minWeight + (static_cast<WeightType>(std::rand()) / RAND_MAX) * (maxWeight - minWeight);
            TKey from = Nodes.GetElem(a);
            TKey to = Nodes.GetElem(b);
            if (!AdjacencyData.exist(from) || !AdjacencyData.exist(to))
                continue;

            bool exists = false;
            DynamicArray<MyWeightedEdge<TKey, WeightType>> edgesFrom = AdjacencyData.get(from);
            for (int i = 0; i < edgesFrom.GetLength(); i++) {
                if (edgesFrom.GetElem(i).GetNode() == to) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                ConnectNodes(from, to, w);
                addedEdges++;
            }
        }
    }

    DynamicArray<TKey> ReconstructPath(int end, const DynamicArray<int>& predecessors) const {
        DynamicArray<TKey> path;
        int current = end;
        while (current != -1) {
            path.Append(Nodes.GetElem(current));
            current = predecessors.GetElem(current);
        }
        DynamicArray<TKey> reversedPath;
        for (int i = path.GetLength() - 1; i >= 0; i--) {
            reversedPath.Append(path.GetElem(i));
        }
        return reversedPath;
    }

    DynamicArray<PathInfo<TKey>> MinimalDistances(const TKey& startNode) const {
        int numNodes = Nodes.GetLength();
        DynamicArray<int> distances;
        distances.Reserve(numNodes);
        for (int i = 0; i < numNodes; i++) {
            distances.Append(std::numeric_limits<int>::max());
        }

        DynamicArray<int> predecessors;
        predecessors.Reserve(numNodes);
        for (int i = 0; i < numNodes; i++) {
            predecessors.Append(-1);
        }

        DynamicArray<bool> visited;
        visited.Reserve(numNodes);
        for (int i = 0; i < numNodes; i++) {
            visited.Append(false);
        }

        int startIndex = FindNodeIndex(startNode);
        if (startIndex == -1) {
            DynamicArray<PathInfo<TKey>> result;
            result.Reserve(numNodes);
            for (int i = 0; i < numNodes; i++) {
                PathInfo<TKey> pi;
                pi.distance = -1;
                result.Append(pi);
            }
            return result;
        }

        distances[startIndex] = 0;

        for (int step = 0; step < numNodes; step++) {
            int minDist = std::numeric_limits<int>::max();
            int minIndex = -1;
            for (int i = 0; i < numNodes; i++) {
                if (!visited.GetElem(i) && distances.GetElem(i) < minDist) {
                    minDist = distances.GetElem(i);
                    minIndex = i;
                }
            }

            if (minIndex == -1)
                break;

            visited[minIndex] = true;

            TKey currentNode = Nodes.GetElem(minIndex);
            if (AdjacencyData.exist(currentNode)) {
                DynamicArray<MyWeightedEdge<TKey, WeightType>> edges = AdjacencyData.get(currentNode);
                for (int e = 0; e < edges.GetLength(); e++) {
                    TKey neighbor = edges.GetElem(e).GetNode();
                    int neighborIdx = FindNodeIndex(neighbor);
                    if (neighborIdx == -1)
                        continue;
                    WeightType weight = edges.GetElem(e).GetWeight();

                    if (distances[minIndex] == std::numeric_limits<int>::max())
                        continue;

                    int tentativeDist = distances[minIndex] + static_cast<int>(weight);
                    if (tentativeDist < distances.GetElem(neighborIdx)) {
                        distances[neighborIdx] = tentativeDist;
                        predecessors[neighborIdx] = minIndex;
                    }
                }
            }
        }

        DynamicArray<PathInfo<TKey>> result;
        result.Reserve(numNodes);
        for (int i = 0; i < numNodes; i++) {
            PathInfo<TKey> pi;
            if (distances.GetElem(i) != std::numeric_limits<int>::max()) {
                pi.distance = distances.GetElem(i);
                pi.path = ReconstructPath(i, predecessors);
            }
            else {
                pi.distance = -1;
            }
            result.Append(pi);
        }
        return result;
    }

    void ClearGraph() {
        AdjacencyData.Clear();
        Nodes.Clear();
    }

    std::string ToString() const {
        std::string res;
        for (int i = 0; i < Nodes.GetLength(); i++) {
            TKey node = Nodes.GetElem(i);
            res += "Vertex: " + KeyToString(node) + " -> [";
            if (AdjacencyData.exist(node)) {
                auto edges = AdjacencyData.get(node);
                for (int j = 0; j < edges.GetLength(); j++) {
                    res += KeyToString(edges.GetElem(j).GetNode()) + " (weight="
                        + std::to_string(static_cast<double>(edges.GetElem(j).GetWeight())) + ")";
                    if (j < edges.GetLength() - 1)
                        res += ", ";
                }
            }
            res += "]\n";
        }
        return res;
    }

    std::string KeyToString(const TKey& key) const {
        if constexpr (std::is_same<TKey, std::string>::value) {
            return key;
        }
        else {
            return std::to_string(key);
        }
    }

    void SaveToFile(const char* filename) const {
        std::ofstream outFile(filename);
        if (!outFile.is_open()) {
            throw std::runtime_error("Failed to open file for saving.");
        }
        outFile << Nodes.GetLength() << "\n";
        for (int i = 0; i < Nodes.GetLength(); i++) {
            outFile << KeyToString(Nodes.GetElem(i)) << "\n";
        }
        for (int i = 0; i < Nodes.GetLength(); i++) {
            TKey node = Nodes.GetElem(i);
            if (AdjacencyData.exist(node)) {
                auto edges = AdjacencyData.get(node);
                for (int j = 0; j < edges.GetLength(); j++) {
                    outFile << KeyToString(node) << " "
                        << KeyToString(edges.GetElem(j).GetNode()) << " "
                        << edges.GetElem(j).GetWeight() << "\n";
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

    int GetNodeCount() const {
        return Nodes.GetLength();
    }

    TKey GetVertex(int index) const {
        if (index < 0 || index >= Nodes.GetLength())
            throw std::out_of_range("Index out of range");
        return Nodes.GetElem(index);
    }

    bool HasVertex(const TKey& vertex) const {
        return AdjacencyData.exist(vertex);
    }

    DynamicArray<MyWeightedEdge<TKey, WeightType>> GetEdges(const TKey& vertex) const {
        if (!AdjacencyData.exist(vertex))
            throw std::runtime_error("Vertex does not exist");
        return AdjacencyData.get(vertex);
    }

private:
    int FindNodeIndex(const TKey& node) const {
        for (int i = 0; i < Nodes.GetLength(); i++) {
            if (Nodes.GetElem(i) == node)
                return i;
        }
        return -1;
    }

public:
    DynamicArray<int> GraphColoring() const {
        int numNodes = Nodes.GetLength();
        DynamicArray<int> colors;
        colors.Reserve(numNodes);
        for (int i = 0; i < numNodes; i++) {
            colors.Append(-1);
        }

        if (numNodes > 0) {
            colors[0] = 0;
        }

        for (int i = 1; i < numNodes; i++) {
            DynamicArray<bool> available;
            available.Reserve(numNodes);
            for (int j = 0; j < numNodes; j++) {
                available.Append(true);
            }

            TKey currentNode = Nodes.GetElem(i);
            if (AdjacencyData.exist(currentNode)) {
                auto edges = AdjacencyData.get(currentNode);
                for (int e = 0; e < edges.GetLength(); e++) {
                    TKey neighbor = edges.GetElem(e).GetNode();
                    int neighborIdx = FindNodeIndex(neighbor);
                    if (neighborIdx != -1 && colors.GetElem(neighborIdx) != -1) {
                        available[colors.GetElem(neighborIdx)] = false;
                    }
                }
            }

            int cr;
            for (cr = 0; cr < numNodes; cr++) {
                if (available.GetElem(cr)) {
                    break;
                }
            }

            colors[i] = cr;
        }

        return colors;
    }
};