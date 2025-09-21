#pragma once
/**
 * @file DFS.h
 * @brief Definição da classe DFS, que encapsula o algoritmo de Busca em Profundidade.
 */

 // A classe DFS precisa saber o que é um Grafo para poder operar sobre ele.
 // Usamos uma declaração antecipada para evitar inclusão circular.
class Grafo; 

#include <vector>

/**
 * @struct ResultadoDFS
 * @brief Armazena o resultado de uma execução da Busca em Profundidade (DFS).
 */
struct ResultadoDFS {
    std::vector<int> pai;   // pai[v] = antecessor de v na árvore de busca
    std::vector<int> nivel; // nivel[v] = profundidade de v na árvore de busca
};

/**
 * @class DFS
 * @brief Executa o algoritmo de Busca em Profundidade iterativo sobre um objeto Grafo.
 */
class DFS {
public:
    /**
     * @brief Executa a busca em profundidade iterativa usando uma pilha explícita.
     * @param grafo O grafo (constante) sobre o qual a busca será executada.
     * @param verticeOrigem O vértice inicial da busca.
     * @return Um struct ResultadoDFS com a árvore de busca.
     */
    ResultadoDFS executar(const Grafo& grafo, int verticeOrigem) const;
};