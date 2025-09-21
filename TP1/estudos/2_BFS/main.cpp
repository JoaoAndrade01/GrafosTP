/**
 * @file main.cpp
 * @brief Programa final de benchmark, com coleta de estatísticas detalhadas.
 */
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <random>
#include <fstream>
#include <stdexcept>
#include <iomanip>
#include <numeric>   // Para std::accumulate
#include <algorithm> // Para std::min_element, std::max_element
#include <cmath>     // Para std::sqrt (raiz quadrada)

#include "../../biblioteca/interface/Grafo.h"
#include "../../biblioteca/algoritmos/BFS.h"
#include "../../biblioteca/algoritmos/DFS.h"

 // ===================================================================
 // ================== VARIÁVEL DE CONTROLE PRINCIPAL ==================
 // ===================================================================
 // Mude este valor para 1 para um teste rápido de validação/estimativa.
 // Mude para 100 para o benchmark completo do estudo de caso.
const int NUM_EXECUCOES_POR_AMOSTRA = 100;
// ===================================================================

// Estrutura para definir uma tarefa de benchmark
struct TarefaBenchmark {
    std::string nomeGrafo;
    TipoRepresentacao tipoRepr;
    std::string nomeRepr;
};

// Estrutura para guardar o resultado de uma amostra (com mais métricas)
struct ResultadoAmostra {
    double tempoTotalMs = 0.0;
    double tempoMedioMs = 0.0;
    double tempoMinMs = 0.0;
    double tempoMaxMs = 0.0;
    double desvioPadraoMs = 0.0;
};

// --- Protótipos ---
ResultadoAmostra executarAmostra(const Grafo& grafo, const std::string& algoritmo, int numExecucoes);

int main() {
    std::cout << "--- Benchmark de Performance (com Estatisticas Detalhadas) ---" << std::endl;
    std::cout << "Configurado para executar " << NUM_EXECUCOES_POR_AMOSTRA << " buscas por amostra." << std::endl;

    // Confirmação do usuário para iniciar
    char continuar;
    std::cout << "\nDeseja iniciar o benchmark? (s/n): ";
    std::cin >> continuar;
    if (continuar != 's' && continuar != 'S') {
        std::cout << "Benchmark cancelado." << std::endl;
        return 0;
    }

    // --- Lógica de Execução ---
    std::string caminhoBase = "C:/Users/João - Dynatest/source/repos/GrafosTP/TP1/estudos/grafos_em_txt/";
    std::ofstream arquivoResultados("resultados_benchmark_final_detalhado.csv");
    // Novo cabeçalho do CSV com as colunas extras
    arquivoResultados << "Grafo,Representacao,Algoritmo,Vertices,Arestas,NumExecucoes,TempoTotal_ms,TempoMedio_ms,TempoMin_ms,TempoMax_ms,DesvioPadrao_ms,Status\n";

    // Define o plano de execução
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

    // Loop principal que orquestra as tarefas
    for (const auto& tarefa : tarefas) {
        std::cout << "\nProcessando: " << tarefa.nomeGrafo << " com " << tarefa.nomeRepr << "..." << std::endl;
        try {
            Grafo grafo(caminhoBase + tarefa.nomeGrafo, tarefa.tipoRepr);

            // --- Executa a amostra para BFS ---
            std::cout << "  - Executando Amostra BFS..." << std::flush;
            ResultadoAmostra resBfs = executarAmostra(grafo, "BFS", NUM_EXECUCOES_POR_AMOSTRA);
            std::cout << " OK (" << resBfs.tempoMedioMs << " ms/busca)" << std::endl;
            // Escreve a linha completa de resultados para o BFS
            arquivoResultados << tarefa.nomeGrafo << "," << tarefa.nomeRepr << ",BFS," << grafo.obterNumeroVertices() << "," << grafo.obterNumeroArestas() << "," << NUM_EXECUCOES_POR_AMOSTRA << "," << resBfs.tempoTotalMs << "," << resBfs.tempoMedioMs << "," << resBfs.tempoMinMs << "," << resBfs.tempoMaxMs << "," << resBfs.desvioPadraoMs << ",OK\n";

            // --- Executa a amostra para DFS ---
            std::cout << "  - Executando Amostra DFS..." << std::flush;
            ResultadoAmostra resDfs = executarAmostra(grafo, "DFS", NUM_EXECUCOES_POR_AMOSTRA);
            std::cout << " OK (" << resDfs.tempoMedioMs << " ms/busca)" << std::endl;
            // Escreve a linha completa de resultados para o DFS
            arquivoResultados << tarefa.nomeGrafo << "," << tarefa.nomeRepr << ",DFS," << grafo.obterNumeroVertices() << "," << grafo.obterNumeroArestas() << "," << NUM_EXECUCOES_POR_AMOSTRA << "," << resDfs.tempoTotalMs << "," << resDfs.tempoMedioMs << "," << resDfs.tempoMinMs << "," << resDfs.tempoMaxMs << "," << resDfs.desvioPadraoMs << ",OK\n";

        }
        catch (const std::exception& e) {
            std::cerr << "  > ERRO: " << e.what() << std::endl;
            // Escreve uma linha de falha com colunas extras para manter o CSV alinhado
            arquivoResultados << tarefa.nomeGrafo << "," << tarefa.nomeRepr << ",BFS,-1,-1," << NUM_EXECUCOES_POR_AMOSTRA << ",-1,-1,-1,-1,-1,FALHA\n";
            arquivoResultados << tarefa.nomeGrafo << "," << tarefa.nomeRepr << ",DFS,-1,-1," << NUM_EXECUCOES_POR_AMOSTRA << ",-1,-1,-1,-1,-1,FALHA\n";
        }
    }

    arquivoResultados.close();
    std::cout << "\n-------------------------------------------------------" << std::endl;
    std::cout << "Benchmark completo! Resultados salvos em 'resultados_benchmark_final_detalhado.csv'" << std::endl;
    std::cout << "-------------------------------------------------------" << std::endl;
    return 0;
}

