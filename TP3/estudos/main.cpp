/**
 * @file main.cpp
 * @brief Programa principal do TP3: caminhos mínimos em grafos direcionados ponderados.
 *
 * Funcionalidades principais:
 *  - Escolher um dos 5 grafos grafo_W_X.txt (X = 1..5);
 *  - Exibir métricas básicas do grafo;
 *  - Calcular caminhos mínimos a partir de uma origem com:
 *      * Dijkstra (Heap ou Vetor), quando não há pesos negativos;
 *      * Bellman-Ford, mesmo com pesos negativos (sem ciclo negativo);
 *  - Comparar Dijkstra x Bellman-Ford a partir de uma origem e destino,
 *    quando o grafo não possui pesos negativos / ciclo negativo.
 */

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <stdexcept>
#include <memory>
#include <iomanip>
#include <chrono>
#include <filesystem>
#include <cmath>

#include "../biblioteca/interface/GrafoDirigidoPesado.h"
#include "../biblioteca/algoritmos/Dijkstra.h"
#include "../biblioteca/algoritmos/BellmanFord.h"

const double infinity = std::numeric_limits<double>::infinity();

// ----------------- Funções utilitárias -----------------

// Caminho absoluto até a pasta de grafos, semelhante ao TP2
inline std::string caminhoGrafos(const std::string& nomeArquivo) {
    namespace fs = std::filesystem;
    fs::path pathAtual(__FILE__);           // estudos/main.cpp
    fs::path pastaEstudos = pathAtual.parent_path();
    fs::path pastaGrafos = pastaEstudos / "grafos_em_txt";

    if (!fs::exists(pastaGrafos)) {
        // tenta subir um nível (caso a estrutura mude)
        pastaEstudos = pastaEstudos.parent_path();
        pastaGrafos = pastaEstudos / "grafos_em_txt";
    }

    if (!fs::exists(pastaGrafos)) {
        std::cerr << "AVISO: pasta 'grafos_em_txt' nao encontrada automaticamente." << std::endl;
        // fallback simples
        return "../grafos_em_txt/" + nomeArquivo;
    }

    return fs::absolute(pastaGrafos / nomeArquivo).lexically_normal().string();
}

