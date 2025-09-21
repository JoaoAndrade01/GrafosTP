/**
 * @file ListaAdjacencia.cpp
 * @brief Implementa��o dos m�todos da classe ListaAdjacencia.
 */

#include "ListaAdjacencia.h"
#include <stdexcept>
#include <algorithm>

 /**
  * @brief Construtor da ListaAdjacencia.
  * @details Inicializa os vetores tempor�rios para a fase de constru��o.
  */
ListaAdjacencia::ListaAdjacencia(int n)
    : numeroDeVertices(n), numeroDeArestas(0),
    ponteirosInicio(n + 2, 0), grausTemporarios(n + 1, 0) {
    if (n <= 0) {
        throw std::invalid_argument("O numero de vertices deve ser positivo.");
    }
}

/**
 * @brief Adiciona uma aresta a um buffer tempor�rio.
 * @details A aresta n�o � inserida na estrutura final do grafo aqui. Ela �
 * apenas armazenada para ser processada posteriormente por finalizarConstrucao().
 */
void ListaAdjacencia::adicionarAresta(int u, int v) {
    if (u > 0 && u <= numeroDeVertices && v > 0 && v <= numeroDeVertices && u != v) {
        arestasTemporarias.emplace_back(u, v);
        // O n�mero de arestas ser� contado no final, mas podemos pr�-incrementar
        // this->numeroDeArestas++; // Opcional, ser� sobrescrito
    }
}

/**
 * @brief Constr�i a estrutura CSR final a partir das arestas armazenadas.
 * @details Este � o passo mais importante. Ele calcula os graus, determina os
 * ponteiros de in�cio de cada lista de vizinhos e preenche a lista de vizinhos final.
 */
void ListaAdjacencia::finalizarConstrucao() {
    this->numeroDeArestas = arestasTemporarias.size();

    // Bloco 1: Contagem dos graus de cada v�rtice
    for (const auto& aresta : arestasTemporarias) {
        grausTemporarios[aresta.first]++;
        grausTemporarios[aresta.second]++;
    }

    // Bloco 2: C�lculo da soma de prefixos para determinar os ponteiros de in�cio
    // ponteirosInicio[i] guardar� o �ndice onde a lista de vizinhos do v�rtice 'i' come�a.
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

    // Bloco 4: Ordena��o das listas de vizinhos de cada v�rtice (opcional, para consist�ncia)
    for (int u = 1; u <= numeroDeVertices; ++u) {
        std::sort(listaVizinhos.begin() + ponteirosInicio[u],
            listaVizinhos.begin() + ponteirosInicio[u + 1]);
    }

    // Bloco 5: Libera��o da mem�ria dos buffers tempor�rios que n�o s�o mais necess�rios
    arestasTemporarias.clear();
    arestasTemporarias.shrink_to_fit();
    grausTemporarios.clear();
    grausTemporarios.shrink_to_fit();
}

/**
 * @brief Obt�m o grau de um v�rtice.
 * @details A estrutura CSR permite calcular o grau em tempo O(1) com uma
 * simples subtra��o dos ponteiros de in�cio.
 */
int ListaAdjacencia::obterGrau(int vertice) const {
    if (vertice > 0 && vertice <= numeroDeVertices) {
        return ponteirosInicio[vertice + 1] - ponteirosInicio[vertice];
    }
    throw std::out_of_range("Vertice invalido ao obter grau.");
}

/**
 * @brief Obt�m a lista de vizinhos de um v�rtice.
 * @details Utiliza os ponteiros de in�cio para localizar a fatia correta
 * do vetor listaVizinhos e retorna uma c�pia dela.
 */
std::vector<int> ListaAdjacencia::obterVizinhos(int vertice) const {
    if (vertice <= 0 || vertice > numeroDeVertices) {
        throw std::out_of_range("Vertice invalido ao obter vizinhos.");
    }
    // Encontra o in�cio e o fim da lista de vizinhos para este v�rtice
    auto inicio = listaVizinhos.begin() + ponteirosInicio[vertice];
    auto fim = listaVizinhos.begin() + ponteirosInicio[vertice + 1];

    // Cria e retorna um novo vetor contendo apenas os vizinhos deste v�rtice
    return std::vector<int>(inicio, fim);
}