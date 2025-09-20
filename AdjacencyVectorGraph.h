#pragma once
#include "IGraph.h"
#include <vector>
#include <utility>
#include <functional>

class AdjacencyVectorGraph : public IGraph {
public:
    explicit AdjacencyVectorGraph(int numVertices);

    void addEdge(int u, int v) override;
    void finalize() override;
    int getNumVertices() const override { return numVertices; }
    int getNumEdges() const override { return numEdges; }
    int getDegree(int vertex) const override;

    // Novo: percorre vizinhos no CSR
    void forEachNeighbor(int u, const std::function<void(int)>& fn) const override;

private:
    int numVertices;
    int numEdges;

    // Estrutura CSR
    std::vector<int> offsets;
    std::vector<int> nbrs;

    // Buffers para construção
    std::vector<int> deg;
    std::vector<std::pair<int,int>> edges;
};
