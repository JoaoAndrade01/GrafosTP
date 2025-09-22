/**
 * @file estudo_caso_1.cpp
 * @brief Implementação do Estudo de Caso 1: Comparação de consumo de memória.
 */
#include "../estudos.h"
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <stdexcept>
#include "../../biblioteca/interface/Grafo.h"

 // --- Funções Auxiliares (locais a este arquivo) ---
static void pausarParaContinuar_EC1() {
    std::cout << "\nPressione ENTER para voltar ao menu...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

static void exibirMenu_EC1() {
    std::cout << "\n--- Estudo de Caso 1: Memoria ---" << std::endl;
    std::cout << "1. Comparar Listas de Adjacencia (Simples vs. Otimizada/CSR)" << std::endl;
    std::cout << "2. Comparar Matrizes de Adjacencia (Classica vs. Triangular)" << std::endl;
    std::cout << "0. Voltar ao Menu Principal" << std::endl;
    std::cout << "---------------------------------" << std::endl;
}

static void executarTesteDeListas_EC1() {
    std::vector<std::string> nomesGrafos = { "grafo_1.txt", "grafo_2.txt", "grafo_3.txt", "grafo_4.txt", "grafo_5.txt", "grafo_6.txt" };
    std::cout << "\n--- Iniciando Teste de LISTAS ---" << std::endl;
    for (const auto& nomeBase : nomesGrafos) {
        std::string caminhoCompleto = caminhoEstudos(nomeBase);
        std::cout << "\n--- Processando: " << nomeBase << " ---" << std::endl;
        try {
            {
                std::cout << "[LISTA 1/2] Carregando com LISTA SIMPLES..." << std::endl;
                Grafo grafo(caminhoCompleto, TipoRepresentacao::LISTA_ADJACENCIA_SIMPLES);
                std::cout << "  > Vertices: " << grafo.obterNumeroVertices() << " | Arestas: " << grafo.obterNumeroArestas() << std::endl;
                std::cout << "\n  >>> ACAO: Verifique a memoria. Pressione ENTER para continuar..." << std::endl;
                std::cin.get();
            }
            {
                std::cout << "\n[LISTA 2/2] Carregando com LISTA OTIMIZADA (CSR)..." << std::endl;
                Grafo grafo(caminhoCompleto, TipoRepresentacao::LISTA_ADJACENCIA);
                std::cout << "  > Vertices: " << grafo.obterNumeroVertices() << " | Arestas: " << grafo.obterNumeroArestas() << std::endl;
                std::cout << "\n  >>> ACAO: Verifique a memoria. Pressione ENTER para o proximo grafo..." << std::endl;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
            }
        }
        catch (const std::exception& e) { std::cerr << "ERRO: " << e.what() << std::endl; }
    }
    std::cout << "\n--- Teste de Listas Concluido! ---" << std::endl;
}

static void executarTesteDeMatrizes_EC1() {
    std::vector<std::string> nomesGrafos = { "grafo_1.txt", "grafo_2.txt", "grafo_3.txt", "grafo_4.txt", "grafo_5.txt", "grafo_6.txt" };
    std::cout << "\n--- Iniciando Teste de MATRIZES ---" << std::endl;
    for (const auto& nomeBase : nomesGrafos) {
        std::string caminhoCompleto = caminhoEstudos(nomeBase);
        std::cout << "\n--- Processando: " << nomeBase << " ---" << std::endl;
        try {
            {
                std::cout << "[MATRIZ 1/2] Carregando com MATRIZ CLASSICA..." << std::endl;
                Grafo grafo(caminhoCompleto, TipoRepresentacao::MATRIZ_ADJACENCIA);
                std::cout << "  > Vertices: " << grafo.obterNumeroVertices() << " | Arestas: " << grafo.obterNumeroArestas() << std::endl;
                std::cout << "\n  >>> ACAO: Verifique a memoria. Pressione ENTER para continuar..." << std::endl;
                std::cin.get();
            }
            {
                std::cout << "\n[MATRIZ 2/2] Carregando com MATRIZ TRIANGULAR..." << std::endl;
                Grafo grafo(caminhoCompleto, TipoRepresentacao::MATRIZ_ADJACENCIA_TRIANGULAR);
                std::cout << "  > Vertices: " << grafo.obterNumeroVertices() << " | Arestas: " << grafo.obterNumeroArestas() << std::endl;
                std::cout << "\n  >>> ACAO: Verifique a memoria. Pressione ENTER para o proximo grafo..." << std::endl;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
            }
        }
        catch (const std::exception& e) { std::cerr << "ERRO: " << e.what() << std::endl; }
    }
    std::cout << "\n--- Teste de Matrizes Concluido! ---" << std::endl;
}


void executarEstudoDeCasoMemoria() {
    int escolha = -1;
    while (escolha != 0) {
        exibirMenu_EC1();
        std::cout << "Digite sua escolha: ";
        std::cin >> escolha;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            escolha = -1;
        }

        switch (escolha) {
        case 1: executarTesteDeListas_EC1(); break;
        case 2: executarTesteDeMatrizes_EC1(); break;
        case 0: std::cout << "Voltando ao menu principal..." << std::endl; break;
        default: std::cout << "\nOpcao invalida." << std::endl; break;
        }
        if (escolha != 0) pausarParaContinuar_EC1();
    }
}