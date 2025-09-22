/**
 * @file main.cpp
 * @brief Ponto de entrada principal que une todos os estudos de caso.
 */
#include <iostream>
#include <limits>
#include "estudos.h"
#include "../biblioteca/interface/Grafo.h"

void executarModoInterativo();

static void pausarFinal() {
    std::cout << "\nPressione ENTER para sair...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

int main() {
    int escolha = -1;

    while (escolha != 0) {
        std::cout << "\n=======================================================" << std::endl;
        std::cout << "=== PROGRAMA DE TESTES E ESTUDOS DE CASO ===" << std::endl;
        std::cout << "=======================================================" << std::endl;
        std::cout << "1. Estudo de Caso 1: Benchmark de Memoria" << std::endl;
        std::cout << "2. Estudo de Caso 2 & 3: Benchmark de Tempo (BFS/DFS)" << std::endl;
        std::cout << "3. Estudo de Caso Final (Perguntas Analiticas 4-7)" << std::endl;
        std::cout << "4. Modo Iterativo" << std::endl;
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
        case 1:
            executarEstudoDeCasoMemoria();
            break;
        case 2:
            executarEstudoDeCasoTempo();
            break;
        case 3:
            executarEstudoDeCasoFinal();
            break;
		case 4:
            executarModoInterativo();
			break;
        case 0:
            std::cout << "Encerrando o programa." << std::endl;
            break;
        default:
            std::cout << "\nOpcao invalida. Pressione ENTER para tentar novamente." << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            break;
        }
    }

    return 0;
}


void menuDeAnalise(Grafo& grafo, const std::string& nomeGrafo) {
    int escolha = -1;
    while (escolha != 0) {
        std::cout << "\n--- Analisando o grafo '" << nomeGrafo << "' ---" << std::endl;
        std::cout << "1. Calcular Distancia entre dois vertices" << std::endl;
        std::cout << "2. Calcular Diametro" << std::endl;
        std::cout << "3. Encontrar Componentes Conexas" << std::endl;
        std::cout << "4. Exibir Relatorio Completo no Console" << std::endl;
        std::cout << "0. Voltar (para escolher outro grafo)" << std::endl;
        std::cout << "Escolha uma analise: ";
        std::cin >> escolha;

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
            case 0:
                std::cout << "Voltando..." << std::endl;
                break;
            default:
                std::cout << "Opcao invalida." << std::endl;
                break;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "ERRO: " << e.what() << std::endl;
        }
        if (escolha != 0) { /* ... pausar para continuar ... */ }
    }
}


void executarModoInterativo() {
    while (true) {
        std::cout << "\n--- MODO INTERATIVO ---" << std::endl;
        std::cout << "Escolha um grafo (1-6) ou 0 para voltar: ";
        int escolhaGrafo;
        std::cin >> escolhaGrafo;
        if (escolhaGrafo == 0) break;
        if (escolhaGrafo < 1 || escolhaGrafo > 6) { std::cout << "Escolha invalida." << std::endl; continue; }

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
            std::string caminho = "C:/Users/João - Dynatest/source/repos/GrafosTP/TP1/estudos/grafos_em_txt/" + nomeGrafo;

            std::cout << "\nCarregando " << nomeGrafo << "..." << std::endl;
            Grafo grafo(caminho, tipo);
            std::cout << "Grafo carregado com sucesso." << std::endl;

            // Entra no menu de análise para este grafo
            menuDeAnalise(grafo, nomeGrafo);

        }
        catch (const std::exception& e) {
            std::cerr << "ERRO ao carregar o grafo: " << e.what() << std::endl;
        }
    }
}