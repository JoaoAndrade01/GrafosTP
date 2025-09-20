#include "AdjacencyListGraph.h"
#include <stdexcept>

AdjacencyListGraph::AdjacencyListGraph(int numVertices)
    : numVertices(numVertices), numEdges(0) {
    adj.resize(numVertices + 1); // usando 1-based
}

void AdjacencyListGraph::addEdge(int u, int v) {
    if (u > 0 && u <= numVertices && v > 0 && v <= numVertices) {
        adj[u].push_back(v);
        adj[v].push_back(u); // grafo não-dirigido
        numEdges++;
    }
}

int AdjacencyListGraph::getNumVertices() const {
    return numVertices;
}

int AdjacencyListGraph::getNumEdges() const {
    return numEdges;
}

int AdjacencyListGraph::getDegree(int vertex) const {
    if (vertex > 0 && vertex <= numVertices) {
        return adj[vertex].size();
    }
    throw std::out_of_range("Vértice inválido.");
}

// 🔹 Implementação da interface uniforme para BFS/DFS
void AdjacencyListGraph::forEachNeighbor(int u, const std::function<void(int)>& fn) const {
    if (u <= 0 || u > numVertices) {
        throw std::out_of_range("Vértice inválido em forEachNeighbor");
    }
    for (int v : adj[u]) {
        fn(v);
    }
}
