#include "Dijkstra.h"

namespace tp2 {

DijkstraResultado dijkstra_vector(const Representacao& G, int s) {
    const int N = G.obterNumeroDeVertices();

    DijkstraResultado R;
    R.dist.assign(N + 1, INF);
    R.pai.assign (N + 1, -1);

    std::vector<char> vis(N + 1, 0);
    R.dist[s] = 0.0;

    for (int it = 0; it < N; ++it) {
        int u = -1;
        Dist best = INF;

        // escolhe o melhor não visitado (varredura linear)
        for (int v = 1; v <= N; ++v) {
            if (!vis[v] && R.dist[v] < best) { best = R.dist[v]; u = v; }
        }
        if (u == -1) break;
        vis[u] = 1;

        // relaxa
        G.paraCadaVizinho(u, [&](int v, double w) {
            if (std::isinf(R.dist[u])) return;
            Dist cand = R.dist[u] + w;
            if (cand < R.dist[v]) {
                R.dist[v] = cand;
                R.pai[v]  = u;
            }
        });
    }

    return R;
}

} // namespace tp2

