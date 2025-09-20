#include <iostream>
#include <string>
#include <memory>
#include <limits>
#include <chrono>
#include "IGraph.h"
#include "GraphFactory.h"
#include "BFS.h"   // bfs(...) e salvarRelatorioCompleto(...)
#include <iomanip>

using std::cin;
using std::cout;
using std::endl;

// Menu de representacao para o usuário

static GraphRepresentation escolherRepresentacao() {
    int opcao = 0;
    while (true) {
        cout << "\n=== Escolha a representacao ===\n";
        cout << "  1) Vetor de adjacencia (CSR)\n";
        cout << "  2) Lista de adjacencia\n";
        cout << "  3) Matriz de adjacencia\n";
        cout << "Opcao: ";
        if (cin >> opcao && (opcao >= 1 && opcao <= 3)) break;
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "Entrada invalida. Tente novamente.\n";
    }
    if (opcao == 1) return GraphRepresentation::ADJACENCY_VECTOR;
    if (opcao == 2) return GraphRepresentation::ADJACENCY_LIST;
    return GraphRepresentation::ADJACENCY_MATRIX;
}

static const char* nomeRep(GraphRepresentation r) {
    switch (r) {
        case GraphRepresentation::ADJACENCY_VECTOR: return "Vector";
        case GraphRepresentation::ADJACENCY_LIST:   return "Lista";
        case GraphRepresentation::ADJACENCY_MATRIX: return "Matriz";
    }
    return "Desconhecida";
}

int main() {

    //Entrada do arquivo
    std::string arquivo;
    cout << "Arquivo do grafo (ex: grafo_1.txt): ";
    cin >> arquivo;

    //Escolha da representacao
    GraphRepresentation rep = escolherRepresentacao();
    cout << "\nLendo '" << arquivo << "' como " << nomeRep(rep) << "...\n";

    //Construir o grafo (tempo medido pela GraphFactory)
    auto grafo = GraphFactory::createGraphFromFile(arquivo, rep);
    if (!grafo) {
        std::cerr << "Erro ao criar o grafo a partir de '" << arquivo << "'.\n";
        return 1;
    }
    double tempoConstrucao = GraphFactory::getLastBuildStats().tempoConstrucaoSeg;

    //Vértice de origem da BFS
    int origem = 1;
    cout << "Vertice de origem da BFS (1..n) [padrao=1]: ";
    if (!(cin >> origem)) {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        origem = 1;
    }
    if (origem < 1 || origem > grafo->getNumVertices()) {
        cout << "Origem invalida. Ajustando para 1.\n";
        origem = 1;
    }


    //Executar BFS (cronometro)
    using clock = std::chrono::high_resolution_clock;
    auto t0 = clock::now();
    auto R  = bfs(*grafo, origem);
    auto t1 = clock::now();
    double tempoBFS = std::chrono::duration<double>(t1 - t0).count();
    // Mostra no terminal também
      std::cout << "Tempo de execucao da BFS: "
          << std::fixed << std::setprecision(6)
          << tempoBFS << " segundos" << std::endl;

    // resumo do resultado da BFS (para não poluir)
    cout << "\n=== Resumo da BFS (" << nomeRep(rep) << ") a partir de " << origem << " ===\n";
    int n = grafo->getNumVertices();
    int mostrar = std::min(n, 20);
    cout << "Vertice  Pai  Nivel\n";
    for (int v = 1; v <= mostrar; ++v) {
        cout << v << "        " << R.pai[v] << "    " << R.nivel[v] << "\n";
    }
    if (n > mostrar) cout << "... (" << (n - mostrar) << " vertices ocultados)\n";


    //Gerar relatorio

    std::string nomeSaida = std::string("relatorio_") + nomeRep(rep) + ".txt";
    salvarRelatorioCompleto(*grafo, R, origem, nomeSaida, tempoConstrucao, tempoBFS);
    cout << "\nRelatorio gerado em '" << nomeSaida << "'.\n";

    return 0;
}

