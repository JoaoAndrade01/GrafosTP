/**
 * @file main.cpp
 * @brief Ponto de entrada principal que une todos os estudos de caso.
 */
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <stdexcept>
#include <memory>    // Para std::unique_ptr
#include <chrono>    // Para medição de tempo
#include <random>    // Para números aleatórios
#include <numeric>   // Para std::accumulate
#include <algorithm> // Para std::min/max_element
#include <cmath>     // Para std::sqrt
#include <iomanip>   // Para std::setprecision
#include "estudos.h"
#include "../biblioteca/interface/Grafo.h"
 // --- Protótipos das Funções ---
void executarModoInterativo();
void menuDeAnalise(Grafo& grafo, const std::string& nomeGrafo);
void pausarParaContinuar();

// Estrutura para o resultado do benchmark, usada no modo interativo
struct ResultadoAmostra {
    double tempoTotalMs = 0.0, tempoMedioMs = 0.0, tempoMinMs = 0.0, tempoMaxMs = 0.0, desvioPadraoMs = 0.0;
};
// Função auxiliar para o benchmark interativo
static ResultadoAmostra executarAmostraInterativa(const Grafo& grafo, const std::string& algoritmo, int numExecucoes);


/**
 * @brief Função principal que gerencia o menu e delega as tarefas.
 */
int main() {
    int escolha = -1;

    while (escolha != 0) {
        std::cout << "\n=======================================================" << std::endl;
        std::cout << "=== PROGRAMA DE TESTES E ESTUDOS DE CASO ===" << std::endl;
        std::cout << "=======================================================" << std::endl;
        std::cout << "1. Estudo de Caso 1: Benchmark de Memoria" << std::endl;
        std::cout << "2. Estudo de Caso 2 & 3: Benchmark de Tempo (BFS/DFS)" << std::endl;
        std::cout << "3. Estudo de Caso Final (Perguntas Analiticas 4-7)" << std::endl;
        std::cout << "4. Modo Interativo de Analise" << std::endl;
        std::cout << "-------------------------------------------------------" << std::endl;
        std::cout << "0. Sair" << std::endl;
        std::cout << "-------------------------------------------------------" << std::endl;
        std::cout << "Digite sua escolha: ";
        std::cin >> escolha;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            escolha = -1;
        }

        switch (escolha) {
        case 1: executarEstudoDeCasoMemoria(); break;
        case 2: executarEstudoDeCasoTempo(); break;
        case 3: executarEstudoDeCasoFinal(); break;
        case 4: executarModoInterativo(); break;
        case 0: std::cout << "Encerrando o programa." << std::endl; break;
        default:
            std::cout << "\nOpcao invalida." << std::endl;
            pausarParaContinuar();
            break;
        }
    }

    return 0;
}


/**
 * @brief Pausa a execução e espera o usuário pressionar Enter.
 */
void pausarParaContinuar() {
    std::cout << "\nPressione ENTER para voltar ao menu principal...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (std::cin.gcount() == 0) {
        std::cin.get();
    }
}

/**
 * @brief Gerencia o menu de análise para um grafo já carregado.
 */
