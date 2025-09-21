/**
 * @file main.cpp
 * @brief Programa de teste com menu para estudos de caso da biblioteca de grafos.
 */

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <chrono>   // Para medi��o de tempo
#include <random>   // Para gerar v�rtices aleat�rios
#include <fstream>  // Para salvar o relat�rio em arquivo

#include "../../biblioteca/interface/Grafo.h"
#include "../../biblioteca/algoritmos/BFS.h"

 // --- Prot�tipos das fun��es ---
void exibirMenuPrincipal();
void executarEstudoDeCasoMemoria(); // Renomeado para clareza
void executarEstudoDeCasoTempo();   // <-- Nova fun��o
void pausarParaContinuar();

/**
 * @brief Fun��o principal que gerencia o menu e delega as tarefas.
 */
int main() {
    int escolha = -1;
    while (true) {
        exibirMenuPrincipal();
        std::cout << "Digite sua escolha: ";
        std::cin >> escolha;

        // ... (valida��o de entrada do cin) ...

        switch (escolha) {
        case 1:
            executarEstudoDeCasoMemoria();
            break;
        case 2:
            executarEstudoDeCasoTempo(); // <-- Nova op��o
            break;
        case 0:
            std::cout << "Saindo do programa..." << std::endl;
            return 0;
        default:
            std::cout << "\nOpcao invalida. Por favor, tente novamente." << std::endl;
            pausarParaContinuar();
            break;
        }
    }
    return 0;
}

/**
 * @brief Exibe o menu principal de op��es.
 */
void exibirMenuPrincipal() {
    // ...
    std::cout << "\n=======================================================" << std::endl;
    std::cout << "=== MENU PRINCIPAL DE ESTUDOS DE CASO ===" << std::endl;
    std::cout << "=======================================================" << std::endl;
    std::cout << "1. Estudo de Caso 1: Comparar Consumo de Memoria" << std::endl;
    std::cout << "2. Estudo de Caso 2: Comparar Tempo de Execucao (BFS)" << std::endl; // <-- Nova op��o
    std::cout << "-------------------------------------------------------" << std::endl;
    std::cout << "0. Sair" << std::endl;
    // ...
}

// ... (fun��o executarEstudoDeCasoMemoria renomeada e seu menu interno) ...

/**
 * @brief Executa o Estudo de Caso 2: benchmark de tempo do BFS.
 */
void executarEstudoDeCasoTempo() {
    // Bloco 1: Setup do teste
    std::cout << "\n--- Estudo de Caso 2: Benchmark de Tempo de Execucao do BFS ---" << std::endl;
    const int NUM_EXECUCOES = 100;
    std::vector<std::string> nomesGrafos = { "grafo_1.txt", "grafo_2.txt", "grafo_3.txt", "grafo_4.txt", "grafo_5.txt", "grafo_6.txt" };

    // Bloco 2: Cria��o do arquivo de resultados
    std::ofstream arquivoResultados("resultados_tempo_bfs.csv");
    if (!arquivoResultados.is_open()) {
        std::cerr << "ERRO: Nao foi possivel criar o arquivo de resultados." << std::endl;
        pausarParaContinuar();
        return;
    }
    // Escreve o cabe�alho do arquivo CSV
    arquivoResultados << "Grafo,Representacao,Vertices,Arestas,TempoTotal_ms,TempoMedio_ms\n";
    std::cout << "Resultados serao salvos em 'resultados_tempo_bfs.csv'" << std::endl;

    // Bloco 3: Loop principal sobre as 4 representa��es
    for (int i = 1; i <= 4; ++i) {
        TipoRepresentacao tipo;
        std::string nomeRepr;
        // Pede para o usu�rio confirmar antes de cada bateria de testes longa
        std::cout << "\nPronto para testar a representacao ";
        switch (i) {
        case 1: tipo = TipoRepresentacao::LISTA_ADJACENCIA_SIMPLES; nomeRepr = "Lista Simples"; break;
        case 2: tipo = TipoRepresentacao::LISTA_ADJACENCIA; nomeRepr = "Lista Otimizada (CSR)"; break;
        case 3: tipo = TipoRepresentacao::MATRIZ_ADJACENCIA; nomeRepr = "Matriz Classica"; break;
        case 4: tipo = TipoRepresentacao::MATRIZ_ADJACENCIA_TRIANGULAR; nomeRepr = "Matriz Triangular"; break;
        }
        std::cout << "'" << nomeRepr << "' em todos os 6 grafos?" << std::endl;
        pausarParaContinuar();

        // Bloco 4: Loop sobre os 6 arquivos de grafo
        for (const auto& nomeBase : nomesGrafos) {
            std::string caminhoCompleto = "C:/Users/Jo�o - Dynatest/source/repos/GrafosTP/TP1/estudos/grafos_em_txt/" + nomeBase;
            std::cout << "\nProcessando: " << nomeBase << " com " << nomeRepr << "..." << std::endl;

            try {
                // Bloco 5: Carregamento do grafo
                Grafo grafo(caminhoCompleto, tipo);
                const int numVertices = grafo.obterNumeroVertices();

                // Bloco 6: Gera��o de 100 v�rtices de partida aleat�rios
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> distrib(1, numVertices);
                std::vector<int> verticesAleatorios;
                for (int k = 0; k < NUM_EXECUCOES; ++k) {
                    verticesAleatorios.push_back(distrib(gen));
                }

                // Bloco 7: Execu��o e medi��o de tempo
                BFS algoritmoBFS;
                auto inicio = std::chrono::high_resolution_clock::now();

                for (int vertice : verticesAleatorios) {
                    algoritmoBFS.executar(grafo, vertice);
                }

                auto fim = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> duracaoTotalMs = fim - inicio;

                double tempoTotal = duracaoTotalMs.count();
                double tempoMedio = tempoTotal / NUM_EXECUCOES;

                // Bloco 8: Registro dos resultados
                std::cout << "  > Concluido! Tempo medio: " << tempoMedio << " ms" << std::endl;
                arquivoResultados << nomeBase << "," << nomeRepr << ","
                    << grafo.obterNumeroVertices() << "," << grafo.obterNumeroArestas() << ","
                    << tempoTotal << "," << tempoMedio << "\n";

            }
            catch (const std::exception& e) {
                // Se der erro (ex: estouro de mem�ria), registra a falha.
                std::cerr << "  > ERRO: " << e.what() << std::endl;
                arquivoResultados << nomeBase << "," << nomeRepr << ",-1,-1,FALHA,FALHA\n";
            }
        }
    }

    arquivoResultados.close();
    std::cout << "\n--- Estudo de Caso de Tempo Concluido! ---" << std::endl;
    pausarParaContinuar();
}

void pausarParaContinuar() { /* ... c�digo anterior ... */ }
void executarEstudoDeCasoMemoria() { /* ... c�digo anterior ... */ }