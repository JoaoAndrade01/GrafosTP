#include "LeitorGrafoPesado.h"
#include <fstream>
#include <sstream>
#include <tuple>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include "../representacao/ListaAdjacenciaPesada.h"

std::unique_ptr<Representacao>
carregarGrafoPesadoAuto(const std::string& caminho, bool* temPesoNeg) {
    if (temPesoNeg) *temPesoNeg = false;

    std::ifstream in(caminho);
    if (!in.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir: " + caminho);
    }

    std::vector<std::tuple<int,int,double>> edges;
    edges.reserve(1 << 20);

    int u, v; double w;
    int N = 0;
    std::string line;

    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream iss(line);
        if (!(iss >> u >> v >> w)) continue;

        N = std::max(N, std::max(u, v));
        if (temPesoNeg && w < 0.0) *temPesoNeg = true;
        edges.emplace_back(u, v, w);
    }

    if (N == 0) throw std::runtime_error("Arquivo vazio ou sem arestas validas.");

    auto G = std::make_unique<ListaAdjacenciaPesada>(N);
    for (auto [a, b, peso] : edges) {
        G->adicionarAresta(a, b, peso); // ida
        G->adicionarAresta(b, a, peso); // volta
    }
    G->finalizarConstrucao();
    return G;
}

