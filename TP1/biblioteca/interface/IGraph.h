#pragma once
#include <string>
#include <vector>
#include <functional>

enum class GraphRepresentation {
    ADJACENCY_LIST,
    ADJACENCY_MATRIX,
    ADJACENCY_VECTOR
};

class IGraph {
public:
    virtual ~IGraph() = default;

    virtual void addEdge(int u, int v) = 0;
    virtual int getNumVertices() const = 0;
    virtual int getNumEdges() const = 0;
    virtual int getDegree(int vertex) const = 0;
    virtual void forEachNeighbor(int u,const std::function<void(int)>& fn) const = 0; 



   /**
     * @brief Calcula mÈtricas do grafo e salva em um arquivo de saÌda.
     * Este mÈtodo È concreto e usa os mÈtodos virtuais acima.
     * @param outputFilePath O caminho para o arquivo de relatÛrio a ser gerado.
    */

    // Para estruturas que precisam ‚Äúfechar‚Äù a constru√ß√£o (CSR). Lista/Matriz n√£o precisam mexer.
    virtual void finalize() {}

    void generateReport(const std::string& outputFilePath) const;
};
