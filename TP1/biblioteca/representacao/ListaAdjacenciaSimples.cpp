/**
 * @file ListaAdjacenciaSimples.cpp
 * @brief Implementa��o dos m�todos da classe ListaAdjacenciaSimples.
 */

#include "ListaAdjacenciaSimples.h"
#include <stdexcept>

 /**
  * @brief Construtor da ListaAdjacenciaSimples.
  * @details Redimensiona o vetor principal para V+1 posi��es para suportar
  * a indexa��o de v�rtices a partir de 1.
  */
ListaAdjacenciaSimples::ListaAdjacenciaSimples(int numVertices)
    : numeroDeVertices(numVertices) {
    // Redimensiona o vetor para ter 'numVertices + 1' listas vazias.
    listaDeAdjacencia.resize(numVertices + 1);
}

/**
 * @brief Adiciona uma aresta ao grafo.
 * @details Como o grafo � n�o direcionado, a aresta � adicionada na lista
 * de ambos os v�rtices (u e v).
 */
void ListaAdjacenciaSimples::adicionarAresta(int u, int v) {
    // Bloco: Valida��o de limites
    if (u > 0 && u <= numeroDeVertices && v > 0 && v <= numeroDeVertices && u != v) {
        // Adiciona v � lista de vizinhos de u.
        listaDeAdjacencia[u].push_back(v);
        // Adiciona u � lista de vizinhos de v.
        listaDeAdjacencia[v].push_back(u);
    }
}

/**
 * @brief Obt�m o grau de um v�rtice.
 * @details O grau de um v�rtice em uma lista de adjac�ncia � simplesmente
 * o tamanho da lista de seus vizinhos. Opera��o muito eficiente.
 */
int ListaAdjacenciaSimples::obterGrau(int vertice) const {
    if (vertice > 0 && vertice <= numeroDeVertices) {
        return listaDeAdjacencia[vertice].size();
    }
    throw std::out_of_range("Vertice invalido ao obter grau.");
}

/**
 * @brief Obt�m a lista de vizinhos de um v�rtice.
 * @details Converte a std::list interna para um std::vector para retornar
 * um tipo consistente, conforme definido pela interface Representacao.
 */
std::vector<int> ListaAdjacenciaSimples::obterVizinhos(int vertice) const {
    if (vertice <= 0 || vertice > numeroDeVertices) {
        throw std::out_of_range("Vertice invalido ao obter vizinhos.");
    }
    // Copia os elementos da lista de adjac�ncia para um novo vetor e o retorna.
    const auto& vizinhos_lista = listaDeAdjacencia[vertice];
    return std::vector<int>(vizinhos_lista.begin(), vizinhos_lista.end());
}