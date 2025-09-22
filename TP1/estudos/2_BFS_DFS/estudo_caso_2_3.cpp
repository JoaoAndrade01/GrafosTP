/**
 * @file estudo_caso_2_3.cpp
 * @brief Implementação do Estudo de Caso 2 e 3: Benchmark de tempo de BFS/DFS.
 */
#include "../estudos.h"
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <random>
#include <fstream>
#include <stdexcept>
#include <iomanip>
#include <numeric>
#include <algorithm>
#include <cmath>
#include "../../biblioteca/interface/Grafo.h"
#include "../../biblioteca/algoritmos/BFS.h"
#include "../../biblioteca/algoritmos/DFS.h"

 // Variável de controle
const int NUM_EXECUCOES_POR_AMOSTRA = 100;

// Estruturas de dados locais
struct TarefaBenchmark {
    std::string nomeGrafo;
    TipoRepresentacao tipoRepr;
    std::string nomeRepr;
};

struct ResultadoAmostra {
    double tempoTotalMs = 0.0, tempoMedioMs = 0.0, tempoMinMs = 0.0, tempoMaxMs = 0.0, desvioPadraoMs = 0.0;
};

// Protótipo da função auxiliar
static ResultadoAmostra executarAmostra(const Grafo& grafo, const std::string& algoritmo, int numExecucoes);


void executarEstudoDeCasoTempo() {
    std::cout << "--- Benchmark de Performance (com Estatisticas Detalhadas) ---" << std::endl;
    std::cout << "Configurado para executar " << NUM_EXECUCOES_POR_AMOSTRA << " buscas por amostra." << std::endl;

    char continuar;
    std::cout << "\nDeseja iniciar o benchmark? (s/n): ";
    std::cin >> continuar;
    if (continuar != 's' && continuar != 'S') {
        std::cout << "Benchmark cancelado." << std::endl;
        return;
    }

    std::ofstream arquivoResultados("resultados_benchmark_final_detalhado.csv");
    arquivoResultados << "Grafo,Representacao,Algoritmo,Vertices,Arestas,NumExecucoes,TempoTotal_ms,TempoMedio_ms,TempoMin_ms,TempoMax_ms,DesvioPadrao_ms,Status\n";

    std::vector<TarefaBenchmark> tarefas;
    std::vector<std::string> grafosPequenos = { "grafo_1.txt", "grafo_2.txt" };
    std::vector<std::string> grafosGrandes = { "grafo_3.txt", "grafo_4.txt", "grafo_5.txt", "grafo_6.txt" };

    std::vector<TipoRepresentacao> todasRepr = { TipoRepresentacao::LISTA_ADJACENCIA_SIMPLES, TipoRepresentacao::LISTA_ADJACENCIA, TipoRepresentacao::MATRIZ_ADJACENCIA, TipoRepresentacao::MATRIZ_ADJACENCIA_TRIANGULAR };
    std::vector<std::string> todosNomesRepr = { "Lista Simples", "Lista Otimizada (CSR)", "Matriz Classica", "Matriz Triangular" };

    std::vector<TipoRepresentacao> reprListas = { TipoRepresentacao::LISTA_ADJACENCIA_SIMPLES, TipoRepresentacao::LISTA_ADJACENCIA };
    std::vector<std::string> nomesReprListas = { "Lista Simples", "Lista Otimizada (CSR)" };

    for (const auto& nome : grafosPequenos) {
        for (size_t i = 0; i < todasRepr.size(); ++i) { tarefas.push_back({ nome, todasRepr[i], todosNomesRepr[i] }); }
    }
    for (const auto& nome : grafosGrandes) {
        for (size_t i = 0; i < reprListas.size(); ++i) { tarefas.push_back({ nome, reprListas[i], nomesReprListas[i] }); }
    }

    std::cout << "\nIniciando benchmark completo..." << std::endl;

    for (const auto& tarefa : tarefas) {
        std::cout << "\nProcessando: " << tarefa.nomeGrafo << " com " << tarefa.nomeRepr << "..." << std::endl;
        try {
            Grafo grafo(caminhoEstudos(tarefa.nomeGrafo), tarefa.tipoRepr);

            std::cout << "  - Executando Amostra BFS..." << std::flush;
            ResultadoAmostra resBfs = executarAmostra(grafo, "BFS", NUM_EXECUCOES_POR_AMOSTRA);
            std::cout << " OK (" << resBfs.tempoMedioMs << " ms/busca)" << std::endl;
            arquivoResultados << tarefa.nomeGrafo << "," << tarefa.nomeRepr << ",BFS," << grafo.obterNumeroVertices() << "," << grafo.obterNumeroArestas() << "," << NUM_EXECUCOES_POR_AMOSTRA << "," << resBfs.tempoTotalMs << "," << resBfs.tempoMedioMs << "," << resBfs.tempoMinMs << "," << resBfs.tempoMaxMs << "," << resBfs.desvioPadraoMs << ",OK\n";

            std::cout << "  - Executando Amostra DFS..." << std::flush;
            ResultadoAmostra resDfs = executarAmostra(grafo, "DFS", NUM_EXECUCOES_POR_AMOSTRA);
            std::cout << " OK (" << resDfs.tempoMedioMs << " ms/busca)" << std::endl;
            arquivoResultados << tarefa.nomeGrafo << "," << tarefa.nomeRepr << ",DFS," << grafo.obterNumeroVertices() << "," << grafo.obterNumeroArestas() << "," << NUM_EXECUCOES_POR_AMOSTRA << "," << resDfs.tempoTotalMs << "," << resDfs.tempoMedioMs << "," << resDfs.tempoMinMs << "," << resDfs.tempoMaxMs << "," << resDfs.desvioPadraoMs << ",OK\n";

        }
        catch (const std::exception& e) {
            std::cerr << "  > ERRO: " << e.what() << std::endl;
            arquivoResultados << tarefa.nomeGrafo << "," << tarefa.nomeRepr << ",BFS,-1,-1," << NUM_EXECUCOES_POR_AMOSTRA << ",-1,-1,-1,-1,-1,FALHA\n";
            arquivoResultados << tarefa.nomeGrafo << "," << tarefa.nomeRepr << ",DFS,-1,-1," << NUM_EXECUCOES_POR_AMOSTRA << ",-1,-1,-1,-1,-1,FALHA\n";
        }
    }

    arquivoResultados.close();
    std::cout << "\n--- Benchmark completo! Resultados salvos em 'resultados_benchmark_final_detalhado.csv' ---" << std::endl;
}