/**
 * @brief Executa uma "amostra" de benchmark, medindo cada execução individualmente
 * para calcular estatísticas detalhadas.
 */
ResultadoAmostra executarAmostra(const Grafo& grafo, const std::string& algoritmo, int numExecucoes) {
    ResultadoAmostra resultado;
    if (numExecucoes == 0) return resultado;

    BFS algoritmoBFS;
    DFS algoritmoDFS;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, grafo.obterNumeroVertices());

    // Vetor para guardar o tempo de cada uma das execuções
    std::vector<double> temposIndividuais;
    temposIndividuais.reserve(numExecucoes);

    // Bloco: Execução e medição individual
    for (int k = 0; k < numExecucoes; ++k) {
        int verticeDePartida = distrib(gen);
        auto inicio = std::chrono::high_resolution_clock::now();

        if (algoritmo == "BFS") {
            algoritmoBFS.executar(grafo, verticeDePartida);
        }
        else if (algoritmo == "DFS") {
            algoritmoDFS.executar(grafo, verticeDePartida);
        }

        auto fim = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duracao = fim - inicio;
        temposIndividuais.push_back(duracao.count());
    }

    // Bloco: Cálculo das estatísticas a partir dos tempos individuais
    if (!temposIndividuais.empty()) {
        // 1. Soma e Média
        double somaDosTempos = std::accumulate(temposIndividuais.begin(), temposIndividuais.end(), 0.0);
        resultado.tempoTotalMs = somaDosTempos;
        resultado.tempoMedioMs = somaDosTempos / numExecucoes;

        // 2. Mínimo e Máximo
        resultado.tempoMinMs = *std::min_element(temposIndividuais.begin(), temposIndividuais.end());
        resultado.tempoMaxMs = *std::max_element(temposIndividuais.begin(), temposIndividuais.end());

        // 3. Desvio Padrão
        double somaDosDesviosQuadrados = 0.0;
        for (double tempo : temposIndividuais) {
            somaDosDesviosQuadrados += (tempo - resultado.tempoMedioMs) * (tempo - resultado.tempoMedioMs);
        }
        double variancia = somaDosDesviosQuadrados / numExecucoes;
        resultado.desvioPadraoMs = std::sqrt(variancia);
    }

    return resultado;
}