/**
 * @file MatrizAdjacencia.cpp
 * @brief Implementa��o dos m�todos da classe MatrizAdjacencia.
 */

#include "MatrizAdjacencia.h"

 /**
  * @brief Construtor da MatrizAdjacencia.
  * @details Inicializa a matriz com o tamanho correto (N+1 x N+1) e preenche
  * todos os seus valores com 'false', indicando a aus�ncia de arestas.
  */
MatrizAdjacencia::MatrizAdjacencia(int numVertices) : numeroDeVertices(numVertices) {
    // Bloco: Aloca��o e inicializa��o da matriz
    // Redimensiona a matriz para (numVertices + 1) x (numVertices + 1).
    // O '+1' � crucial para trabalharmos confortavelmente com a indexa��o a partir de 1.
    // O �ndice 0 de linhas e colunas n�o ser� utilizado.
    matriz.resize(numeroDeVertices + 1, std::vector<bool>(numeroDeVertices + 1, false));
}

/**
 * @brief Adiciona uma aresta na matriz.
 * @details Como o grafo � n�o direcionado, a conex�o � m�tua.
 * A matriz � sim�trica, ent�o marcamos tanto [u][v] quanto [v][u] como 'true'.
 */
void MatrizAdjacencia::adicionarAresta(int u, int v) {
    // Bloco: Valida��o de limites (opcional, mas boa pr�tica)
    if (u > 0 && u <= numeroDeVertices && v > 0 && v <= numeroDeVertices) {
        matriz[u][v] = true;
        matriz[v][u] = true; // Garante a simetria para grafos n�o direcionados
    }
}

/**
 * @brief Calcula o grau de um v�rtice na matriz.
 * @details O grau � o n�mero de arestas conectadas a um v�rtice, o que corresponde
 * ao n�mero de c�lulas 'true' na linha (ou coluna) referente �quele v�rtice.
 */
int MatrizAdjacencia::obterGrau(int vertice) const {
    // Bloco: Valida��o de limites
    if (vertice <= 0 || vertice > numeroDeVertices) {
        return 0; // Ou lan�ar uma exce��o
    }

    int grau = 0;
    // Bloco: Contagem de vizinhos
    // Itera por todas as colunas da linha do v�rtice para contar as conex�es.
    for (int i = 1; i <= numeroDeVertices; ++i) {
        if (matriz[vertice][i]) {
            grau++;
        }
    }
    return grau;
}

/**
 * @brief Encontra todos os vizinhos de um v�rtice.
 * @details Percorre a linha da matriz correspondente ao v�rtice e coleta os
 * �ndices de todas as colunas marcadas como 'true'.
 */
std::vector<int> MatrizAdjacencia::obterVizinhos(int vertice) const {
    // Bloco: Valida��o de limites
    if (vertice <= 0 || vertice > numeroDeVertices) {
        return {}; // Retorna um vetor vazio se o v�rtice for inv�lido
    }

    std::vector<int> vizinhos;
    // Bloco: Coleta de vizinhos
    // Itera por todas as colunas e adiciona o �ndice 'i' � lista de vizinhos se houver uma aresta.
    for (int i = 1; i <= numeroDeVertices; ++i) {
        if (matriz[vertice][i]) {
            vizinhos.push_back(i);
        }
    }
    return vizinhos;
}