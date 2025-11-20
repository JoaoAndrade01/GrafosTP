/**
 * @file BellmanFord.cpp
 * @brief Implementação dos métodos da classe BellmanFord.
 */

#include "BellmanFord.h"
#include "../interface/GrafoPesado.h"
#include <limits>
#include <stdexcept>

 // Define um valor grande para representar o infinito.
static const double INFINITO_BF = std::numeric_limits<double>::infinity();

ResultadoBellmanFord BellmanFord::executar(const GrafoPesado& grafo, int origem) const {
    ResultadoBellmanFord resultado;

    const int numVertices = grafo.obterNumeroVertices();
    if (numVertices <= 0) {
        throw std::runtime_error("GrafoPesado com numero de vertices invalido em Bellman-Ford.");
    }

    // Verificação básica da origem
    if (origem <= 0 || origem > numVertices) {
        throw std::runtime_error("Vertice de origem invalido em Bellman-Ford.");
    }

    // 1. Inicialização das estruturas de dados
    resultado.dist.assign(numVertices + 1, INFINITO_BF);
    resultado.pai.assign(numVertices + 1, -1);  // -1 => inalcançável ou sem pai
    resultado.temCicloNegativo = false;

    // Configuração da origem
    resultado.dist[origem] = 0.0;
    resultado.pai[origem] = 0; // 0 => marca a raiz da árvore de caminhos

    // 2. Relaxamento das arestas (V - 1) vezes
    //
    // O algoritmo relaxa todas as arestas do grafo V-1 vezes.
    // Graças à abstração do GrafoPesado/VetorAdjacenciaPesada, iterar pelos vizinhos
    // cobre corretamente todas as arestas, seja o grafo direcionado ou não.
    for (int i = 1; i <= numVertices - 1; ++i) {
        bool houveAtualizacao = false;

        // Itera sobre todos os vértices u
        for (int u = 1; u <= numVertices; ++u) {
            double distU = resultado.dist[u];

            // Se u é inalcançável, não podemos relaxar seus vizinhos
            if (distU == INFINITO_BF) {
                continue;
            }

            // Itera sobre todos os vizinhos v de u
            for (const auto& viz : grafo.obterVizinhosComPesos(u)) {
                int v = viz.id;
                double pesoUV = viz.peso;

                // Passo de relaxamento
                if (distU + pesoUV < resultado.dist[v]) {
                    resultado.dist[v] = distU + pesoUV;
                    resultado.pai[v] = u;
                    houveAtualizacao = true;
                }
            }
        }   

        // Otimização: Se nenhuma distância mudou nesta passagem, 
        // o caminho mínimo já foi encontrado.
        if (!houveAtualizacao) {
            break;
        }
    }

    // 3. Detecção de ciclos de peso negativo
    //
    // Uma passagem adicional para verificar se ainda é possível relaxar alguma aresta.
    // Se sim, existe um ciclo negativo alcançável a partir da origem.
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
                return resultado; // Retorna imediatamente ao detectar o ciclo
            }
        }
    }

    return resultado;
}