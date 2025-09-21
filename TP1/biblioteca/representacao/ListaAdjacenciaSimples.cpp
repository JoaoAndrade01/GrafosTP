/**
 * @file ListaAdjacenciaSimples.cpp
 * @brief Implementação dos métodos da classe ListaAdjacenciaSimples.
 */

#include "ListaAdjacenciaSimples.h"
#include <stdexcept>

 /**
  * @brief Construtor da ListaAdjacenciaSimples.
  * @details Redimensiona o vetor principal para V+1 posições para suportar
  * a indexação de vértices a partir de 1.
  */
ListaAdjacenciaSimples::ListaAdjacenciaSimples(int numVertices)
    : numeroDeVertices(numVertices) {
    // Redimensiona o vetor para ter 'numVertices + 1' listas vazias.
    listaDeAdjacencia.resize(numVertices + 1);
}

/**
 * @brief Adiciona uma aresta ao grafo.
 * @details Como o grafo é não direcionado, a aresta é adicionada na lista
 * de ambos os vértices (u e v).
 */
void ListaAdjacenciaSimples::adicionarAresta(int u, int v) {
    // Bloco: Validação de limites
    if (u > 0 && u <= numeroDeVertices && v > 0 && v <= numeroDeVertices && u != v) {
        // Adiciona v à lista de vizinhos de u.
        listaDeAdjacencia[u].push_back(v);
        // Adiciona u à lista de vizinhos de v.
        listaDeAdjacencia[v].push_back(u);
    }
}

/**
 * @brief Obtém o grau de um vértice.
 * @details O grau de um vértice em uma lista de adjacência é simplesmente
 * o tamanho da lista de seus vizinhos. Operação muito eficiente.
 */
int ListaAdjacenciaSimples::obterGrau(int vertice) const {
    if (vertice > 0 && vertice <= numeroDeVertices) {
        return listaDeAdjacencia[vertice].size();
    }
    throw std::out_of_range("Vertice invalido ao obter grau.");
}

/**
 * @brief Obtém a lista de vizinhos de um vértice.
 * @details Converte a std::list interna para um std::vector para retornar
 * um tipo consistente, conforme definido pela interface Representacao.
 */
std::vector<int> ListaAdjacenciaSimples::obterVizinhos(int vertice) const {
    if (vertice <= 0 || vertice > numeroDeVertices) {
        throw std::out_of_range("Vertice invalido ao obter vizinhos.");
    }
    // Copia os elementos da lista de adjacência para um novo vetor e o retorna.
    const auto& vizinhos_lista = listaDeAdjacencia[vertice];
    return std::vector<int>(vizinhos_lista.begin(), vizinhos_lista.end());
}