void pausarParaContinuar() {
    std::cout << "\nPressione ENTER para continuar...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Reconstrói caminho a partir do vetor de pais (tanto para Dijkstra quanto Bellman-Ford)
std::vector<int> reconstruirCaminho(const std::vector<int>& pai, int origem, int destino) {
    std::vector<int> caminho;

    if (origem < 0 || destino < 0 ||
        origem >= static_cast<int>(pai.size()) ||
        destino >= static_cast<int>(pai.size())) {
        return caminho;
    }

    if (destino != origem && pai[destino] == -1) {
        // inalcançável
        return caminho;
    }

    int atual = destino;
    size_t limite = pai.size() + 5; // só para segurança contra loops
    while (atual != -1 && limite > 0) {
        caminho.push_back(atual);
        if (atual == origem) break;
        atual = pai[atual];
        --limite;
    }

    if (caminho.empty() || caminho.back() != origem) {
        // não conseguiu voltar até a origem
        return {};
    }

    std::reverse(caminho.begin(), caminho.end());
    return caminho;
}

std::string formatarCaminho(const std::vector<int>& caminho) {
    if (caminho.empty()) return "N/A";
    std::string s = std::to_string(caminho[0]);
    for (size_t i = 1; i < caminho.size(); ++i) {
        s += " -> " + std::to_string(caminho[i]);
    }
    return s;
}

// ----------------- Menus auxiliares -----------------

int escolherGrafo() {
    int escolha = -1;
    while (true) {
        std::cout << "\n=============================================\n";
        std::cout << " TP3 - Escolha do Grafo (grafo_W_X.txt)\n";
        std::cout << "=============================================\n";
        std::cout << "Informe o numero do grafo (1 a 5) ou 0 para voltar/sair: ";
        std::cin >> escolha;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entrada invalida.\n";
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (escolha == 0) {
            return 0;
        }
        if (escolha >= 1 && escolha <= 5) {
            return escolha;
        }
        std::cout << "Opcao invalida. Tente novamente.\n";
    }
}

// Pergunta uma origem válida
int lerVerticeOrigem(const GrafoDirigidoPesado& grafo) {
    int origem = -1;
    while (true) {
        std::cout << "Informe o vertice de origem (1.." << grafo.obterNumeroVertices() << "): ";
        std::cin >> origem;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entrada invalida.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (origem >= 1 && origem <= grafo.obterNumeroVertices()) {
            return origem;
        }
        std::cout << "Vertice invalido.\n";
    }
}

// Pergunta um destino (ou 0 para "todos")
int lerVerticeDestino(const GrafoDirigidoPesado& grafo) {
    int destino = -1;
    while (true) {
        std::cout << "Informe o vertice de destino (1.." << grafo.obterNumeroVertices()
                  << ") ou 0 para mostrar todas as distancias: ";
        std::cin >> destino;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entrada invalida.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (destino == 0) return 0;
        if (destino >= 1 && destino <= grafo.obterNumeroVertices()) {
            return destino;
        }
        std::cout << "Vertice invalido.\n";
    }
}

// ----------------- Ações de análise -----------------

void acaoExibirMetricas(const GrafoDirigidoPesado& grafo) {
    std::cout << "\n--- Metricas basicas do grafo ---\n";
    std::cout << "  > Numero de vertices: " << grafo.obterNumeroVertices() << "\n";
    std::cout << "  > Numero de arestas : " << grafo.obterNumeroArestas() << "\n";
    std::cout << "  > Possui peso negativo em alguma aresta? "
              << (grafo.temPesoNegativo() ? "Sim" : "Nao") << "\n";
}

// Usa Bellman-Ford só para verificar ciclo negativo (sem imprimir tudo)
void acaoVerificarCicloNegativo(const GrafoDirigidoPesado& grafo) {
    std::cout << "\n--- Verificacao de ciclo de peso negativo (Bellman-Ford) ---\n";
    if (grafo.obterNumeroVertices() <= 0) {
        std::cout << "Grafo sem vertices.\n";
        return;
    }

    // escolhe uma origem arbitraria (1) só para rodar BF
    int origem = 1;
    BellmanFord bf;
    auto resultado = bf.executar(grafo, origem);

    if (!resultado.temCicloNegativo) {
        std::cout << "Nao foi detectado ciclo de peso negativo.\n";
    } else {
        std::cout << "ATENCAO: Foi detectado pelo menos um ciclo de peso negativo!\n";
        // aqui poderia imprimir o ciclo se ele estiver disponível na struct
    }
}

// Calcula caminhos mínimos a partir de uma origem, com o algoritmo escolhido
void acaoCalcularCaminho(const GrafoDirigidoPesado& grafo) {
    if (grafo.obterNumeroVertices() <= 0) {
        std::cout << "Grafo vazio.\n";
        return;
    }

    int origem = lerVerticeOrigem(grafo);

    std::cout << "\nEscolha o algoritmo:\n";
    std::cout << "  1 - Dijkstra (Heap)\n";
    std::cout << "  2 - Dijkstra (Vetor)\n";
    std::cout << "  3 - Bellman-Ford\n";
    std::cout << "Opcao: ";

    int opcAlg = 0;
    std::cin >> opcAlg;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Entrada invalida.\n";
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Verificações para Dijkstra
    if ((opcAlg == 1 || opcAlg == 2) && grafo.temPesoNegativo()) {
        std::cout << "\nAVISO: O grafo possui pesos negativos. "
                     "Dijkstra nao deve ser usado nesse caso.\n";
        return;
    }

    int destino = lerVerticeDestino(grafo);

    // Execução
    Dijkstra dijkstraAlgo;
    BellmanFord bf;
    ResultadoDijkstra resDij;
    ResultadoBellmanFord resBF;
    bool usouDijkstra = false;
    bool usouBellmanFord = false;

    auto inicio = std::chrono::high_resolution_clock::now();

    try {
        if (opcAlg == 1) {
            resDij = dijkstraAlgo.executarHeap(grafo, origem);
            usouDijkstra = true;
        } else if (opcAlg == 2) {
            resDij = dijkstraAlgo.executarVetor(grafo, origem);
            usouDijkstra = true;
        } else if (opcAlg == 3) {
            resBF = bf.executar(grafo, origem);
            usouBellmanFord = true;
        } else {
            std::cout << "Opcao de algoritmo invalida.\n";
            return;
        }
    } catch (const std::exception& e) {
        std::cerr << "ERRO ao executar algoritmo: " << e.what() << "\n";
        return;
    }

    auto fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duracao = fim - inicio;
    std::cout << "\nTempo de execucao: " << duracao.count() << " ms\n";

    if (usouBellmanFord && resBF.temCicloNegativo) {
        std::cout << "ATENCAO: Bellman-Ford detectou ciclo de peso negativo. "
                     "As distancias podem ser invalidas.\n";
    }

    // Impressão dos resultados
    std::cout << std::fixed << std::setprecision(4);

    if (destino == 0) {
        // Mostrar distâncias para todos
        std::cout << "\nDistancias minimas a partir da origem " << origem << ":\n";
        int n = grafo.obterNumeroVertices();
        for (int v = 1; v <= n; ++v) {
            double dist = infinity;
            std::vector<int> pai;

            if (usouDijkstra) {
                dist = resDij.dist[v];
                pai = resDij.pai;
            } else if (usouBellmanFord) {
                dist = resBF.dist[v];
                pai = resBF.pai;
            }

            std::cout << "  Vertice " << v << ": ";
            if (dist == infinity) {
                std::cout << "Infinita (inalcancavel)\n";
            } else {
                std::cout << dist << " | Caminho: "
                          << formatarCaminho(reconstruirCaminho(pai, origem, v))
                          << "\n";
            }
        }
    } else {
        // Só um destino
        double dist = infinity;
        std::vector<int> pai;

        if (usouDijkstra) {
            dist = resDij.dist[destino];
            pai = resDij.pai;
        } else if (usouBellmanFord) {
            dist = resBF.dist[destino];
            pai = resBF.pai;
        }

        std::cout << "\nOrigem " << origem << " -> Destino " << destino << "\n";
        if (dist == infinity) {
            std::cout << "  Distancia: Infinita (inalcancavel)\n";
        } else {
            std::cout << "  Distancia: " << dist << "\n";
            auto cam = reconstruirCaminho(pai, origem, destino);
            std::cout << "  Caminho : " << formatarCaminho(cam) << "\n";
        }
    }
}

// ---------- Comparar Dijkstra x Bellman-Ford (com tempos e fator de diferença) ----------

void acaoCompararAlgoritmos(const GrafoDirigidoPesado& grafo) {
    if (grafo.obterNumeroVertices() <= 0) {
        std::cout << "Grafo vazio.\n";
        return;
    }

    if (grafo.temPesoNegativo()) {
        std::cout << "\nAVISO: O grafo possui pesos negativos.\n";
        std::cout << "      Nao faz sentido comparar diretamente Dijkstra com Bellman-Ford,\n"
                  << "      pois Dijkstra nao eh indicado nesse contexto.\n";
        return;
    }

    // 1) Escolher a versão de Dijkstra
    int opcAlg = 0;
    while (true) {
        std::cout << "\nEscolha a versao de Dijkstra para a comparacao:\n";
        std::cout << "  1 - Dijkstra (Heap)\n";
        std::cout << "  2 - Dijkstra (Vetor)\n";
        std::cout << "Opcao: ";
        std::cin >> opcAlg;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entrada invalida.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (opcAlg == 1 || opcAlg == 2) break;
        std::cout << "Opcao invalida. Escolha 1 ou 2.\n";
    }

    std::string nomeVersaoDij = (opcAlg == 1 ? "Dijkstra (Heap)" : "Dijkstra (Vetor)");

    // 2) Ler origem
    int origem = lerVerticeOrigem(grafo);

    // 3) Ler destino (sem opção 0 aqui)
    int destino = -1;
    while (true) {
        std::cout << "Informe o vertice de destino para a comparacao (1.."
                  << grafo.obterNumeroVertices() << "): ";
        std::cin >> destino;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entrada invalida.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (destino >= 1 && destino <= grafo.obterNumeroVertices()) break;
        std::cout << "Vertice invalido.\n";
    }

    Dijkstra dijkstraAlgo;
    BellmanFord bf;
    ResultadoDijkstra resDij;
    ResultadoBellmanFord resBF;

    double tempoDijMs = 0.0;
    double tempoBFMs  = 0.0;

    try {
        // Tempo de Dijkstra
        auto inicioDij = std::chrono::high_resolution_clock::now();
        if (opcAlg == 1) {
            resDij = dijkstraAlgo.executarHeap(grafo, origem);
        } else {
            resDij = dijkstraAlgo.executarVetor(grafo, origem);
        }
        auto fimDij = std::chrono::high_resolution_clock::now();
        tempoDijMs = std::chrono::duration<double, std::milli>(fimDij - inicioDij).count();

        // Tempo de Bellman-Ford
        auto inicioBF = std::chrono::high_resolution_clock::now();
        resBF  = bf.executar(grafo, origem);
        auto fimBF = std::chrono::high_resolution_clock::now();
        tempoBFMs = std::chrono::duration<double, std::milli>(fimBF - inicioBF).count();

    } catch (const std::exception& e) {
        std::cerr << "ERRO ao executar algoritmos: " << e.what() << "\n";
        return;
    }

    if (resBF.temCicloNegativo) {
        std::cout << "ATENCAO: Bellman-Ford detectou ciclo de peso negativo.\n"
                     "Comparacao de distancias pode nao ser valida.\n";
    }

    std::cout << std::fixed << std::setprecision(6);

    double dDij = resDij.dist[destino];
    double dBF  = resBF.dist[destino];

    auto camDij = reconstruirCaminho(resDij.pai, origem, destino);
    auto camBF  = reconstruirCaminho(resBF.pai, origem, destino);

    std::string sDij = (dDij == infinity) ? "Inf" : std::to_string(dDij);
    std::string sBF  = (dBF == infinity) ? "Inf" : std::to_string(dBF);

    std::string status;
    if (dDij == infinity && dBF == infinity) {
        status = "Ambos inalcancaveis";
    } else if (std::fabs(dDij - dBF) < 1e-6) {
        status = "OK (iguais)";
    } else {
        status = "DIFERENTE";
    }

    std::cout << "\nComparacao de distancias entre "
              << nomeVersaoDij << " e Bellman-Ford\n";
    std::cout << "Origem = " << origem << ", Destino = " << destino << "\n\n";

    std::cout << "  " << nomeVersaoDij << ":\n";
    if (dDij == infinity) {
        std::cout << "    Distancia: Infinita (inalcancavel)\n";
    } else {
        std::cout << "    Distancia: " << sDij << "\n";
        std::cout << "    Caminho  : " << formatarCaminho(camDij) << "\n";
    }
    std::cout << "    Tempo de execucao: " << tempoDijMs << " ms\n\n";

    std::cout << "  Bellman-Ford:\n";
    if (dBF == infinity) {
        std::cout << "    Distancia: Infinita (inalcancavel)\n";
    } else {
        std::cout << "    Distancia: " << sBF << "\n";
        std::cout << "    Caminho  : " << formatarCaminho(camBF) << "\n";
    }
    std::cout << "    Tempo de execucao: " << tempoBFMs << " ms\n\n";

    std::cout << "  Status da comparacao: " << status << "\n";

    // ---- Fator de diferença de tempo ----
    if (tempoDijMs > 0.0 && tempoBFMs > 0.0) {
        double fator = 1.0;
        std::string maisRapido;

        if (tempoDijMs < tempoBFMs) {
            fator = tempoBFMs / tempoDijMs;
            maisRapido = nomeVersaoDij + " mais rapido que Bellman-Ford";
        } else if (tempoBFMs < tempoDijMs) {
            fator = tempoDijMs / tempoBFMs;
            maisRapido = "Bellman-Ford mais rapido que " + nomeVersaoDij;
        } else {
            std::cout << "  Fator de diferenca de tempo: tempos praticamente iguais.\n";
            return;
        }

        std::cout << std::setprecision(3);
        std::cout << "  Fator de diferenca de tempo: "
                  << fator << "x (" << maisRapido << ")\n";
    } else {
        std::cout << "  Nao foi possivel calcular o fator de diferenca de tempo.\n";
    }
}

// ----------------- Menu para um grafo carregado -----------------

void menuAnaliseGrafo(GrafoDirigidoPesado& grafo, const std::string& nomeGrafo) {
    int opc = -1;
    while (opc != 0) {
        std::cout << "\n=============================================\n";
        std::cout << " Analise do grafo '" << nomeGrafo << "'\n";
        std::cout << "=============================================\n";
        std::cout << "1 - Exibir metricas basicas\n";
        std::cout << "2 - Verificar ciclo de peso negativo (Bellman-Ford)\n";
        std::cout << "3 - Calcular caminhos minimos a partir de uma origem\n";
        std::cout << "4 - Comparar Dijkstra x Bellman-Ford (origem/destino)\n";
        std::cout << "0 - Voltar ao menu de escolha de grafo\n";
        std::cout << "Opcao: ";

        std::cin >> opc;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entrada invalida.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (opc) {
        case 1:
            acaoExibirMetricas(grafo);
            pausarParaContinuar();
            break;
        case 2:
            acaoVerificarCicloNegativo(grafo);
            pausarParaContinuar();
            break;
        case 3:
            acaoCalcularCaminho(grafo);
            pausarParaContinuar();
            break;
        case 4:
            acaoCompararAlgoritmos(grafo);
            pausarParaContinuar();
            break;
        case 0:
            std::cout << "Retornando ao menu anterior...\n";
            break;
        default:
            std::cout << "Opcao invalida.\n";
            pausarParaContinuar();
            break;
        }
    }
}

// ----------------- Função principal -----------------

int main() {
    int escolhaGrafo = -1;

    while (true) {
        escolhaGrafo = escolherGrafo();
        if (escolhaGrafo == 0) {
            std::cout << "Encerrando o programa.\n";
            break;
        }

        std::string nomeArquivo = "grafo_W_" + std::to_string(escolhaGrafo) + ".txt";
        std::string caminho = caminhoGrafos(nomeArquivo);

        try {
            std::cout << "\nCarregando " << nomeArquivo << "...\n";
            GrafoDirigidoPesado grafo(caminho);
            std::cout << "Grafo carregado com sucesso ("
                      << grafo.obterNumeroVertices() << " vertices, "
                      << grafo.obterNumeroArestas() << " arestas).\n";

            menuAnaliseGrafo(grafo, nomeArquivo);
        } catch (const std::exception& e) {
            std::cerr << "ERRO ao carregar o grafo: " << e.what() << "\n";
            pausarParaContinuar();
        }
    }

    return 0;
}
