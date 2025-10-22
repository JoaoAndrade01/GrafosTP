#include "ListaAdjacenciaPesada.h"
#include <algorithm>   // std::max
#include <utility>     // std::move

ListaAdjacenciaPesada::ListaAdjacenciaPesada(int n)
    : N(n), M(0), head(n + 2, 0) {}

void ListaAdjacenciaPesada::adicionarAresta(int u, int v, double w) {
    edges_u.push_back(u);
    edges_v.push_back(v);
    edges_w.push_back(w);
    ++M;
}

void ListaAdjacenciaPesada::finalizarConstrucao() {
    // 1) graus por vértice de origem
    std::vector<int> deg(N + 1, 0);
    for (int u : edges_u) ++deg[u];

    // 2) prefix-sum -> head[1..N+1]
    head[1] = 0;
    for (int u = 2; u <= N + 1; ++u) head[u] = head[u - 1] + deg[u - 1];

    // 3) preencher arrays CSR
    nbr.resize(M);
    wgt.resize(M);
    std::vector<int> cur = head;
    for (size_t i = 0; i < edges_u.size(); ++i) {
        int u = edges_u[i];
        int pos = cur[u]++;
        nbr[pos] = edges_v[i];
        wgt[pos] = edges_w[i];
    }

    // liberar buffers temporários
    edges_u.clear(); edges_u.shrink_to_fit();
    edges_v.clear(); edges_v.shrink_to_fit();
    edges_w.clear(); edges_w.shrink_to_fit();
}

int ListaAdjacenciaPesada::obterNumeroDeVertices() const { return N; }
long long ListaAdjacenciaPesada::obterNumeroDeArestas()  const { return M; }

void ListaAdjacenciaPesada::paraCadaVizinho(
    int u, const std::function<void(int,double)>& fn) const
{
    int ini = head[u], fim = head[u + 1];
    for (int i = ini; i < fim; ++i) fn(nbr[i], wgt[i]);
}

