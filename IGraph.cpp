#include "IGraph.h"
#include <vector>
#include <fstream>
#include <numeric>   // Para std::accumulate
#include <algorithm> // Para std::min_element, std::max_element, std::sort
#include <iostream>  // Para std::cerr

void IGraph::generateReport(const std::string& outputFilePath) const {
    std::ofstream outputFile(outputFilePath);
    if (!outputFile.is_open()) {
        std::cerr << "Erro: Nao foi possivel criar o arquivo de saida " << outputFilePath << std::endl;
        return;
    }

    int nVertices = getNumVertices();
    int nEdges = getNumEdges();

    if (nVertices == 0) {
        outputFile << "Grafo vazio." << std::endl;
        return;
    }

    std::vector<int> degrees;
    degrees.reserve(nVertices);
    for (int i = 1; i <= nVertices; ++i) {
        degrees.push_back(getDegree(i));
    }

    int minDegree = *std::min_element(degrees.begin(), degrees.end());
    int maxDegree = *std::max_element(degrees.begin(), degrees.end());
    double avgDegree = static_cast<double>(2.0 * nEdges) / nVertices;

    // Cálculo da Mediana
    std::sort(degrees.begin(), degrees.end());
    double medianDegree;
    if (nVertices % 2 == 0) {
        medianDegree = (degrees[nVertices / 2 - 1] + degrees[nVertices / 2]) / 2.0;
    }
    else {
        medianDegree = degrees[nVertices / 2];
    }

    // Escrevendo no arquivo
    outputFile << "# n_vertices: " << nVertices << std::endl;
    outputFile << "# n_arestas: " << nEdges << std::endl;
    outputFile << "# grau_minimo: " << minDegree << std::endl;
    outputFile << "# grau_maximo: " << maxDegree << std::endl;
    outputFile << "# grau_medio: " << avgDegree << std::endl;
    outputFile << "# mediana_grau: " << medianDegree << std::endl;

    outputFile.close();
}