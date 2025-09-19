#include "AdjacencyVectorGraph.h"
#include <stdexcept>
#include <algorithm>

AdjacencyVectorGraph::AdjacencyVectorGraph(int n)
: numVertices(n), numEdges(0), offsets(n+1, 0), deg(n+1, 0) {
    if (n <= 0) throw std::invalid_argument("numVertices invalido");
}

void AdjacencyVectorGraph::addEdge(int u, int v) {
    // Mantém o mesmo padrão 1-based do projeto
    if (u > 0 && u <= numVertices && v > 0 && v <= numVertices && u != v) {
        edges.emplace_back(u, v);
        ++numEdges; // conta a aresta uma vez (não-dirigido)
    }
}

void AdjacencyVectorGraph::finalize() {
    // Conta graus (1-based)
    for (auto &e : edges) {
        int u = e.first, v = e.second;
        ++deg[u]; ++deg[v];
    }

    // Prefix-sum para offsets (1..n), offsets[1] = 0
    offsets[1] = 0;
    for (int i = 1; i <= numVertices; ++i) {
        offsets[i+1] = offsets[i] + deg[i];
    }

    // Aloca vizinhos e ponteiro de escrita
    nbrs.assign(offsets[numVertices+1], -1);
    std::vector<int> writePtr = offsets; // cópia

    // Preenche (não-dirigido: simétrico)
    for (auto &e : edges) {
        int u = e.first, v = e.second;
        nbrs[ writePtr[u]++ ] = v;
        nbrs[ writePtr[v]++ ] = u;
    }

    // Opcional: ordena vizinhos de cada vértice para reprodutibilidade
    for (int u = 1; u <= numVertices; ++u) {
        std::sort(nbrs.begin() + offsets[u], nbrs.begin() + offsets[u+1]);
    }

    // Libera buffers de construção
    edges.clear(); edges.shrink_to_fit();
    deg.clear();   deg.shrink_to_fit();
}

int AdjacencyVectorGraph::getDegree(int vertex) const {
    if (vertex > 0 && vertex <= numVertices) {
        return offsets[vertex+1] - offsets[vertex];
    }
    throw std::out_of_range("Vertice invalido.");
}
