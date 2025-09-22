/**
 * @file MatrizAdjacencia.cpp
 * @brief Implementação dos métodos da classe MatrizAdjacencia.
 */

#include "MatrizAdjacencia.h"

 /**
  * @brief Construtor da MatrizAdjacencia.
  * @details Inicializa a matriz com o tamanho correto (N+1 x N+1) e preenche
  * todos os seus valores com 'false', indicando a ausência de arestas.
  */
MatrizAdjacencia::MatrizAdjacencia(int numVertices) : numeroDeVertices(numVertices) {
    // Bloco: Alocação e inicialização da matriz
    // Redimensiona a matriz para (numVertices + 1) x (numVertices + 1).
    // O '+1' é crucial para trabalharmos confortavelmente com a indexação a partir de 1.
    // O índice 0 de linhas e colunas não será utilizado.
    matriz.resize(numeroDeVertices + 1, std::vector<bool>(numeroDeVertices + 1, false));
}

/**
 * @brief Adiciona uma aresta na matriz.
 * @details Como o grafo é não direcionado, a conexão é mútua.
 * A matriz é simétrica, então marcamos tanto [u][v] quanto [v][u] como 'true'.
 */
void MatrizAdjacencia::adicionarAresta(int u, int v) {
    // Bloco: Validação de limites (opcional, mas boa prática)
    if (u > 0 && u <= numeroDeVertices && v > 0 && v <= numeroDeVertices) {
        matriz[u][v] = true;
        matriz[v][u] = true; // Garante a simetria para grafos não direcionados
    }
}

/**
 * @brief Calcula o grau de um vértice na matriz.
 * @details O grau é o número de arestas conectadas a um vértice, o que corresponde
 * ao número de células 'true' na linha (ou coluna) referente àquele vértice.
 */
int MatrizAdjacencia::obterGrau(int vertice) const {
    // Bloco: Validação de limites
    if (vertice <= 0 || vertice > numeroDeVertices) {
        return 0; // Ou lançar uma exceção
    }

    int grau = 0;
    // Bloco: Contagem de vizinhos
    // Itera por todas as colunas da linha do vértice para contar as conexões.
    for (int i = 1; i <= numeroDeVertices; ++i) {
        if (matriz[vertice][i]) {
            grau++;
        }
    }
    return grau;
}

/**
 * @brief Encontra todos os vizinhos de um vértice.
 * @details Percorre a linha da matriz correspondente ao vértice e coleta os
 * índices de todas as colunas marcadas como 'true'.
 */
std::vector<int> MatrizAdjacencia::obterVizinhos(int vertice) const {
    // Bloco: Validação de limites
    if (vertice <= 0 || vertice > numeroDeVertices) {
        return {}; // Retorna um vetor vazio se o vértice for inválido
    }

    std::vector<int> vizinhos;
    // Bloco: Coleta de vizinhos
    // Itera por todas as colunas e adiciona o índice 'i' à lista de vizinhos se houver uma aresta.
    for (int i = 1; i <= numeroDeVertices; ++i) {
        if (matriz[vertice][i]) {
            vizinhos.push_back(i);
        }
    }
    return vizinhos;
}