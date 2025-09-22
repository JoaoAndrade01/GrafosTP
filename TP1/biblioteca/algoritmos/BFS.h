#pragma once
/**
 * @file BFS.h
 * @brief Definição da classe BFS, que encapsula o algoritmo de Busca em Largura.
 */

 // A classe BFS precisa saber o que é um Grafo para poder operar sobre ele.
 // Usamos uma declaração antecipada para evitar inclusão circular.
class Grafo;

#include <vector>

/**
 * @struct ResultadoBFS
 * @brief Armazena o resultado de uma execução da Busca em Largura (BFS).
 * @details Movido para este arquivo pois é um resultado direto deste algoritmo.
 */
struct ResultadoBFS {
    std::vector<int> pai;   // pai[v] = antecessor de v na árvore de busca (0 para a raiz)
    std::vector<int> nivel; // nivel[v] = distância em arestas a partir da origem (-1 se inalcançável)
};

/**
 * @class BFS
 * @brief Executa o algoritmo de Busca em Largura sobre um objeto Grafo.
 */
class BFS {
public:
    /**
     * @brief Executa a busca em largura.
     * @param grafo O grafo (constante) sobre o qual a busca será executada.
     * @param verticeOrigem O vértice inicial da busca.
     * @return Um struct ResultadoBFS com a árvore de busca.
     */
    ResultadoBFS executar(const Grafo& grafo, int verticeOrigem) const;
};