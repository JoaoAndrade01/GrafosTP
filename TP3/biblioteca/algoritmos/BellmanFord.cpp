/**
 * @file BellmanFord.cpp
 * @brief Implementação dos métodos da classe BellmanFord.
 */

#include "BellmanFord.h"
#include "../interface/GrafoDirigidoPesado.h"  // Usa o grafo dirigido pesado
#include <limits>
#include <stdexcept>

/**
 * Define um valor grande para representar o infinito.
 * Mantemos a mesma convenção de Dijkstra (double infinito).
 */
static const double INFINITO_BF = std::numeric_limits<double>::infinity();

ResultadoBellmanFord BellmanFord::executar(const GrafoDirigidoPesado& grafo, int origem) const {
    ResultadoBellmanFord resultado;

    const int numVertices = grafo.obterNumeroVertices();
    if (numVertices <= 0) {
        throw std::runtime_error("GrafoDirigidoPesado com numero de vertices invalido em Bellman-Ford.");
    }

    // Verificação básica da origem
    if (origem <= 0 || origem > numVertices) {
        throw std::runtime_error("Vertice de origem invalido em Bellman-Ford.");
    }

    // 1. Inicialização das estruturas de dados
    resultado.dist.assign(numVertices + 1, INFINITO_BF);
    resultado.pai.assign(numVertices + 1, -1);  // -1 => inalcançável
    resultado.temCicloNegativo = false;

    // Origem
    resultado.dist[origem] = 0.0;
    resultado.pai[origem] = 0;                  // 0 => marca a origem (mesma convenção do Dijkstra)

    // 2. Relaxamento das arestas (V - 1) vezes
    //
    // Estratégia:
    //  - Percorremos todos os vértices u = 1..N.
    //  - Para cada u, percorremos a lista de vizinhos com pesos (u -> v, w).
    //  - Relaxamos a aresta se encontrar caminho melhor:
    //        dist[v] > dist[u] + w
    //
    // Observação:
    //  - Só faz sentido relaxar se dist[u] for finita (u alcançável).
    for (int i = 1; i <= numVertices - 1; ++i) {
        bool houveAtualizacao = false;

        for (int u = 1; u <= numVertices; ++u) {
            double distU = resultado.dist[u];
            if (distU == INFINITO_BF) {
                // Se u é inalcançável, nenhuma aresta saindo dele ajudará
                continue;
            }

            // Percorre todos os vizinhos (arestas saindo de u)
            for (const auto& viz : grafo.obterVizinhosComPesos(u)) {
                int v = viz.id;
                double pesoUV = viz.peso;

                if (distU + pesoUV < resultado.dist[v]) {
                    resultado.dist[v] = distU + pesoUV;
                    resultado.pai[v] = u;
                    houveAtualizacao = true;
                }
            }
        }

        // Otimização: se em uma iteração não houve nenhuma atualização,
        // já encontramos todos os caminhos mínimos possíveis e podemos parar.
        if (!houveAtualizacao) {
            break;
        }
    }

    // 3. Detecção de ciclos de peso negativo alcançáveis a partir da origem
    //
    // Se ainda for possível relaxar alguma aresta, então existe um ciclo
    // de peso negativo (alcançável).
    for (int u = 1; u <= numVertices; ++u) {
        double distU = resultado.dist[u];
        if (distU == INFINITO_BF) {
            continue;
        }

        for (const auto& viz : grafo.obterVizinhosComPesos(u)) {
            int v = viz.id;
            double pesoUV = viz.peso;

            if (distU + pesoUV < resultado.dist[v]) {
                resultado.temCicloNegativo = true;
                // Podemos sair cedo; já sabemos que há ciclo negativo.
                // Se você quiser, poderia coletar mais informações aqui,
                // mas para o TP normalmente basta o "flag".
                return resultado;
            }
        }
    }

    return resultado;
}
