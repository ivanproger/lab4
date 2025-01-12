#pragma once

template <typename TKey, typename WeightType = double>
class MyWeightedEdge
{
private:
    TKey node;        // Соседняя вершина
    WeightType edgeWeight;   // Вес ребра

public:
    MyWeightedEdge() : node(), edgeWeight(0) {}

    MyWeightedEdge(const TKey& vertex, WeightType weight)
        : node(vertex), edgeWeight(weight) {}

    TKey GetNode() const {
        return node;
    }

    WeightType GetWeight() const {
        return edgeWeight;
    }

    void SetNode(const TKey& newNode) {
        node = newNode;
    }

    void SetWeight(WeightType w) {
        edgeWeight = w;
    }

};