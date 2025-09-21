#pragma once
/**
 * @file ListaAdjacencia.h
 * @brief Definição da classe ListaAdjacencia, uma implementação de grafo
 * usando o formato otimizado Compressed Sparse Row (CSR).
 */

#include "Representacao.h"
#include <vector>
#include <utility> // Para std::pair

 /**
  * @class ListaAdjacencia
  * @brief Implementação da representação de um grafo usando a técnica CSR.
  * @details Esta classe armazena o grafo de forma muito compacta e eficiente
  * para consultas, ideal para grafos grandes e esparsos. A construção é feita
  * em duas fases: primeiro as arestas são armazenadas temporariamente, depois
  * a estrutura final é montada pelo método finalizarConstrucao().
  */
class ListaAdjacencia : public Representacao {
public:
    /**
     * @brief Construtor da classe ListaAdjacencia.
     * @param numeroDeVertices O número total de vértices que o grafo terá.
     */
    explicit ListaAdjacencia(int numeroDeVertices);

    /**
     * @brief Destrutor padrão.
     */
    ~ListaAdjacencia() override = default;

    // Bloco: Implementação do "contrato" da interface Representacao
    void adicionarAresta(int u, int v) override;
    void finalizarConstrucao() override;
    int obterGrau(int vertice) const override;
    std::vector<int> obterVizinhos(int vertice) const override;

private:
    int numeroDeVertices;
    int numeroDeArestas;

    // Estrutura principal do CSR, usada para consulta após a finalização
    std::vector<int> ponteirosInicio; // Equivalente a 'offsets'
    std::vector<int> listaVizinhos;   // Equivalente a 'nbrs'

    // Buffers temporários usados apenas durante a fase de construção
    std::vector<int> grausTemporarios;                 // Equivalente a 'deg'
    std::vector<std::pair<int, int>> arestasTemporarias; // Equivalente a 'edges'
};