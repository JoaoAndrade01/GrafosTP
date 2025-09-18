#pragma once
#include "IGraph.h"
#include <list>

class AdjacencyListGraph : public IGraph {
public:
    AdjacencyListGraph(int numVertices);
    void addEdge(int u, int v) override;
    int getNumVertices() const override;
    int getNumEdges() const override;
    int getDegree(int vertex) const override;

private:
    int numVertices;
    int numEdges;
    std::vector<std::list<int>> adj;
};