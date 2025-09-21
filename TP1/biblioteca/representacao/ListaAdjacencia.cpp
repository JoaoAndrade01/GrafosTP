/**
 * @file ListaAdjacencia.cpp
 * @brief Implementação dos métodos da classe ListaAdjacencia.
 */

#include "ListaAdjacencia.h"
#include <stdexcept>
#include <algorithm>

 /**
  * @brief Construtor da ListaAdjacencia.
  * @details Inicializa os vetores temporários para a fase de construção.
  */
ListaAdjacencia::ListaAdjacencia(int n)
    : numeroDeVertices(n), numeroDeArestas(0),
    ponteirosInicio(n + 2, 0), grausTemporarios(n + 1, 0) {
    if (n <= 0) {
        throw std::invalid_argument("O numero de vertices deve ser positivo.");
    }
}

/**
 * @brief Adiciona uma aresta a um buffer temporário.
 * @details A aresta não é inserida na estrutura final do grafo aqui. Ela é
 * apenas armazenada para ser processada posteriormente por finalizarConstrucao().
 */
void ListaAdjacencia::adicionarAresta(int u, int v) {
    if (u > 0 && u <= numeroDeVertices && v > 0 && v <= numeroDeVertices && u != v) {
        arestasTemporarias.emplace_back(u, v);
        // O número de arestas será contado no final, mas podemos pré-incrementar
        // this->numeroDeArestas++; // Opcional, será sobrescrito
    }
}

/**
 * @brief Constrói a estrutura CSR final a partir das arestas armazenadas.
 * @details Este é o passo mais importante. Ele calcula os graus, determina os
 * ponteiros de início de cada lista de vizinhos e preenche a lista de vizinhos final.
 */
void ListaAdjacencia::finalizarConstrucao() {
    this->numeroDeArestas = arestasTemporarias.size();

    // Bloco 1: Contagem dos graus de cada vértice
    for (const auto& aresta : arestasTemporarias) {
        grausTemporarios[aresta.first]++;
        grausTemporarios[aresta.second]++;
    }

    // Bloco 2: Cálculo da soma de prefixos para determinar os ponteiros de início
    // ponteirosInicio[i] guardará o índice onde a lista de vizinhos do vértice 'i' começa.
    ponteirosInicio[1] = 0;
    for (int i = 1; i <= numeroDeVertices; ++i) {
        ponteirosInicio[i + 1] = ponteirosInicio[i] + grausTemporarios[i];
    }

    // Bloco 3: Preenchimento do vetor de vizinhos (listaVizinhos)
    listaVizinhos.assign(ponteirosInicio[numeroDeVertices + 1], -1);
    std::vector<int> ponteirosEscrita = ponteirosInicio;
    for (const auto& aresta : arestasTemporarias) {
        int u = aresta.first;
        int v = aresta.second;
        // Adiciona v na lista de u e u na lista de v
        listaVizinhos[ponteirosEscrita[u]++] = v;
        listaVizinhos[ponteirosEscrita[v]++] = u;
    }

    // Bloco 4: Ordenação das listas de vizinhos de cada vértice (opcional, para consistência)
    for (int u = 1; u <= numeroDeVertices; ++u) {
        std::sort(listaVizinhos.begin() + ponteirosInicio[u],
            listaVizinhos.begin() + ponteirosInicio[u + 1]);
    }

    // Bloco 5: Liberação da memória dos buffers temporários que não são mais necessários
    arestasTemporarias.clear();
    arestasTemporarias.shrink_to_fit();
    grausTemporarios.clear();
    grausTemporarios.shrink_to_fit();
}

/**
 * @brief Obtém o grau de um vértice.
 * @details A estrutura CSR permite calcular o grau em tempo O(1) com uma
 * simples subtração dos ponteiros de início.
 */
int ListaAdjacencia::obterGrau(int vertice) const {
    if (vertice > 0 && vertice <= numeroDeVertices) {
        return ponteirosInicio[vertice + 1] - ponteirosInicio[vertice];
    }
    throw std::out_of_range("Vertice invalido ao obter grau.");
}

/**
 * @brief Obtém a lista de vizinhos de um vértice.
 * @details Utiliza os ponteiros de início para localizar a fatia correta
 * do vetor listaVizinhos e retorna uma cópia dela.
 */
std::vector<int> ListaAdjacencia::obterVizinhos(int vertice) const {
    if (vertice <= 0 || vertice > numeroDeVertices) {
        throw std::out_of_range("Vertice invalido ao obter vizinhos.");
    }
    // Encontra o início e o fim da lista de vizinhos para este vértice
    auto inicio = listaVizinhos.begin() + ponteirosInicio[vertice];
    auto fim = listaVizinhos.begin() + ponteirosInicio[vertice + 1];

    // Cria e retorna um novo vetor contendo apenas os vizinhos deste vértice
    return std::vector<int>(inicio, fim);
}