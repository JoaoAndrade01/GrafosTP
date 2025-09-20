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
     * @brief Calcula m�tricas do grafo e salva em um arquivo de sa�da.
     * Este m�todo � concreto e usa os m�todos virtuais acima.
     * @param outputFilePath O caminho para o arquivo de relat�rio a ser gerado.
    */

    // Para estruturas que precisam “fechar” a construção (CSR). Lista/Matriz não precisam mexer.
    virtual void finalize() {}

    void generateReport(const std::string& outputFilePath) const;
};
