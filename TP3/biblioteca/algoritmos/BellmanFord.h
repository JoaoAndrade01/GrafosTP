/**
 * @file BellmanFord.h
 * @brief Declaração da classe BellmanFord para cálculo de caminhos mínimos.
 * * O algoritmo de Bellman-Ford:
 * - Calcula caminhos mínimos de uma origem única.
 * - Suporta arestas com pesos negativos.
 * - É capaz de detectar ciclos de peso negativo alcançáveis a partir da origem.
 * - Complexidade: O(V * E).
 */

#pragma once

#include <vector>

 // Forward declaration da classe principal de grafos do TP3
class GrafoPesado;

/**
 * @brief Estrutura que armazena o resultado do algoritmo de Bellman-Ford.
 */
struct ResultadoBellmanFord {
    std::vector<double> dist;           ///< dist[v]: distância mínima da origem até v
    std::vector<int>    pai;            ///< pai[v]: predecessor de v no caminho mínimo
    bool                temCicloNegativo = false; ///< Indica se foi detectado um ciclo negativo
};

/**
 * @class BellmanFord
 * @brief Implementação do algoritmo de Bellman-Ford.
 */
class BellmanFord {
public:
    /**
     * @brief Executa o algoritmo de Bellman-Ford.
     * * @details Este método funciona tanto para grafos direcionados quanto não direcionados
     * (desde que representados pela classe GrafoPesado).
     * Nota: Em grafos não direcionados, uma única aresta negativa implica imediatamente
     * em um ciclo negativo (ida e volta), o que será detectado por este algoritmo.
     * * @param grafo  Referência constante para o grafo ponderado.
     * @param origem Vértice de origem (indexado a partir de 1).
     * * @throws std::runtime_error se a origem for inválida ou o grafo estiver vazio.
     * * @return Estrutura ResultadoBellmanFord contendo distâncias, predecessores
     * e flag de ciclo negativo.
     */
    ResultadoBellmanFord executar(const GrafoPesado& grafo, int origem) const;
};