#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <fstream>
#include <memory>
#include <chrono>
#include <iomanip>

#include "../biblioteca/representacao/ListaAdjacenciaPesada.h"
#include "../biblioteca/algoritmos/Dijkstra.h"
#include "../biblioteca/util/LeitorGrafoPesado.h"

// Imprime o caminho s -> t usando o vetor de pais, no stream indicado
static void imprimirCaminho(const std::vector<int>& pai, int s, int t, std::ostream& os) {
    if (t < 1 || s < 1) { os << "(sem caminho)\n"; return; }
    if (pai[t] == -1 && t != s) { os << "(sem caminho)\n"; return; }

    std::vector<int> path;
    for (int v = t; v != -1; v = pai[v]) path.push_back(v);
    for (int i = (int)path.size() - 1; i >= 0; --i) {
        os << path[i] << (i ? " -> " : "");
    }
    os << "\n";
}

static std::string caminhoDatasetPadrao(const std::string& rel) {
    // se estiver executando de dentro de build/, ../datasets/... existe
    std::ifstream test1(std::string("../") + rel);
    if (test1.good()) return std::string("../") + rel;
    return rel;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(&std::cout);

    std::cout << "=== TP2: Dijkstra ===\n";
    std::cout << "Arquivo (ex.: datasets/grafo_W_1.txt): ";
    std::string arquivo;
    std::getline(std::cin, arquivo);
    if (arquivo.empty()) arquivo = "datasets/grafo_W_1.txt";
    arquivo = caminhoDatasetPadrao(arquivo);

    bool temNeg = false;
    std::unique_ptr<Representacao> G;
    try {
        // leitor que detecta N no arquivo e avisa se ha peso negativo
        G = carregarGrafoPesadoAuto(arquivo, &temNeg);
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
    const long M = G->obterNumeroDeArestas();
    std::cout << "Grafo carregado (N=" << N << ", M=" << M << ")\n\n";

    while (true) {
        std::cout << "Menu:\n"
                  << "  1) Distancia e caminho entre um vertice e para outro vertice\n"
                  << "  2) Distancias e caminhos de um vertice para TODOS\n"
                  << "  0) Sair\n"
                  << "Escolha: ";
        int op = -1;
        if (!(std::cin >> op)) return 0;
        if (op == 0) break;

        if (op == 1) {
            int s, t;
            std::cout << "Origem s (1..N, 0 sai): ";
            if (!(std::cin >> s) || s == 0) break;
            std::cout << "Destino t (1..N, 0 sai): ";
            if (!(std::cin >> t) || t == 0) break;

            if (s < 1 || s > N || t < 1 || t > N) {
                std::cout << "Fora de [1..N]. Tente novamente.\n";
                continue;
            }

            auto t0 = std::chrono::steady_clock::now();
            auto R  = tp2::dijkstra_vector(*G, s);   // versao com vetor
            auto t1 = std::chrono::steady_clock::now();
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();

            if (std::isinf(R.dist[t])) {
                std::cout << "Nao ha caminho de " << s << " ate " << t << ".\n";
            } else {
                std::cout << std::fixed << std::setprecision(2);
                std::cout << "Distancia minima de " << s << " ate " << t << " = " << R.dist[t] << "\n";
                std::cout << "Caminho: ";
                imprimirCaminho(R.pai, s, t, std::cout);
            }
            std::cout << "(tempo: " << ms << " ms)\n\n";
        }
        else if (op == 2) {
            int s;
            std::cout << "Origem s (1..N, 0 sai): ";
            if (!(std::cin >> s) || s == 0) break;
            if (s < 1 || s > N) { std::cout << "s fora de [1..N].\n"; continue; }

            std::cout << "Salvar TODOS os caminhos em arquivo? (s/n): ";
            char salvar = 'n';
            std::cin >> salvar;
            bool emArquivo = (salvar == 's' || salvar == 'S');

            std::ofstream arq;
            if (emArquivo) {
                std::string nome = "arvore_s_" + std::to_string(s) + ".txt";
                arq.open(nome);
                if (!arq) {
                    std::cout << "Nao foi possivel criar " << nome << " (imprimindo no console).\n";
                    emArquivo = false;
                } else {
                    std::cout << "Salvando em " << nome << "...\n";
                }
            }

            auto t0 = std::chrono::steady_clock::now();
            auto R  = tp2::dijkstra_vector(*G, s);
            auto t1 = std::chrono::steady_clock::now();
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();

            std::ostream& out = emArquivo ? arq : std::cout;
            out << std::fixed << std::setprecision(2);

            long alcancaveis = 0;
            for (int t = 1; t <= N; ++t) {
                out << "t=" << t << "  dist=";
                if (std::isinf(R.dist[t])) {
                    out << "INF  caminho: (sem caminho)\n";
                } else {
                    ++alcancaveis;
                    out << R.dist[t] << "  caminho: ";
                    imprimirCaminho(R.pai, s, t, out);
                }
            }

            std::cout << "Fonte s=" << s
                      << " | alcancaveis=" << alcancaveis << "/" << N
                      << " | tempo total: " << ms << " ms\n\n";
        }
        else {
            std::cout << "Opcao invalida.\n";
        }
    }

    std::cout << "Encerrando.\n";
    return 0;
}

