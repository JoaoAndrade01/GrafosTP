/**
 * @file BellmanFord.h
 * @brief Declaração da classe BellmanFord para cálculo de caminhos mínimos
 *        em grafos direcionados com pesos (incluindo pesos negativos).
 *
 * O algoritmo de Bellman-Ford:
 *  - Funciona com pesos negativos.
 *  - Detecta ciclos de peso negativo alcançáveis a partir da origem.
 *  - Complexidade: O(V * E), onde V é o número de vértices e E o número de arestas.
 */

#pragma once

#include <vector>

// Forward declaration para evitar dependência circular
class GrafoDirigidoPesado;

/**
 * @brief Estrutura que armazena o resultado do algoritmo de Bellman-Ford.
 *
 * Convenções:
 *  - dist[v]  : distância mínima da origem até o vértice v.
 *  - pai[v]   : predecessor imediato de v no caminho mínimo.
 *               * 0  => marca a origem (sem predecessor "real").
 *               * -1 => vértice inalcançável a partir da origem.
 *  - temCicloNegativo:
 *               * true  => existe ciclo de peso negativo alcançável pela origem.
 *               * false => não foi detectado ciclo negativo.
 */
struct ResultadoBellmanFord {
    std::vector<double> dist;
    std::vector<int>    pai;
    bool                temCicloNegativo = false;
};

/**
 * @brief Classe que implementa o algoritmo de Bellman-Ford.
 *
 * Uso típico:
 * @code
 *   GrafoDirigidoPesado g("grafo_W_1.txt");
 *   BellmanFord bf;
 *   ResultadoBellmanFord res = bf.executar(g, origem);
 *   if (res.temCicloNegativo) {
 *       // tratar ciclo negativo
 *   }
 * @endcode
 */
class BellmanFord {
public:
    /**
     * @brief Executa o algoritmo de Bellman-Ford em um grafo direcionado.
     *
     * @param grafo  Referência constante para o grafo direcionado e ponderado.
     * @param origem Vértice de origem (1..N).
     *
     * @throws std::runtime_error se a origem for inválida.
     *
     * @return Estrutura ResultadoBellmanFord com distâncias, pais e indicação
     *         de ciclo negativo.
     */
    ResultadoBellmanFord executar(const GrafoDirigidoPesado& grafo, int origem) const;
};
