#pragma once
/**
 * @file MatrizAdjacenciaTriangular.h
 * @brief Definição da classe MatrizAdjacenciaTriangular, uma implementação otimizada
 * de grafo usando uma matriz de adjacência triangular.
 */

#include "Representacao.h"
#include <vector>
#include <algorithm>

class MatrizAdjacenciaTriangular : public Representacao {
public:
    explicit MatrizAdjacenciaTriangular(int numeroDeVertices);
    ~MatrizAdjacenciaTriangular() override = default;

    void adicionarAresta(int u, int v) override;
    int obterGrau(int vertice) const override;
    std::vector<int> obterVizinhos(int vertice) const override;
    void finalizarConstrucao() override { /* Não faz nada */ }

private:
    bool existeAresta(int u, int v) const;
    size_t calcularIndice(int u, int v) const;

    std::vector<bool> matrizTriangular;
    int numeroDeVertices;
};