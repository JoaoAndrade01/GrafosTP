#pragma once

#include <vector>
#include <limits>   // Para std::numeric_limits
#include <stdexcept> // Para std::runtime_error
#include <queue>     // Para std::priority_queue
#include <utility>   // Para std::pair

// Forward declaration da nossa interface de grafo pesado
class GrafoPesado;

/**
 * @struct ResultadoDijkstra
 * @brief Armazena o resultado da execução do algoritmo de Dijkstra.
 */
struct ResultadoDijkstra {
    std::vector<double> dist; // dist[v] = distância mínima da origem até v
    std::vector<int> pai;     // pai[v] = predecessor de v no caminho mínimo
};

/**
 * @class Dijkstra
 * @brief Encapsula as implementações do algoritmo de Dijkstra.
 */
class Dijkstra {
public:
    /**
     * @brief Executa o algoritmo de Dijkstra usando uma Fila de Prioridade (Heap).
     * @details Complexidade: O(E log V) com heap binário.
     * @param grafo O grafo ponderado (constante).
     * @param origem O vértice inicial da busca (indexado a partir de 1).
     * @return Um struct ResultadoDijkstra com as distâncias e predecessores.
     * @throws std::runtime_error se o grafo contiver pesos negativos.
     */
    ResultadoDijkstra executarHeap(const GrafoPesado& grafo, int origem);

    /**
     * @brief Executa o algoritmo de Dijkstra usando um Vetor para busca do mínimo.
     * @details Complexidade: O(V^2 + E) = O(V^2) em grafos densos.
     * @param grafo O grafo ponderado (constante).
     * @param origem O vértice inicial da busca (indexado a partir de 1).
     * @return Um struct ResultadoDijkstra com as distâncias e predecessores.
     * @throws std::runtime_error se o grafo contiver pesos negativos.
     */
    ResultadoDijkstra executarVetor(const GrafoPesado& grafo, int origem);
};