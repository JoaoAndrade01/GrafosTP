/**
 * @file estudo_caso_final.cpp
 * @brief Implementação do programa que responde às perguntas 4, 5, 6 e 7.
 */
#include "../estudos.h"
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <stdexcept>
#include <memory> // Para std::unique_ptr
#include "../../biblioteca/interface/Grafo.h"

static void pausarParaContinuar_EC_Final() {
    std::cout << "\nPressione ENTER para voltar ao menu...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void executarEstudoDeCasoFinal() {
    std::unique_ptr<Grafo> grafo;
    std::string nomeGrafoCarregado = "";
    int escolhaGrafo = 0;

    std::cout << "\n--- Estudo de Caso Final (Perguntas Analiticas) ---" << std::endl;
    std::cout << "Primeiro, escolha o grafo para analisar (1-6): ";
    std::cin >> escolhaGrafo;
    if (escolhaGrafo < 1 || escolhaGrafo > 6) { std::cout << "Escolha invalida." << std::endl; return; }

    try {
        std::string caminho = "C:/Users/João - Dynatest/source/repos/GrafosTP/TP1/estudos/grafos_em_txt/grafo_" + std::to_string(escolhaGrafo) + ".txt";
        nomeGrafoCarregado = "grafo_" + std::to_string(escolhaGrafo) + ".txt";
        grafo = std::make_unique<Grafo>(caminho, TipoRepresentacao::LISTA_ADJACENCIA);
        std::cout << "Grafo '" << nomeGrafoCarregado << "' carregado com sucesso." << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "ERRO ao carregar o grafo: " << e.what() << std::endl;
        return;
    }
    pausarParaContinuar_EC_Final();

    int escolhaPergunta = -1;
    while (escolhaPergunta != 0) {
        std::cout << "\n--- Analisando o grafo '" << nomeGrafoCarregado << "' ---" << std::endl;
        std::cout << "4. Obter pai dos vertices {10, 20, 30} com BFS e DFS" << std::endl;
        std::cout << "5. Obter distancia entre os pares {10,20}, {10,30}, {20,30}" << std::endl;
        std::cout << "6. Analisar Componentes Conexas" << std::endl;
        std::cout << "7. Calcular o Diametro" << std::endl;
        std::cout << "0. Voltar ao Menu Principal" << std::endl;
        std::cout << "Escolha: ";
        std::cin >> escolhaPergunta;

        switch (escolhaPergunta) {
        case 4: {
            std::cout << "\n--- Resposta da Pergunta 4 ---" << std::endl;
            std::vector<int> origens = { 1, 2, 3 };
            std::vector<int> alvos = { 10, 20, 30 };
            for (int origem : origens) {
                ResultadoBFS res_bfs = grafo->executarBFS(origem);
                ResultadoDFS res_dfs = grafo->executarDFS(origem);
                std::cout << "\nOrigem da busca: " << origem << std::endl;
                for (int alvo : alvos) {
                    std::cout << "  - Vertice " << alvo << ": Pai no BFS = " << res_bfs.pai[alvo] << ", Pai no DFS = " << res_dfs.pai[alvo] << std::endl;
                }
            }
            break;
        }
        case 5: {
            std::cout << "\n--- Resposta da Pergunta 5 ---" << std::endl;
            std::vector<std::pair<int, int>> pares = { {10, 20}, {10, 30}, {20, 30} };
            for (const auto& par : pares) {
                int dist = grafo->calcularDistancia(par.first, par.second);
                std::cout << "  - Distancia (" << par.first << ", " << par.second << "): " << dist << std::endl;
            }
            break;
        }
        case 6: {
            std::cout << "\n--- Resposta da Pergunta 6 ---" << std::endl;
            auto componentes = grafo->encontrarComponentesConexas();
            std::cout << "  - Numero de componentes conexas: " << componentes.size() << std::endl;
            if (!componentes.empty()) {
                std::cout << "  - Tamanho da MAIOR componente: " << componentes.front().tamanho << " vertices" << std::endl;
                std::cout << "  - Tamanho da MENOR componente: " << componentes.back().tamanho << " vertices" << std::endl;
            }
            break;
        }
        case 7: {
            std::cout << "\n--- Resposta da Pergunta 7 ---" << std::endl;
            std::cout << "1. Calcular Exato (lento)\n2. Calcular Aproximado (rapido)\nEscolha: ";
            int escolhaDiametro;
            std::cin >> escolhaDiametro;
            if (escolhaDiametro == 1) {
                std::cout << "Calculando diametro EXATO... (aguarde)" << std::endl;
                int diametro = grafo->calcularDiametro();
                std::cout << "  - O diametro exato do grafo e: " << diametro << std::endl;
            }
            else if (escolhaDiametro == 2) {
                std::cout << "Calculando diametro APROXIMADO com 10 iteracoes..." << std::endl;
                int diametroAprox = grafo->calcularDiametroAproximado(10);
                std::cout << "  - O diametro aproximado do grafo e: " << diametroAprox << std::endl;
            }
            break;
        }
        case 0: std::cout << "Voltando ao menu principal..." << std::endl; break;
        default: std::cout << "Opcao invalida." << std::endl; break;
        }
        if (escolhaPergunta != 0) pausarParaContinuar_EC_Final();
    }
}