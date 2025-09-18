#include "AdjacencyMatrixGraph.h"
#include <numeric> // Para std::accumulate
#include <stdexcept>

AdjacencyMatrixGraph::AdjacencyMatrixGraph(int numVertices)
    : numVertices(numVertices), numEdges(0) { // Inicializa numEdges
    matrix.resize(numVertices + 1, std::vector<int>(numVertices + 1, 0));
}

void AdjacencyMatrixGraph::addEdge(int u, int v) {
    if (u > 0 && u <= numVertices && v > 0 && v <= numVertices) {
        if (matrix[u][v] == 0) { // Evita contar arestas duplicadas
            matrix[u][v] = 1;
            matrix[v][u] = 1;
            numEdges++; // Incrementa o contador
        }
    }
}

int AdjacencyMatrixGraph::getNumVertices() const {
    return numVertices;
}

int AdjacencyMatrixGraph::getNumEdges() const {
    return numEdges;
}

int AdjacencyMatrixGraph::getDegree(int vertex) const {
    if (vertex > 0 && vertex <= numVertices) {
        // Soma todos os elementos na linha correspondente ao vértice
        return std::accumulate(matrix[vertex].begin(), matrix[vertex].end(), 0);
    }
    throw std::out_of_range("Vertice invalido.");
}