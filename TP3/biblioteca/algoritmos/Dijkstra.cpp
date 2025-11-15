/**
 * @file Dijkstra.cpp
 * @brief Implementação do algoritmo de Dijkstra (versões Heap e Vetor)
 *        para grafos dirigidos com pesos (GrafoDirigidoPesado).
 */

#include "Dijkstra.h"
#include "../interface/GrafoDirigidoPesado.h"

#include <queue>
#include <limits>
#include <stdexcept>
#include <functional> // std::greater

// Alias para infinito
static const double INF_D = std::numeric_limits<double>::infinity();

/**
 * @brief Executa Dijkstra usando fila de prioridade (heap binário).
 *
 * @param grafo  Grafo dirigido com pesos (não pode ter pesos negativos).
 * @param origem Vértice de origem (1..N).
 *
 * @return ResultadoDijkstra com vetores dist e pai preenchidos.
 */
ResultadoDijkstra Dijkstra::executarHeap(const GrafoDirigidoPesado& grafo, int origem) {
    if (grafo.temPesoNegativo()) {
        throw std::runtime_error("Dijkstra (heap) nao aceita pesos negativos.");
    }

    const int numVertices = grafo.obterNumeroVertices();
    if (origem < 1 || origem > numVertices) {
        throw std::runtime_error("Vertice de origem invalido em Dijkstra (heap).");
    }

    ResultadoDijkstra resultado;
    resultado.dist.assign(numVertices + 1, INF_D);
    resultado.pai.assign(numVertices + 1, -1);

    using ParDistVert = std::pair<double, int>;
    std::priority_queue<
        ParDistVert,
        std::vector<ParDistVert>,
        std::greater<ParDistVert>
    > fila;

    resultado.dist[origem] = 0.0;
    fila.emplace(0.0, origem);

    while (!fila.empty()) {
        auto [distAtual, u] = fila.top();
        fila.pop();

        // Se já temos uma distância melhor registrada, ignorar esse par
        if (distAtual > resultado.dist[u]) {
            continue;
        }

        // Para cada vizinho v de u
        const auto vizinhos = grafo.obterVizinhosComPesos(u);
        for (const auto& viz : vizinhos) {
            int v       = viz.id;
            double w_uv = viz.peso;

            double novaDist = resultado.dist[u] + w_uv;
            if (novaDist < resultado.dist[v]) {
                resultado.dist[v] = novaDist;
                resultado.pai[v]  = u;
                fila.emplace(novaDist, v);
            }
        }
    }

    return resultado;
}

/**
 * @brief Executa Dijkstra usando vetor (complexidade O(V^2)).
 *
 * @param grafo  Grafo dirigido com pesos (não pode ter pesos negativos).
 * @param origem Vértice de origem (1..N).
 *
 * @return ResultadoDijkstra com vetores dist e pai preenchidos.
 */
ResultadoDijkstra Dijkstra::executarVetor(const GrafoDirigidoPesado& grafo, int origem) {
    if (grafo.temPesoNegativo()) {
        throw std::runtime_error("Dijkstra (vetor) nao aceita pesos negativos.");
    }

    const int numVertices = grafo.obterNumeroVertices();
    if (origem < 1 || origem > numVertices) {
        throw std::runtime_error("Vertice de origem invalido em Dijkstra (vetor).");
    }

    ResultadoDijkstra resultado;
    resultado.dist.assign(numVertices + 1, INF_D);
    resultado.pai.assign(numVertices + 1, -1);

    std::vector<bool> visitado(numVertices + 1, false);
    resultado.dist[origem] = 0.0;

    // Laço principal: em cada passo escolhe o vértice não visitado
    // com menor distância conhecida
    for (int iter = 1; iter <= numVertices; ++iter) {
        int u = -1;
        double melhorDist = INF_D;

        // Seleciona o vértice u não visitado de menor dist[u]
        for (int v = 1; v <= numVertices; ++v) {
            if (!visitado[v] && resultado.dist[v] < melhorDist) {
                melhorDist = resultado.dist[v];
                u = v;
            }
        }

        if (u == -1) {
            // Não há mais vértices alcançáveis
            break;
        }

        visitado[u] = true;

        // Relaxa as arestas de u
        const auto vizinhos = grafo.obterVizinhosComPesos(u);
        for (const auto& viz : vizinhos) {
            int v       = viz.id;
            double w_uv = viz.peso;

            double novaDist = resultado.dist[u] + w_uv;
            if (novaDist < resultado.dist[v]) {
                resultado.dist[v] = novaDist;
                resultado.pai[v]  = u;
            }
        }
    }

    return resultado;
}
