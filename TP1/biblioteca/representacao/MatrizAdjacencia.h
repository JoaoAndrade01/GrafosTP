#pragma once
/**
 * @file MatrizAdjacencia.h
 * @brief Definição da classe MatrizAdjacencia, uma implementação de grafo
 * usando matriz de adjacência.
 */

#include "Representacao.h" // Inclui a interface que estamos implementando
#include <vector>

 /**
  * @class MatrizAdjacencia
  * @brief Implementação concreta da representação de um grafo usando uma matriz de adjacência.
  * @details Herda de Representacao e implementa seus métodos virtuais puros.
  * A matriz armazena 'true' se uma aresta existe entre dois vértices e 'false' caso contrário.
  */
class MatrizAdjacencia : public Representacao {
public:
    /**
     * @brief Construtor da classe MatrizAdjacencia.
     * @param numeroDeVertices O número total de vértices que o grafo terá.
     */
    explicit MatrizAdjacencia(int numeroDeVertices);

    /**
     * @brief Destrutor padrão.
     */
    ~MatrizAdjacencia() override = default;

    // Bloco: Implementação do "contrato" da interface Representacao
    void adicionarAresta(int u, int v) override;
    int obterGrau(int vertice) const override;
    std::vector<int> obterVizinhos(int vertice) const override;

private:
    // A matriz de adjacência. Usamos bool para economizar espaço.
    // O tamanho será (numeroDeVertices + 1) x (numeroDeVertices + 1) para suportar indexação a partir de 1.
    std::vector<std::vector<bool>> matriz;

    // Armazena o número de vértices para evitar o uso de matriz.size() repetidamente,
    // e para clareza no código.
    int numeroDeVertices;
};