#include "AdjacencyVectorGraph.h"
#include <stdexcept>
#include <algorithm>

AdjacencyVectorGraph::AdjacencyVectorGraph(int n)
    : numVertices(n), numEdges(0), offsets(n+1, 0), deg(n+1, 0) {
    if (n <= 0) throw std::invalid_argument("numVertices inválido");
}

void AdjacencyVectorGraph::addEdge(int u, int v) {
    if (u > 0 && u <= numVertices && v > 0 && v <= numVertices && u != v) {
        edges.emplace_back(u, v);
        ++numEdges;
    }
}

void AdjacencyVectorGraph::finalize() {
    // Conta graus
    for (auto &e : edges) {
        int u = e.first, v = e.second;
        ++deg[u]; ++deg[v];
    }

    // Prefix-sum
    offsets[1] = 0;
    for (int i = 1; i <= numVertices; ++i) {
        offsets[i+1] = offsets[i] + deg[i];
    }

    // Preenche vizinhos
    nbrs.assign(offsets[numVertices+1], -1);
    std::vector<int> writePtr = offsets;
    for (auto &e : edges) {
        int u = e.first, v = e.second;
        nbrs[writePtr[u]++] = v;
        nbrs[writePtr[v]++] = u;
    }

    // Ordena vizinhos (para consistência)
    for (int u = 1; u <= numVertices; ++u) {
        std::sort(nbrs.begin() + offsets[u], nbrs.begin() + offsets[u+1]);
    }

    // Libera buffers
    edges.clear(); edges.shrink_to_fit();
    deg.clear(); deg.shrink_to_fit();
}

int AdjacencyVectorGraph::getDegree(int vertex) const {
    if (vertex > 0 && vertex <= numVertices) {
        return offsets[vertex+1] - offsets[vertex];
    }
    throw std::out_of_range("Vértice inválido.");
}

// 🔹 Implementação da interface uniforme
void AdjacencyVectorGraph::forEachNeighbor(int u, const std::function<void(int)>& fn) const {
    if (u <= 0 || u > numVertices) {
        throw std::out_of_range("Vértice inválido em forEachNeighbor");
    }
    for (int i = offsets[u]; i < offsets[u+1]; ++i) {
        fn(nbrs[i]);
    }
}
