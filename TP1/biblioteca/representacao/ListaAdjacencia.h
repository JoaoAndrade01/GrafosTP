#pragma once
/**
 * @file ListaAdjacencia.h
 * @brief Defini��o da classe ListaAdjacencia, uma implementa��o de grafo
 * usando o formato otimizado Compressed Sparse Row (CSR).
 */

#include "Representacao.h"
#include <vector>
#include <utility> // Para std::pair

 /**
  * @class ListaAdjacencia
  * @brief Implementa��o da representa��o de um grafo usando a t�cnica CSR.
  * @details Esta classe armazena o grafo de forma muito compacta e eficiente
  * para consultas, ideal para grafos grandes e esparsos. A constru��o � feita
  * em duas fases: primeiro as arestas s�o armazenadas temporariamente, depois
  * a estrutura final � montada pelo m�todo finalizarConstrucao().
  */
class ListaAdjacencia : public Representacao {
public:
    /**
     * @brief Construtor da classe ListaAdjacencia.
     * @param numeroDeVertices O n�mero total de v�rtices que o grafo ter�.
     */
    explicit ListaAdjacencia(int numeroDeVertices);

    /**
     * @brief Destrutor padr�o.
     */
    ~ListaAdjacencia() override = default;

    // Bloco: Implementa��o do "contrato" da interface Representacao
    void adicionarAresta(int u, int v) override;
    void finalizarConstrucao() override;
    int obterGrau(int vertice) const override;
    std::vector<int> obterVizinhos(int vertice) const override;

private:
    int numeroDeVertices;
    int numeroDeArestas;

    // Estrutura principal do CSR, usada para consulta ap�s a finaliza��o
    std::vector<int> ponteirosInicio; // Equivalente a 'offsets'
    std::vector<int> listaVizinhos;   // Equivalente a 'nbrs'

    // Buffers tempor�rios usados apenas durante a fase de constru��o
    std::vector<int> grausTemporarios;                 // Equivalente a 'deg'
    std::vector<std::pair<int, int>> arestasTemporarias; // Equivalente a 'edges'
};