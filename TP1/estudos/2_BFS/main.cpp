/**
 * @file main.cpp
 * @brief Programa final para executar os testes e responder às perguntas do trabalho.
 */

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <stdexcept>

#include "../../biblioteca/interface/Grafo.h"

 // --- Protótipos das Funções ---
void executarEstudoDeCasoFinal();
void pausarParaContinuar();

// Função principal simples que chama o menu do estudo de caso
int main() {
    executarEstudoDeCasoFinal();
    return 0;
}

/**
 * @brief Gerencia um menu para executar os testes que respondem às perguntas 4, 5, 6 e 7.
 */
void executarEstudoDeCasoFinal() {
    Grafo* grafo = nullptr;
    std::string nomeGrafoCarregado = "";
    int escolhaGrafo = 0;

    // Bloco: Carregamento do Grafo
    std::cout << "--- Estudo de Caso Final ---" << std::endl;
    std::cout << "Primeiro, escolha o grafo para analisar (1-6): ";
    std::cin >> escolhaGrafo;
    if (escolhaGrafo < 1 || escolhaGrafo > 6) { std::cout << "Escolha invalida." << std::endl; return; }

    try {
        std::string caminho = "C:/Users/João - Dynatest/source/repos/GrafosTP/TP1/estudos/grafos_em_txt/grafo_" + std::to_string(escolhaGrafo) + ".txt";
        nomeGrafoCarregado = "grafo_" + std::to_string(escolhaGrafo) + ".txt";
        // Usando a representação mais eficiente para garantir que todos os grafos carreguem
        grafo = new Grafo(caminho, TipoRepresentacao::LISTA_ADJACENCIA);
        std::cout << "Grafo '" << nomeGrafoCarregado << "' carregado com sucesso." << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "ERRO ao carregar o grafo: " << e.what() << std::endl;
        return;
    }
    pausarParaContinuar();


    // Bloco: Menu de Perguntas
    int escolhaPergunta = -1;
    while (escolhaPergunta != 0) {
        std::cout << "\n--- Analisando o grafo '" << nomeGrafoCarregado << "' ---" << std::endl;
        std::cout << "Qual pergunta voce quer responder?" << std::endl;
        std::cout << "4. Obter pai dos vertices {10, 20, 30} com BFS e DFS" << std::endl;
        std::cout << "5. Obter distancia entre os pares {10,20}, {10,30}, {20,30}" << std::endl;
        std::cout << "6. Analisar Componentes Conexas" << std::endl;
        std::cout << "7. Calcular o Diametro" << std::endl;
        std::cout << "0. Sair" << std::endl;
        std::cout << "Escolha: ";
        std::cin >> escolhaPergunta;

        switch (escolhaPergunta) {
        case 4: { // Teste Pais BFS/DFS
            std::cout << "\n--- Resposta da Pergunta 4 ---" << std::endl;
            std::vector<int> origens = { 1, 2, 3 };
            std::vector<int> alvos = { 10, 20, 30 };
            for (int origem : origens) {
                ResultadoBFS res_bfs = grafo->executarBFS(origem);
                ResultadoDFS res_dfs = grafo->executarDFS(origem);
                std::cout << "\nOrigem da busca: " << origem << std::endl;
                for (int alvo : alvos) {
                    std::cout << "  - Vertice " << alvo << ": Pai no BFS = " << res_bfs.pai[alvo]
                        << ", Pai no DFS = " << res_dfs.pai[alvo] << std::endl;
                }
            }
            break;
        }
        case 5: { // Teste Distâncias
            std::cout << "\n--- Resposta da Pergunta 5 ---" << std::endl;
            std::vector<std::pair<int, int>> pares = { {10, 20}, {10, 30}, {20, 30} };
            for (const auto& par : pares) {
                int dist = grafo->calcularDistancia(par.first, par.second);
                std::cout << "  - Distancia (" << par.first << ", " << par.second << "): " << dist << std::endl;
            }
            break;
        }
        case 6: { // Teste Componentes Conexas
            std::cout << "\n--- Resposta da Pergunta 6 ---" << std::endl;
            auto componentes = grafo->encontrarComponentesConexas();
            std::cout << "  - Numero de componentes conexas: " << componentes.size() << std::endl;
            if (!componentes.empty()) {
                std::cout << "  - Tamanho da MAIOR componente: " << componentes.front().tamanho << " vertices" << std::endl;
                std::cout << "  - Tamanho da MENOR componente: " << componentes.back().tamanho << " vertices" << std::endl;
            }
            break;
        }
        case 7: { // Teste Diâmetro
            std::cout << "\n--- Resposta da Pergunta 7 ---" << std::endl;
            std::cout << "O calculo do diametro exato pode ser EXTREMAMENTE demorado." << std::endl;
            std::cout << "1. Calcular Exato (lento)" << std::endl;
            std::cout << "2. Calcular Aproximado (rapido)" << std::endl;
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
        case 0:
            std::cout << "Encerrando..." << std::endl;
            break;
        default:
            std::cout << "Opcao invalida." << std::endl;
            break;
        }
        if (escolhaPergunta != 0) pausarParaContinuar();
    }

    // Libera a memória alocada para o grafo
    delete grafo;
}

/**
 * @brief Pausa a execução e espera o usuário pressionar Enter.
 */
void pausarParaContinuar() {
    std::cout << "\nPressione ENTER para voltar ao menu...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (std::cin.gcount() == 0) {
        std::cin.get();
    }
}