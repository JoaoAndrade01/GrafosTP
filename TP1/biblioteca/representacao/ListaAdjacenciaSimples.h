#pragma once
/**
 * @file ListaAdjacenciaSimples.h
 * @brief Defini��o da classe ListaAdjacenciaSimples, uma implementa��o
 * cl�ssica de grafo usando um vetor de listas.
 */

#include "Representacao.h"
#include <vector>
#include <list>

 /**
  * @class ListaAdjacenciaSimples
  * @brief Implementa��o cl�ssica e did�tica de uma lista de adjac�ncia.
  * @details Utiliza um std::vector onde cada posi��o 'i' cont�m uma std::list
  * com os vizinhos do v�rtice 'i'. A constru��o do grafo � feita dinamicamente
  * a cada chamada de adicionarAresta().
  */
class ListaAdjacenciaSimples : public Representacao {
public:
    /**
     * @brief Construtor da classe ListaAdjacenciaSimples.
     * @param numeroDeVertices O n�mero total de v�rtices que o grafo ter�.
     */
    explicit ListaAdjacenciaSimples(int numeroDeVertices);

    /**
     * @brief Destrutor padr�o.
     */
    ~ListaAdjacenciaSimples() override = default;

    // Bloco: Implementa��o do "contrato" da interface Representacao
    void adicionarAresta(int u, int v) override;
    int obterGrau(int vertice) const override;
    std::vector<int> obterVizinhos(int vertice) const override;

private:
    int numeroDeVertices;
    // A estrutura de dados principal: um vetor de listas de inteiros.
    std::vector<std::list<int>> listaDeAdjacencia;
};