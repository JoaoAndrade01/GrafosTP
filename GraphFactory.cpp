#include "GraphFactory.h"
#include "AdjacencyListGraph.h"
#include "AdjacencyMatrixGraph.h"
#include "AdjacencyVectorGraph.h"   // << meu teste de  vetor adjacente
#include <fstream>
#include <sstream>
#include <iostream>

std::unique_ptr<IGraph> GraphFactory::createGraphFromFile(
    const std::string& filePath,
    GraphRepresentation type
){
    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo " << filePath << std::endl;
        return nullptr;
    }

    int numVertices = 0;
    inputFile >> numVertices; // primeira linha do arquivo: numero de vertices (1..n)

    // Cria o tipo correto de grafo baseado na escolha do usuario
    std::unique_ptr<IGraph> graph;
    if (type == GraphRepresentation::ADJACENCY_LIST) {
        graph = std::make_unique<AdjacencyListGraph>(numVertices);
    } else if (type == GraphRepresentation::ADJACENCY_MATRIX) {
        graph = std::make_unique<AdjacencyMatrixGraph>(numVertices);
    } else { // GraphRepresentation::ADJACENCY_VECTOR (CSR)
        graph = std::make_unique<AdjacencyVectorGraph>(numVertices);
    }

    // Le as arestas restantes (formato: u v), mantendo 1-based como no resto do projeto
    int u, v;
    while (inputFile >> u >> v) {
        graph->addEdge(u, v);
    }
    inputFile.close();

    // Fecha a construcao (CSR usa para montar offsets/nbrs; lista/matriz ignoram)
    graph->finalize();

    return graph;
}

