#pragma once
/**
 * @file ListaAdjacenciaSimples.h
 * @brief Definição da classe ListaAdjacenciaSimples, uma implementação
 * clássica de grafo usando um vetor de listas.
 */

#include "Representacao.h"
#include <vector>
#include <list>

 /**
  * @class ListaAdjacenciaSimples
  * @brief Implementação clássica e didática de uma lista de adjacência.
  * @details Utiliza um std::vector onde cada posição 'i' contém uma std::list
  * com os vizinhos do vértice 'i'. A construção do grafo é feita dinamicamente
  * a cada chamada de adicionarAresta().
  */
class ListaAdjacenciaSimples : public Representacao {
public:
    /**
     * @brief Construtor da classe ListaAdjacenciaSimples.
     * @param numeroDeVertices O número total de vértices que o grafo terá.
     */
    explicit ListaAdjacenciaSimples(int numeroDeVertices);

    /**
     * @brief Destrutor padrão.
     */
    ~ListaAdjacenciaSimples() override = default;

    // Bloco: Implementação do "contrato" da interface Representacao
    void adicionarAresta(int u, int v) override;
    int obterGrau(int vertice) const override;
    std::vector<int> obterVizinhos(int vertice) const override;

private:
    int numeroDeVertices;
    // A estrutura de dados principal: um vetor de listas de inteiros.
    std::vector<std::list<int>> listaDeAdjacencia;
};