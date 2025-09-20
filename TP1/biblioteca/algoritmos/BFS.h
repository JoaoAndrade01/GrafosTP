#pragma once
#include "IGraph.h"
#include <vector>
#include <queue>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <string>

/*
  - Estruturas de dados simples e baratas (vetores e fila) -> bom uso de cache.
  - Evitar "limpezas" O(N) a cada execução -> visitado com timestamp.
  - Não duplicar dados desnecessários -> resultados diretos (pai, nivel) e
    lista de vértices apenas onde o enunciado exige (componentes).

   Notação:
  N = número de vértices, M = número de arestas (não-direcionado conta 2 na representação).

  Resumo de complexidade:
  bfs(...)  ->  O(N + M) tempo,   O(N) memória (pai, nivel, fila).
  componentesConexas(...)   ->  O(N + M) tempo,   O(N) memória (visitado + fila) + soma dos tamanhos das componentes.
  calcularEstatisticasGrau(...) ->  Lista/CSR: O(N)   |  Matriz: O(N^2) (inevitável na matriz).
  salvarRelatorioCompleto(...)  ->  O(N + M) no pior (depende das rotinas acima) e gravação O(N).

  Observação crítica:
  - Em representação **matricial**, calcular grau exige varrer a linha (N colunas) -> O(N) por vértice.
    Portanto, *qualquer* abordagem de graus na matriz acabará O(N^2). Em lista/CSR é O(N).
*/

struct BFSResultado {
    std::vector<int> pai;    // pai[v] = antecessor de v na árvore de BFS (0 indica raiz)
    std::vector<int> nivel;  // nivel[v] = distância em arestas a partir da origem; -1 se inalcançável
};

struct EstatisticasGrau {
    int grauMin = 0;
    int grauMax = 0;
    double grauMedio = 0.0;
    double grauMediana = 0.0;
    std::vector<int> graus;  // guarda graus (1..N) para eventual auditoria/relatório
};

struct Componente {
    std::vector<int> vertices; // solicitado no enunciado (listar vértices por componente)
};

/*
   BFS (Busca em Largura)
   - Tempo: O(N + M)
   - Memória: O(N) para pai, nivel e estrutura da fila. O "visitado" usa um
     vetor estático com timestamp para evitar custo O(N) de limpar a cada execução. */

inline BFSResultado bfs(const IGraph& G, int origem) {
    const int n = G.getNumVertices();

    BFSResultado R;
    R.pai.assign(n + 1, -1);
    R.nivel.assign(n + 1, -1);

    // "visitado" com timestamp: evita memset O(N) a cada chamada.
    static std::vector<int> visitadoTick; // tamanho cresce até N e permanece
    static int tickGlobal = 1;
    if ((int)visitadoTick.size() < n + 1) visitadoTick.assign(n + 1, 0);
    const int tick = tickGlobal++;

    std::queue<int> fila;           // fila FIFO (deque por baixo, O(1) amortizado)
    visitadoTick[origem] = tick;
    R.pai[origem]   = 0;            // raiz
    R.nivel[origem] = 0;
    fila.push(origem);

    while (!fila.empty()) {
        int u = fila.front(); fila.pop();

        // Percurso genérico via IGraph::forEachNeighbor (lista/CSR: O(grau(u)), matriz: O(N) scan)
        G.forEachNeighbor(u, [&](int v){
            if (visitadoTick[v] != tick) {
                visitadoTick[v] = tick;
                R.pai[v]   = u;
                R.nivel[v] = R.nivel[u] + 1;
                fila.push(v);
            }
        });
    }
    return R;
}
   //Estatísticas de Grau
   //- Lista/CSR: getDegree(u) deve ser O(1) -> total O(N).
   //- Matriz: getDegree(u) tipicamente soma a linha -> O(N) por vértice -> O(N^2).
   //- Memória: O(N) para armazenar a lista de graus (útil para mediana).

