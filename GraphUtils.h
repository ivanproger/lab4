#pragma once
#include "Graph.h"
#include <limits>
#include <stdexcept>

template <typename TKey, typename WeightType>
DynamicArray<int> GraphColoring(const Graph<TKey, WeightType>& graph)
{
    int numNodes = graph.GetNodeCount();
    DynamicArray<int> colors;
    colors.Reserve(numNodes);

    for (int i = 0; i < numNodes; i++) {
        colors.Append(-1);
    }
    if (numNodes == 0) {
        return colors; 
    }

    colors[0] = 0;

    for (int i = 1; i < numNodes; i++) {
        DynamicArray<bool> available;
        available.Reserve(numNodes);
        for (int j = 0; j < numNodes; j++) {
            available.Append(true);
        }

        // Берём смежные вершины и отмечаем их цвета как недоступные
        TKey currentNode = graph.GetVertex(i);
        auto neighbors = graph.GetAdjacentVertices(currentNode);
        for (int idx = 0; idx < neighbors.GetLength(); idx++) {
            TKey neighbor = neighbors[idx].GetNode();
            int neighborIndex = graph.FindNodeIndex(neighbor);  
            if (neighborIndex != -1 && colors[neighborIndex] != -1) {
                available[colors[neighborIndex]] = false;
            }
        }

        // Находим первый доступный цвет
        int c;
        for (c = 0; c < numNodes; c++) {
            if (available[c]) {
                break;
            }
        }
        colors[i] = c;
    }

    return colors;
}

template <typename TKey>
DynamicArray<TKey> ReconstructPath(int endIndex, const DynamicArray<int>& predecessors, const Graph<TKey>& graph)
{
    DynamicArray<TKey> path;
    int current = endIndex;
    while (current != -1) {
        path.Append(graph.GetVertex(current));
        current = predecessors[current];
    }

    DynamicArray<TKey> reversedPath;
    for (int i = path.GetLength() - 1; i >= 0; i--) {
        reversedPath.Append(path[i]);
    }
    return reversedPath;
}

template <typename TKey, typename WeightType>
DynamicArray<PathInfo<TKey>> MinDistances(const Graph<TKey, WeightType>& graph, const TKey& startNode)
{
    int numNodes = graph.GetNodeCount();
    DynamicArray<int> dist;
    dist.Reserve(numNodes);

    for (int i = 0; i < numNodes; i++) {
        dist.Append(std::numeric_limits<int>::max());
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

    int startIndex = graph.FindNodeIndex(startNode);
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

    dist[startIndex] = 0;

    for (int step = 0; step < numNodes; step++) {
        int minDist = std::numeric_limits<int>::max();
        int minIndex = -1;
        for (int i = 0; i < numNodes; i++) {
            if (!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                minIndex = i;
            }
        }

        if (minIndex == -1)
            break; // больше нет достижимых вершин

        visited[minIndex] = true;

        auto neighbors = graph.GetAdjacentVertices(graph.GetVertex(minIndex));
        for (int e = 0; e < neighbors.GetLength(); e++) {
            TKey neighbor = neighbors[e].GetNode();
            WeightType w = neighbors[e].GetWeight();
            int neighborIdx = graph.FindNodeIndex(neighbor);
            if (neighborIdx == -1) continue;

            if (dist[minIndex] == std::numeric_limits<int>::max())
                continue;

            int newDist = dist[minIndex] + static_cast<int>(w);
            if (newDist < dist[neighborIdx]) {
                dist[neighborIdx] = newDist;
                predecessors[neighborIdx] = minIndex;
            }
        }
    }

    DynamicArray<PathInfo<TKey>> result;
    result.Reserve(numNodes);
    for (int i = 0; i < numNodes; i++) {
        PathInfo<TKey> pi;
        if (dist[i] != std::numeric_limits<int>::max()) {
            pi.distance = dist[i];
            pi.path = ReconstructPath(i, predecessors, graph);
        }
        else {
            pi.distance = -1;
        }
        result.Append(pi);
    }
    return result;
}