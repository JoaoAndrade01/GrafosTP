#pragma once
#include "IGraph.h"
#include <vector>

class AdjacencyMatrixGraph : public IGraph {
public:
    AdjacencyMatrixGraph(int numVertices);
    void addEdge(int u, int v) override;
    int getNumVertices() const override;
    int getNumEdges() const override;
    int getDegree(int vertex) const override;

private:
    int numVertices;
    int numEdges; // Novo: contador de arestas
    std::vector<std::vector<int>> matrix;
};