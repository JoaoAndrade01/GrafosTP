/**
 * @file MatrizAdjacenciaTriangular.cpp
 * @brief Implementação dos métodos da classe MatrizAdjacenciaTriangular.
 */

#include "MatrizAdjacenciaTriangular.h"
#include <stdexcept>

MatrizAdjacenciaTriangular::MatrizAdjacenciaTriangular(int numVertices) : numeroDeVertices(numVertices) {
    long long tamanhoNecessario = (long long)numVertices * (numVertices + 1) / 2;
    matrizTriangular.resize(tamanhoNecessario, false);
}

size_t MatrizAdjacenciaTriangular::calcularIndice(int u, int v) const {
    if (u < v) std::swap(u, v);
    return (size_t)(((long long)u * (u - 1) / 2) + v - 1);
}

void MatrizAdjacenciaTriangular::adicionarAresta(int u, int v) {
    if (u <= 0 || u > numeroDeVertices || v <= 0 || v > numeroDeVertices || u == v) return;
    size_t indice = calcularIndice(u, v);
    matrizTriangular[indice] = true;
}

bool MatrizAdjacenciaTriangular::existeAresta(int u, int v) const {
    if (u <= 0 || u > numeroDeVertices || v <= 0 || v > numeroDeVertices || u == v) return false;
    size_t indice = calcularIndice(u, v);
    return matrizTriangular[indice];
}

int MatrizAdjacenciaTriangular::obterGrau(int vertice) const {
    if (vertice <= 0 || vertice > numeroDeVertices) {
        throw std::out_of_range("Vertice invalido ao obter grau.");
    }
    int grau = 0;
    for (int i = 1; i <= numeroDeVertices; ++i) {
        if (vertice != i && existeAresta(vertice, i)) {
            grau++;
        }
    }
    return grau;
}

std::vector<int> MatrizAdjacenciaTriangular::obterVizinhos(int vertice) const {
    if (vertice <= 0 || vertice > numeroDeVertices) {
        throw std::out_of_range("Vertice invalido ao obter vizinhos.");
    }
    std::vector<int> vizinhos;
    for (int i = 1; i <= numeroDeVertices; ++i) {
        if (vertice != i && existeAresta(vertice, i)) {
            vizinhos.push_back(i);
        }
    }
    return vizinhos;
}