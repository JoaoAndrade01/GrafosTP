#include "GraphFactory.h"
#include "AdjacencyListGraph.h"
#include "AdjacencyMatrixGraph.h"
#include "AdjacencyVectorGraph.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <chrono>

BuildStats GraphFactory::s_stats{};

std::unique_ptr<IGraph> GraphFactory::createGraphFromFile(
    const std::string& filePath,
    GraphRepresentation type
) {
    using relogio = std::chrono::high_resolution_clock;

    // Abre arquivo
    std::ifstream in(filePath);
    if (!in.is_open()) {
        std::cerr << "Erro: nao foi possivel abrir o arquivo '" << filePath << "'.\n";
        return nullptr;
    }

    // Lê número de vértices
    int n = 0;
    if (!(in >> n) || n <= 0) {
        std::cerr << "Erro: primeira linha deve conter o numero de vertices (>0).\n";
        return nullptr;
    }

    // Lê todas as arestas em memória (NÃO conta tempo)
    std::vector<std::pair<int,int>> arestas;
    int u, v;
    while (in >> u >> v) {
        arestas.emplace_back(u, v);
    }
    in.close();

    // Começa a medir tempo só da CONSTRUÇÃO
    auto t0 = relogio::now();

    std::unique_ptr<IGraph> g;
    switch (type) {
        case GraphRepresentation::ADJACENCY_LIST:
            g = std::make_unique<AdjacencyListGraph>(n);
            break;
        case GraphRepresentation::ADJACENCY_MATRIX:
            g = std::make_unique<AdjacencyMatrixGraph>(n);
            break;
        case GraphRepresentation::ADJACENCY_VECTOR:
        default:
            g = std::make_unique<AdjacencyVectorGraph>(n);
            break;
    }

    for (auto &e : arestas) {
        g->addEdge(e.first, e.second);
    }

    // Finaliza (CSR precisa)
    g->finalize();

    auto t1 = relogio::now();
    s_stats.tempoConstrucaoSeg = std::chrono::duration<double>(t1 - t0).count();

    return g;
}

BuildStats GraphFactory::getLastBuildStats() {
    return s_stats;
}

