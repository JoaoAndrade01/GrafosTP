#include "GraphFactory.h"
#include "AdjacencyListGraph.h"
#include "AdjacencyMatrixGraph.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::unique_ptr<IGraph> GraphFactory::createGraphFromFile(
    const std::string& filePath,
    GraphRepresentation type
) {
    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo " << filePath << std::endl;
        return nullptr;
    }

    int numVertices;
    inputFile >> numVertices; // L� a primeira linha

    // Cria o tipo correto de grafo baseado na escolha do usu�rio
    std::unique_ptr<IGraph> graph;
    if (type == GraphRepresentation::ADJACENCY_LIST) {
        graph = std::make_unique<AdjacencyListGraph>(numVertices);
    }
    else {
        graph = std::make_unique<AdjacencyMatrixGraph>(numVertices);
    }

    int u, v;
    // L� as arestas restantes
    while (inputFile >> u >> v) {
        graph->addEdge(u, v);
    }

    inputFile.close();
    return graph;
}