static ResultadoAmostra executarAmostra(const Grafo& grafo, const std::string& algoritmo, int numExecucoes) {
    ResultadoAmostra resultado;
    if (numExecucoes == 0) return resultado;
    BFS algoritmoBFS;
    DFS algoritmoDFS;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, grafo.obterNumeroVertices());

    std::vector<double> temposIndividuais;
    temposIndividuais.reserve(numExecucoes);

    for (int k = 0; k < numExecucoes; ++k) {
        int verticeDePartida = distrib(gen);
        auto inicio = std::chrono::high_resolution_clock::now();
        if (algoritmo == "BFS") algoritmoBFS.executar(grafo, verticeDePartida);
        else if (algoritmo == "DFS") algoritmoDFS.executar(grafo, verticeDePartida);
        auto fim = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duracao = fim - inicio;
        temposIndividuais.push_back(duracao.count());
    }

    if (!temposIndividuais.empty()) {
        double somaDosTempos = std::accumulate(temposIndividuais.begin(), temposIndividuais.end(), 0.0);
        resultado.tempoTotalMs = somaDosTempos;
        resultado.tempoMedioMs = somaDosTempos / numExecucoes;
        resultado.tempoMinMs = *std::min_element(temposIndividuais.begin(), temposIndividuais.end());
        resultado.tempoMaxMs = *std::max_element(temposIndividuais.begin(), temposIndividuais.end());
        double somaDosDesviosQuadrados = 0.0;
        for (double tempo : temposIndividuais) {
            somaDosDesviosQuadrados += (tempo - resultado.tempoMedioMs) * (tempo - resultado.tempoMedioMs);
        }
        double variancia = somaDosDesviosQuadrados / numExecucoes;
        resultado.desvioPadraoMs = std::sqrt(variancia);
    }
    return resultado;
}