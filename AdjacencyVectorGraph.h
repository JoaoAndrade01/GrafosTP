#pragma once
#include "IGraph.h"
#include <vector>
#include <utility>

class AdjacencyVectorGraph : public IGraph {
public:
    explicit AdjacencyVectorGraph(int numVertices);

    void addEdge(int u, int v) override;   // recebe 1..n
    void finalize() override;              // monta CSR
    int  getNumVertices() const override { return numVertices; }
    int  getNumEdges()   const override { return numEdges; }
    int  getDegree(int vertex) const override;

private:
    int numVertices;
    int numEdges; // conta cada aresta uma vez (não-dirigido)

    // CSR (1-based): offsets tem tamanho numVertices+1 e usamos [1..n]
    std::vector<int> offsets;   // offsets[i] início da lista de i; offsets[i+1]-offsets[i] = grau(i)
    std::vector<int> nbrs;      // vizinhos concatenados (2*numEdges)

    // buffers para construção
    std::vector<int> deg;                      // graus 1..n
    std::vector<std::pair<int,int>> edges;     // guarda (u,v) até finalizar
};
