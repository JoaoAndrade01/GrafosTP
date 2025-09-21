/**
 * @file BFS.cpp
 * @brief Implementação da classe BFS.
 */
#include "BFS.h"
#include "../../biblioteca/interface/Grafo.h" // Inclui a definição completa do Grafo
#include <queue>

ResultadoBFS BFS::executar(const Grafo& grafo, int verticeOrigem) const {
    // Bloco: Preparação
    const int numVertices = grafo.obterNumeroVertices();
    ResultadoBFS resultado;
    resultado.pai.assign(numVertices + 1, -1);
    resultado.nivel.assign(numVertices + 1, -1);

    // Bloco: Otimização de 'visitado' com timestamp
    static std::vector<int> visitadoTick;
    static int tickGlobal = 1;
    if ((int)visitadoTick.size() < numVertices + 1) {
        visitadoTick.assign(numVertices + 1, 0);
    }
    const int tickAtual = tickGlobal++;

    // Bloco: Inicialização da busca
    std::queue<int> fila;
    visitadoTick[verticeOrigem] = tickAtual;
    resultado.pai[verticeOrigem] = 0; // Raiz da busca
    resultado.nivel[verticeOrigem] = 0;
    fila.push(verticeOrigem);

    // Bloco: Loop principal do BFS
    while (!fila.empty()) {
        int u = fila.front();
        fila.pop();

        // Acessa o grafo através de sua interface pública
        for (int v : grafo.obterVizinhos(u)) {
            if (visitadoTick[v] != tickAtual) {
                visitadoTick[v] = tickAtual;
                resultado.pai[v] = u;
                resultado.nivel[v] = resultado.nivel[u] + 1;
                fila.push(v);
            }
        }
    }
    return resultado;
}