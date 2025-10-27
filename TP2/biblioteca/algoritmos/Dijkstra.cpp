#include "Dijkstra.h"
#include "../interface/GrafoPesado.h" // Inclui nossa classe GrafoPesado
#include <vector>

// Define um valor grande para representar infinito
const double infinity = std::numeric_limits<double>::infinity();

/**
 * Implementação de Dijkstra com HEAP.
 */
ResultadoDijkstra Dijkstra::executarHeap(const GrafoPesado& grafo, int origem) {
    // 1. Verificação de Pré-condição
    if (grafo.temPesoNegativo()) {
        throw std::runtime_error("Algoritmo de Dijkstra nao suporta grafos com pesos negativos.");
    }

    // 2. Inicialização
    const int numVertices = grafo.obterNumeroVertices();
    ResultadoDijkstra resultado;
    resultado.dist.assign(numVertices + 1, infinity);
    resultado.pai.assign(numVertices + 1, -1); // -1 indica sem predecessor

    resultado.dist[origem] = 0.0;
    resultado.pai[origem] = 0; // Marca a origem com pai 0

    // Fila de prioridade (min-heap simulado com max-heap e pesos negativos)
    // Armazena pares { -distancia, vertice }
    std::priority_queue<std::pair<double, int>> pq;
    pq.push({ 0.0, origem });

    // 3. Loop Principal
    while (!pq.empty()) {
        double d_neg = pq.top().first; // Distância negativa
        int u = pq.top().second;
        pq.pop();

        // Verifica se é uma entrada obsoleta na fila
        if (-d_neg > resultado.dist[u]) {
            continue;
        }

        // 4. Relaxamento dos Vizinhos
        for (const auto& vizinho : grafo.obterVizinhosComPesos(u)) {
            int v = vizinho.id;
            double pesoUV = vizinho.peso;

            // Condição de relaxamento
            if (resultado.dist[u] + pesoUV < resultado.dist[v]) {
                resultado.dist[v] = resultado.dist[u] + pesoUV;
                resultado.pai[v] = u;
                // Adiciona a nova distância (negativa) à fila (simulando decrease-key)
                pq.push({ -resultado.dist[v], v });
            }
        }
    }
    return resultado;
}


/**
 * Implementação de Dijkstra com VETOR.
 */
ResultadoDijkstra Dijkstra::executarVetor(const GrafoPesado& grafo, int origem) {
    // 1. Verificação de Pré-condição
    if (grafo.temPesoNegativo()) {
        throw std::runtime_error("Algoritmo de Dijkstra nao suporta grafos com pesos negativos.");
    }

    // 2. Inicialização
    const int numVertices = grafo.obterNumeroVertices();
    ResultadoDijkstra resultado;
    resultado.dist.assign(numVertices + 1, infinity);
    resultado.pai.assign(numVertices + 1, -1);
    std::vector<bool> finalizado(numVertices + 1, false); // Marca vértices cujo caminho mínimo já foi encontrado

    resultado.dist[origem] = 0.0;
    resultado.pai[origem] = 0;

    // 3. Loop Principal (executa V vezes)
    for (int count = 0; count < numVertices; ++count) {
        // 3.a. Encontrar o vértice 'u' não finalizado com menor dist[u]
        double minDist = infinity;
        int u = -1;
        for (int v_atual = 1; v_atual <= numVertices; ++v_atual) {
            if (!finalizado[v_atual] && resultado.dist[v_atual] < minDist) {
                minDist = resultado.dist[v_atual];
                u = v_atual;
            }
        }

        // Se não encontrou (grafo desconexo ou todos alcançáveis já finalizados)
        if (u == -1) {
            break;
        }

        // Marca 'u' como finalizado
        finalizado[u] = true;

        // 4. Relaxamento dos Vizinhos de 'u'
        for (const auto& vizinho : grafo.obterVizinhosComPesos(u)) {
            int v = vizinho.id;
            double pesoUV = vizinho.peso;

            // Relaxa apenas se v não foi finalizado e o caminho é melhor
            if (!finalizado[v] && resultado.dist[u] != infinity && resultado.dist[u] + pesoUV < resultado.dist[v]) {
                resultado.dist[v] = resultado.dist[u] + pesoUV;
                resultado.pai[v] = u;
            }
        }
    }
    return resultado;
}