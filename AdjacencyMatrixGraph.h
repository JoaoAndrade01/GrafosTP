#pragma once
#include "IGraph.h"
#include <vector>
#include <functional>

class AdjacencyMatrixGraph : public IGraph {
public:
    AdjacencyMatrixGraph(int numVertices);
    void addEdge(int u, int v) override;
    int getNumVertices() const override;
    int getNumEdges() const override;
    int getDegree(int vertex) const override;
    void forEachNeighbor(int u, const std::function<void(int)>& fn) const override;
private:
    int numVertices;
    int numEdges; // Novo: contador de arestas
    std::vector<std::vector<int>> matrix;
};
