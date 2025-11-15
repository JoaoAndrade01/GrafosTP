#pragma once

#include <vector>
#include <limits>
#include <stdexcept>
#include <queue>
#include <utility>

// Agora o algoritmo trabalha com o GrafoDirigidoPesado do TP3
class GrafoDirigidoPesado;

/**
 * @struct ResultadoDijkstra
 * @brief Armazena o resultado da execução do algoritmo de Dijkstra.
 */
struct ResultadoDijkstra {
    std::vector<double> dist;  // dist[v] = distância mínima da origem até v
    std::vector<int>    pai;   // pai[v]  = predecessor de v no caminho mínimo
};

/**
 * @class Dijkstra
 * @brief Encapsula as implementações do algoritmo de Dijkstra.
 */
class Dijkstra {
public:
    // Versão com heap (priority_queue)
    ResultadoDijkstra executarHeap(const GrafoDirigidoPesado& grafo, int origem);

    // Versão com vetor (O(V^2))
    ResultadoDijkstra executarVetor(const GrafoDirigidoPesado& grafo, int origem);
};

