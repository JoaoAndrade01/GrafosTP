/**
 * @file main.cpp
 * @brief Estudo de Caso 1: Comparação de consumo de memória com menu interativo.
 * @details O programa oferece um menu para o usuário escolher entre comparar as
 * implementações de Lista de Adjacência ou de Matriz de Adjacência. Em seguida,
 * itera sobre os 6 grafos de teste para o grupo escolhido.
 */

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "../../biblioteca/interface/Grafo.h"

 // --- Protótipos das funções ---
void exibirMenuPrincipal();
void executarTesteDeListas();
void executarTesteDeMatrizes();
void pausarParaContinuar();

/**
 * @brief Função principal que gerencia o menu e delega as tarefas.
 */
int main() {
    int escolha = -1;
    while (true) {
        exibirMenuPrincipal();
        std::cout << "Digite sua escolha: ";
        std::cin >> escolha;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            escolha = -1; // Força a entrada no 'else'
        }

        switch (escolha) {
        case 1:
            executarTesteDeListas();
            break;
        case 2:
            executarTesteDeMatrizes();
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
 * @brief Exibe o menu principal de opções.
 */
void exibirMenuPrincipal() {
    // system("cls"); // Descomente no Windows para limpar a tela
    // system("clear"); // Descomente no Linux/macOS para limpar a tela
    std::cout << "\n=======================================================" << std::endl;
    std::cout << "=== ESTUDO DE CASO 1: COMPARACAO DE MEMORIA ===" << std::endl;
    std::cout << "=======================================================" << std::endl;
    std::cout << "Escolha qual grupo de representacoes voce quer testar:" << std::endl;
    std::cout << "1. Comparar Listas de Adjacencia (Simples vs. Otimizada/CSR)" << std::endl;
    std::cout << "2. Comparar Matrizes de Adjacencia (Classica vs. Triangular)" << std::endl;
    std::cout << "-------------------------------------------------------" << std::endl;
    std::cout << "0. Sair" << std::endl;
    std::cout << "-------------------------------------------------------" << std::endl;
}

/**
 * @brief Executa o ciclo de testes para as duas representações de Lista.
 */
void executarTesteDeListas() {
    std::vector<std::string> nomesGrafos = { "grafo_1.txt", "grafo_2.txt", "grafo_3.txt", "grafo_4.txt", "grafo_5.txt", "grafo_6.txt" };

    std::cout << "\n--- Iniciando Teste de LISTAS DE ADJACENCIA ---" << std::endl;
    for (const auto& nomeBase : nomesGrafos) {
        std::string caminhoCompleto = "C:/Users/João - Dynatest/source/repos/GrafosTP/TP1/estudos/grafos_em_txt/" + nomeBase;
        std::cout << "\n--- Processando arquivo: " << caminhoCompleto << " ---\n" << std::endl;

        // Teste 1: Lista Simples
        try {
            {
                std::cout << "[LISTA 1/2] Carregando com LISTA SIMPLES..." << std::endl;
                Grafo grafo(caminhoCompleto, TipoRepresentacao::LISTA_ADJACENCIA_SIMPLES);
                std::cout << "  > Vertices: " << grafo.obterNumeroVertices() << " | Arestas: " << grafo.obterNumeroArestas() << std::endl;
                std::cout << "\n  >>> ACAO: Verifique a memoria. Pressione ENTER para continuar..." << std::endl;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
            }
        }
        catch (const std::exception& e) { std::cerr << "ERRO: " << e.what() << std::endl; }

        // Teste 2: Lista Otimizada (CSR)
        try {
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
        std::cout << "\n-------------------------------------------------" << std::endl;
    }
    std::cout << "\n--- Teste de Listas Concluido! ---" << std::endl;
    pausarParaContinuar();
}

/**
 * @brief Executa o ciclo de testes para as duas representações de Matriz.
 */
void executarTesteDeMatrizes() {
    std::vector<std::string> nomesGrafos = { "grafo_1.txt", "grafo_2.txt", "grafo_3.txt", "grafo_4.txt", "grafo_5.txt", "grafo_6.txt" };

    std::cout << "\n--- Iniciando Teste de MATRIZES DE ADJACENCIA ---" << std::endl;
    for (const auto& nomeBase : nomesGrafos) {
        std::string caminhoCompleto = "C:/Users/João - Dynatest/source/repos/GrafosTP/TP1/estudos/grafos_em_txt/" + nomeBase;
        std::cout << "\n--- Processando arquivo: " << caminhoCompleto << " ---\n" << std::endl;

        // Teste 1: Matriz Clássica
        try {
            {
                std::cout << "[MATRIZ 1/2] Carregando com MATRIZ CLASSICA..." << std::endl;
                Grafo grafo(caminhoCompleto, TipoRepresentacao::MATRIZ_ADJACENCIA);
                std::cout << "  > Vertices: " << grafo.obterNumeroVertices() << " | Arestas: " << grafo.obterNumeroArestas() << std::endl;
                std::cout << "\n  >>> ACAO: Verifique a memoria. Pressione ENTER para continuar..." << std::endl;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
            }
        }
        catch (const std::exception& e) { std::cerr << "ERRO: " << e.what() << std::endl; }

        // Teste 2: Matriz Triangular
        try {
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
        std::cout << "\n-------------------------------------------------" << std::endl;
    }
    std::cout << "\n--- Teste de Matrizes Concluido! ---" << std::endl;
    pausarParaContinuar();
}

/**
 * @brief Pausa a execução e espera o usuário pressionar Enter.
 */
void pausarParaContinuar() {
    std::cout << "\nPressione ENTER para voltar ao menu principal...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}