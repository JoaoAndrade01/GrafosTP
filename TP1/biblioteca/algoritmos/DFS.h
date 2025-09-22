#pragma once
/**
 * @file DFS.h
 * @brief Defini��o da classe DFS, que encapsula o algoritmo de Busca em Profundidade.
 */

 // A classe DFS precisa saber o que � um Grafo para poder operar sobre ele.
 // Usamos uma declara��o antecipada para evitar inclus�o circular.
class Grafo; 

#include <vector>

/**
 * @struct ResultadoDFS
 * @brief Armazena o resultado de uma execu��o da Busca em Profundidade (DFS).
 */
struct ResultadoDFS {
    std::vector<int> pai;   // pai[v] = antecessor de v na �rvore de busca
    std::vector<int> nivel; // nivel[v] = profundidade de v na �rvore de busca
};

/**
 * @class DFS
 * @brief Executa o algoritmo de Busca em Profundidade iterativo sobre um objeto Grafo.
 */
class DFS {
public:
    /**
     * @brief Executa a busca em profundidade iterativa usando uma pilha expl�cita.
     * @param grafo O grafo (constante) sobre o qual a busca ser� executada.
     * @param verticeOrigem O v�rtice inicial da busca.
     * @return Um struct ResultadoDFS com a �rvore de busca.
     */
    ResultadoDFS executar(const Grafo& grafo, int verticeOrigem) const;
};