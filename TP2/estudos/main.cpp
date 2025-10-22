#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <memory>

#include "../biblioteca/representacao/ListaAdjacenciaPesada.h"
#include "../biblioteca/algoritmos/Dijkstra.h"
#include "../biblioteca/util/LeitorGrafoPesado.h"

// Imprime o caminho s -> t com vetor de pais
static void imprimirCaminho(const std::vector<int>& pai, int s, int t) {
    if (t < 1 || s < 1) { std::cout << "(sem caminho)\n"; return; }
    if (pai[t] == -1 && t != s) { std::cout << "(sem caminho)\n"; return; }
    std::vector<int> path;
    for (int v = t; v != -1; v = pai[v]) path.push_back(v);
    for (int i = (int)path.size() - 1; i >= 0; --i)
        std::cout << path[i] << (i ? " -> " : "\n");
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(&std::cout);

    std::string arquivo;
    std::cout << "=== TP2: Dijkstra ===\n";
    std::cout << "Arquivo (ex.: datasets/grafo_W_1.txt): ";
    std::getline(std::cin, arquivo);

    if (arquivo.empty()) {
        std::ifstream test1("../datasets/grafo_W_1.txt");
        arquivo = (test1.good() ? "../datasets/grafo_W_1.txt" : "datasets/grafo_W_1.txt");
    }

    bool temNeg = false;
    std::unique_ptr<Representacao> G;
    try {
        G = carregarGrafoPesadoAuto(arquivo, &temNeg); // <== usa o leitor do util
    } catch (const std::exception& e) {
        std::cout << "[ERRO] " << e.what() << "\n";
        return 1;
    }

    if (temNeg) {
        std::cout << "Dijkstra bloqueado: ha pesos negativos no grafo.\n";
        std::cout << "A biblioteca ainda nao implementa caminhos minimos com pesos negativos.\n";
        return 0;
    }

    const int N = G->obterNumeroDeVertices();
    std::cout << "Grafo carregado (N=" << N << ", M=" << G->obterNumeroDeArestas() << ")\n";

    while (true) {
        int s, t;
        std::cout << "\nOrigem s (1..N, 0 sai): ";
        if (!(std::cin >> s) || s == 0) break;
        std::cout << "Destino t (1..N, 0 sai): ";
        if (!(std::cin >> t) || t == 0) break;

        if (s < 1 || s > N || t < 1 || t > N) {
            std::cout << "Fora de [1..N]. Tente novamente.\n";
            continue;
        }

        auto R = tp2::dijkstra_vector(*G, s);

        if (std::isinf(R.dist[t])) {
            std::cout << "Nao ha caminho de " << s << " ate " << t << ".\n";
        } else {
            std::cout << "Distancia minima de " << s << " ate " << t << " = " << R.dist[t] << "\n";
            std::cout << "Caminho: ";
            imprimirCaminho(R.pai, s, t);
        }
    }

    std::cout << "Encerrando.\n";
    return 0;
}