inline EstatisticasGrau calcularEstatisticasGrau(const IGraph& G) {
    const int n = G.getNumVertices();
    EstatisticasGrau E;
    E.graus.resize(n + 1);

    long long soma = 0;
    for (int v = 1; v <= n; ++v) {
        int g = G.getDegree(v); // ver nota no cabeçalho sobre custo em matriz
        E.graus[v] = g;
        if (v == 1) { E.grauMin = E.grauMax = g; }
        else {
            E.grauMin = std::min(E.grauMin, g);
            E.grauMax = std::max(E.grauMax, g);
        }
        soma += g;
    }
    E.grauMedio = (n > 0) ? (double)soma / (double)n : 0.0;

    // Mediana -> precisamos de graus ordenados (cópia sem o índice 0)
    std::vector<int> ordenados(E.graus.begin() + 1, E.graus.end());
    std::sort(ordenados.begin(), ordenados.end());
    if (n > 0) {
        if (n & 1)   E.grauMediana = ordenados[n / 2];
        else         E.grauMediana = (ordenados[n/2 - 1] + ordenados[n/2]) / 2.0;
    }
    return E;
}


   //Componentes Conexas (via BFS)
   //Executa BFS múltiplas vezes (uma por componente).
   // Tempo: O(N + M)
   // Memória: O(N) (visitado + fila). Guardamos a lista de vértices de cada
    //componente porque o enunciado exige listagem completa.

inline std::vector<Componente> componentesConexas(const IGraph& G) {
    const int n = G.getNumVertices();
    std::vector<Componente> comps;

    std::vector<char> vis(n + 1, 0);  // char para economizar (1 byte por vértice)
    std::queue<int> fila;

    for (int s = 1; s <= n; ++s) {
        if (vis[s]) continue;

        // Inicia BFS a partir de s para descobrir a componente
        vis[s] = 1;
        fila.push(s);
        Componente C;
        C.vertices.reserve(8); // pequena reserva inicial (cresce conforme necessário)
        C.vertices.push_back(s);

        while (!fila.empty()) {
            int u = fila.front(); fila.pop();
            G.forEachNeighbor(u, [&](int v){
                if (!vis[v]) {
                    vis[v] = 1;
                    fila.push(v);
                    C.vertices.push_back(v);
                }
            });
        }
        comps.push_back(std::move(C));
    }

    // Ordem decrescente de tamanho (como pede o enunciado)
    std::sort(comps.begin(), comps.end(),
              [](const Componente& a, const Componente& b){
                  return a.vertices.size() > b.vertices.size();
              });
    return comps;
}

   // Relatório Completo
   // Grava: N, M, grau min/max/médio/mediana, componentes (com vértices),
    // e a árvore de BFS (pai e nivel).
   // Custos ficam concentrados nas funções chamadas acima.

inline void salvarRelatorioCompleto(const IGraph& G,
                                    const BFSResultado& R,
                                    int origem,
                                    const std::string& caminhoArquivo,
                                    double tempoConstrucaoSeg = 0.0,
                                    double tempoBFSSeg = 0.0) {
    std::ofstream out(caminhoArquivo);
    if (!out.is_open()) return;

    const int n = G.getNumVertices();
    const int m = G.getNumEdges();

    out << "Numero de vertices: " << n << "\n";
    out << "Numero de arestas: " << m << "\n";

    auto E = calcularEstatisticasGrau(G);
    out << "Grau minimo: "  << E.grauMin     << "\n";
    out << "Grau maximo: "  << E.grauMax     << "\n";
    out << "Grau medio: "   << E.grauMedio   << "\n";
    out << "Grau mediana: " << E.grauMediana << "\n\n";

    auto comps = componentesConexas(G);
    out << "Componentes conexas: " << comps.size() << "\n";
    for (size_t i = 0; i < comps.size(); ++i) {
        out << "Componente " << (i + 1)
            << " (tamanho=" << comps[i].vertices.size() << "): ";
        for (size_t j = 0; j < comps[i].vertices.size(); ++j) {
            if (j) out << ' ';
            out << comps[i].vertices[j];
        }
        out << "\n";
    }
    out << "\n";

    out << "BFS a partir do vertice " << origem << ":\n";
    out << "Vertice Pai Nivel\n";
    for (int v = 1; v <= n; ++v) {
        out << v << " " << R.pai[v] << " " << R.nivel[v] << "\n";
    }

    // Rodapé com tempos (úteis para análise comparativa Lista vs CSR vs Matriz)
    out << "\nTempo de construcao (segundos): " << tempoConstrucaoSeg << "\n";
    out << "Tempo de BFS (segundos): " << tempoBFSSeg << "\n";
}
