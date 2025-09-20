#pragma once
#include "IGraph.h"
#include <list>
#include <vector>
#include <functional>

class AdjacencyListGraph : public IGraph {
public:
    AdjacencyListGraph(int numVertices);

    void addEdge(int u, int v) override;
    int getNumVertices() const override;
    int getNumEdges() const override;
    int getDegree(int vertex) const override;

    // Novo: percorre todos os vizinhos de um vértice
    void forEachNeighbor(int u, const std::function<void(int)>& fn) const override;

private:
    int numVertices;
    int numEdges;
    std::vector<std::list<int>> adj; // lista de adjacência
};