void menuDeAnalise(Grafo& grafo, const std::string& nomeGrafo) {
    int escolha = -1;
    while (escolha != 0) {
        std::cout << "\n--- Analisando o grafo '" << nomeGrafo << "' ---" << std::endl;
        std::cout << "1. Calcular Distancia entre dois vertices" << std::endl;
        std::cout << "2. Calcular Diametro (Aproximado)" << std::endl;
        std::cout << "3. Encontrar Componentes Conexas" << std::endl;
        std::cout << "4. Exibir Relatorio Completo no Console" << std::endl;
        std::cout << "5. Executar Benchmark de Performance (BFS/DFS)" << std::endl; // NOVA OPÇÃO
        std::cout << "0. Voltar (para escolher outro grafo)" << std::endl;
        std::cout << "Escolha uma analise: ";
        std::cin >> escolha;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            escolha = -1; // Invalida a escolha
        }

        try {
            switch (escolha) {
            case 1: {
                int u, v;
                std::cout << "Digite os dois vertices (ex: 10 20): ";
                std::cin >> u >> v;
                int dist = grafo.calcularDistancia(u, v);
                std::cout << "  > Resultado: A distancia entre " << u << " e " << v << " e: " << dist << std::endl;
                break;
            }
            case 2: {
                std::cout << "  > O diametro (aproximado) e: " << grafo.calcularDiametroAproximado() << std::endl;
                break;
            }
            case 3: {
                auto componentes = grafo.encontrarComponentesConexas();
                std::cout << "  > O grafo tem " << componentes.size() << " componente(s) conexa(s)." << std::endl;
                break;
            }
            case 4: {
                std::cout << "\n";
                grafo.gerarRelatorioCompleto(std::cout); // <-- Usando a nova função com std::cout
                break;
            }

            case 5: { // NOVA FUNCIONALIDADE DE BENCHMARK
                std::cout << "\n--- Benchmark de Performance Interativo ---" << std::endl;
                std::cout << "Qual algoritmo testar? (1 = BFS, 2 = DFS): ";
                int escolhaAlg;
                std::cin >> escolhaAlg;
                std::string nomeAlg = (escolhaAlg == 1) ? "BFS" : "DFS";

                std::cout << "Quantas execucoes (ex: 100): ";
                int numExecucoes;
                std::cin >> numExecucoes;

                if (numExecucoes > 0) {
                    std::cout << "Executando " << numExecucoes << " buscas com " << nomeAlg << "..." << std::flush;
                    ResultadoAmostra res = executarAmostraInterativa(grafo, nomeAlg, numExecucoes);
                    std::cout << " Concluido!" << std::endl;

                    std::cout << std::fixed << std::setprecision(4);
                    std::cout << "  > Tempo Medio: " << res.tempoMedioMs << " ms\n";
                    std::cout << "  > Tempo Minimo: " << res.tempoMinMs << " ms\n";
                    std::cout << "  > Tempo Maximo: " << res.tempoMaxMs << " ms\n";
                    std::cout << "  > Desvio Padrao: " << res.desvioPadraoMs << " ms\n";
                }
                break;
            }
            case 0: std::cout << "Voltando..." << std::endl; break;
            default: std::cout << "Opcao invalida." << std::endl; break;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "ERRO: " << e.what() << std::endl;
        }

        if (escolha != 0) {
            std::cout << "\nPressione ENTER para continuar...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (std::cin.gcount() == 0) std::cin.get();
        }
    }
}


/**
 * @brief Orquestra o modo interativo, permitindo carregar grafos e analisá-los.
 */
void executarModoInterativo() {
    while (true) {
        std::cout << "\n--- MODO INTERATIVO DE ANALISE ---" << std::endl;
        std::cout << "Escolha um grafo (1-6) ou 0 para voltar ao menu principal: ";
        int escolhaGrafo;
        std::cin >> escolhaGrafo;
        if (std::cin.fail() || escolhaGrafo == 0) { /* ... (código de saída) ... */ break; }
        if (escolhaGrafo < 1 || escolhaGrafo > 6) { /* ... (código de erro) ... */ continue; }

        std::cout << "\nEscolha a representacao:\n";
        std::cout << "1. Matriz Classica\n2. Matriz Triangular\n3. Lista Otimizada (CSR)\n4. Lista Simples\nEscolha: ";
        int escolhaRepr;
        std::cin >> escolhaRepr;

        TipoRepresentacao tipo;
        switch (escolhaRepr) {
        case 1: tipo = TipoRepresentacao::MATRIZ_ADJACENCIA; break;
        case 2: tipo = TipoRepresentacao::MATRIZ_ADJACENCIA_TRIANGULAR; break;
        case 3: tipo = TipoRepresentacao::LISTA_ADJACENCIA; break;
        case 4: tipo = TipoRepresentacao::LISTA_ADJACENCIA_SIMPLES; break;
        default: std::cout << "Escolha invalida." << std::endl; continue;
        }

        try {
            std::string nomeGrafo = "grafo_" + std::to_string(escolhaGrafo) + ".txt";
            std::string caminho = caminhoEstudos(nomeGrafo);

            std::cout << "\nCarregando " << nomeGrafo << "..." << std::endl;
            auto grafo = std::make_unique<Grafo>(caminho, tipo);
            std::cout << "Grafo carregado com sucesso." << std::endl;

            menuDeAnalise(*grafo, nomeGrafo);

        }
        catch (const std::exception& e) {
            std::cerr << "ERRO ao carregar ou analisar o grafo: " << e.what() << std::endl;
        }
    }
}


/**
 * @brief Executa uma "amostra" de benchmark (usado pelo modo interativo).
 */
static ResultadoAmostra executarAmostraInterativa(const Grafo& grafo, const std::string& algoritmo, int numExecucoes) {
    ResultadoAmostra resultado;
    if (numExecucoes <= 0) return resultado;

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
        double variancia = (numExecucoes > 1) ? (somaDosDesviosQuadrados / (numExecucoes - 1)) : 0.0;
        resultado.desvioPadraoMs = std::sqrt(variancia);
    }
    return resultado;
}

