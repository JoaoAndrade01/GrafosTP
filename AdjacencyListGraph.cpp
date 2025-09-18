#include "AdjacencyListGraph.h"
#include <stdexcept>

AdjacencyListGraph::AdjacencyListGraph(int numVertices)
    : numVertices(numVertices), numEdges(0) { // Inicializa numEdges
    adj.resize(numVertices + 1);
}

void AdjacencyListGraph::addEdge(int u, int v) {
    if (u > 0 && u <= numVertices && v > 0 && v <= numVertices) {
        adj[u].push_back(v);
        adj[v].push_back(u);
        numEdges++; // Incrementa o contador
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
    throw std::out_of_range("Vertice invalido.");
}