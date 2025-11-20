/**
 * @file main.cpp
 * @brief Programa principal para o TP3: Grafos Direcionados, Ponderados e Bellman-Ford.
 */

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <stdexcept>
#include <memory>    // Para std::unique_ptr
#include <iomanip>   // Para std::setprecision
#include <chrono>    // Para medição de tempo
#include <numeric>   // Para std::accumulate
#include <algorithm> // Para std::min/max_element, std::reverse
#include <cmath>     // Para std::sqrt
#include <filesystem>// Para manipulação de caminhos
#include <fstream>   // Para arquivos
#include <random>    // Para geração de números aleatórios

 // Includes da biblioteca TP3 (ajustados para o caminho correto)
#include "../biblioteca/interface/GrafoPesado.h"
#include "../biblioteca/algoritmos/BellmanFord.h"
#include "../biblioteca/algoritmos/Dijkstra.h"

// --- Constantes ---
const int NUM_RODADAS_BELLMAN = 10; // Conforme especificado no estudo de caso 2
const double infinity = std::numeric_limits<double>::infinity();

// Estrutura para métricas de tempo
struct MetricasTempo {
    double tempoTotal = 0.0;
    double tempoMedio = 0.0;
    double tempoMin = infinity;
    double tempoMax = 0.0;
    double desvioPadrao = 0.0;
};

// --- Protótipos ---
void executarEstudoCaso1_DistanciasEspecificas();
void executarEstudoCaso2_BenchmarkBellmanFord();
void executarEstudoCaso3_GrafoTransposto(); // <-- NOVO
void executarModoInterativo();
void menuAnaliseGrafo(GrafoPesado& grafo, const std::string& nomeGrafo);

// Auxiliares
std::string caminhoEstudos(const std::string& nomeArquivo);
std::vector<int> reconstruirCaminho(const std::vector<int>& pai, int origem, int destino);
std::string formatarCaminho(const std::vector<int>& caminho);
void pausar();

// --- MAIN ---
int main() {
    // Configuração de locale para acentuação (opcional, depende do sistema)
    std::setlocale(LC_ALL, "");

    int escolha = -1;
    while (escolha != 0) {
        std::cout << "\n=======================================================\n";
        std::cout << "=== TP3: Grafos Direcionados e Bellman-Ford ===\n";
        std::cout << "=======================================================\n";
        std::cout << "1. Estudo de Caso 1: Distancias (10, 20, 30 -> 100)\n";
        std::cout << "2. Estudo de Caso 2: Benchmark Bellman-Ford (Tempo)\n";
        std::cout << "3. Estudo de Caso 3: Grafo Transposto (Dijkstra Heap)\n"; // <-- NOVO
        std::cout << "4. Modo Interativo (Carregar grafo e analisar)\n";
        std::cout << "0. Sair\n";
        std::cout << "-------------------------------------------------------\n";
        std::cout << "Escolha: ";
        std::cin >> escolha;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (escolha) {
        case 1: executarEstudoCaso1_DistanciasEspecificas(); break;
        case 2: executarEstudoCaso2_BenchmarkBellmanFord(); break;
        case 3: executarEstudoCaso3_GrafoTransposto(); break; // <-- NOVO
        case 4: executarModoInterativo(); break;
        case 0: std::cout << "Saindo...\n"; break;
        default: std::cout << "Opcao invalida.\n"; pausar(); break;
        }
    }
    return 0;
}

// --- Implementações dos Estudos de Caso ---

/**
 * @brief Estudo de Caso 1:
 * Para cada grafo, determinar a distância dos vértices 10, 20 e 30 para o vértice 100.
 * Salvar em CSV.
 */
void executarEstudoCaso1_DistanciasEspecificas() {
    std::cout << "\n--- Estudo de Caso 1: Distancias para o Vertice 100 ---\n";

    std::ofstream arquivoCSV("resultados_tp3_estudo1.csv");
    if (!arquivoCSV.is_open()) {
        std::cerr << "Erro ao criar arquivo CSV.\n";
        return;
    }
    arquivoCSV << "Grafo,Origem,Destino,Distancia,Caminho\n";
    arquivoCSV << std::fixed << std::setprecision(4);

    std::vector<std::string> grafos = {
        /*"grafo_W_1.txt",*/ "grafo_W_2.txt", "grafo_W_3.txt", "grafo_W_4.txt", "grafo_W_5.txt"
    };
    std::vector<int> origens = { 10, 20, 30 };
    int destino = 100;

    bool ehDirecionado = true;

    for (const auto& nomeGrafo : grafos) {
        std::cout << "Processando " << nomeGrafo << "..." << std::endl;
        try {
            std::string caminho = caminhoEstudos(nomeGrafo);
            GrafoPesado grafo(caminho, ehDirecionado);

            BellmanFord bf;

            for (int origem : origens) {
                if (origem > grafo.obterNumeroVertices() || destino > grafo.obterNumeroVertices()) {
                    arquivoCSV << nomeGrafo << "," << origem << "," << destino << ",N/A,Vertice Invalido\n";
                    continue;
                }

                // Executa o algoritmo
                auto resultado = bf.executar(grafo, origem);

                // Processa resultado para o destino específico
                double dist = resultado.dist[destino];
                std::string distStr = (dist == infinity) ? "INF" : std::to_string(dist);

                std::string caminhoStr = "N/A";
                if (dist != infinity) {
                    auto caminho = reconstruirCaminho(resultado.pai, origem, destino);
                    caminhoStr = formatarCaminho(caminho);
                }

                if (resultado.temCicloNegativo) {
                    distStr += " (Ciclo Negativo)";
                }

                arquivoCSV << nomeGrafo << "," << origem << "," << destino << "," << distStr << ",\"" << caminhoStr << "\"\n";
            }

        }
        catch (const std::exception& e) {
            std::cerr << "Erro no grafo " << nomeGrafo << ": " << e.what() << "\n";
            for (int origem : origens) {
                arquivoCSV << nomeGrafo << "," << origem << "," << destino << ",ERRO," << e.what() << "\n";
            }
        }
    }

    std::cout << "Resultados salvos em 'resultados_tp3_estudo1.csv'.\n";
    pausar();
}

/**
 * @brief Estudo de Caso 2:
 * Calcular tempo médio do Bellman-Ford (10 rodadas). Salvar métricas em CSV.
 */
void executarEstudoCaso2_BenchmarkBellmanFord() {
    std::cout << "\n--- Estudo de Caso 2: Benchmark Bellman-Ford ---\n";

    std::ofstream arquivoCSV("resultados_tp3_estudo2_tempo.csv");
    if (!arquivoCSV.is_open()) {
        std::cerr << "Erro ao criar arquivo CSV.\n";
        return;
    }
    arquivoCSV << "Grafo,Algoritmo,Direcionado,NumRodadas,TempoTotal_s,TempoMedio_s,TempoMin_s,TempoMax_s,DesvioPadrao_s\n";
    arquivoCSV << std::fixed << std::setprecision(6);

    std::vector<std::string> grafos = {
        /*"grafo_W_1.txt",*/ "grafo_W_2.txt", "grafo_W_3.txt", "grafo_W_4.txt", "grafo_W_5.txt"
    };

    bool ehDirecionado = true; // Assumindo direcionado para o teste
    int origemFixa = 10; // Vértice fixo para o teste de tempo (ou poderia ser aleatório)

    for (const auto& nomeGrafo : grafos) {
        std::cout << "Benchmarking " << nomeGrafo << "..." << std::flush;
        try {
            std::string caminho = caminhoEstudos(nomeGrafo);
            GrafoPesado grafo(caminho, ehDirecionado);
            BellmanFord bf;

            std::vector<double> tempos;
            tempos.reserve(NUM_RODADAS_BELLMAN);

            for (int i = 0; i < NUM_RODADAS_BELLMAN; ++i) {
                auto inicio = std::chrono::high_resolution_clock::now();

                // Executa o algoritmo
                bf.executar(grafo, origemFixa);

                auto fim = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duracao = fim - inicio; // em segundos
                tempos.push_back(duracao.count());
            }

            // Cálculos Estatísticos
            double soma = std::accumulate(tempos.begin(), tempos.end(), 0.0);
            double media = soma / NUM_RODADAS_BELLMAN;
            double min = *std::min_element(tempos.begin(), tempos.end());
            double max = *std::max_element(tempos.begin(), tempos.end());

            double somaQuad = 0.0;
            for (double t : tempos) somaQuad += (t - media) * (t - media);
            double desvio = std::sqrt(somaQuad / NUM_RODADAS_BELLMAN);

            // Salvar no CSV
            arquivoCSV << nomeGrafo << ",BellmanFord," << (ehDirecionado ? "Sim" : "Nao") << ","
                << NUM_RODADAS_BELLMAN << "," << soma << "," << media << ","
                << min << "," << max << "," << desvio << "\n";

            std::cout << " OK (Media: " << media << "s)\n";

        }
        catch (const std::exception& e) {
            std::cerr << " Erro: " << e.what() << "\n";
            arquivoCSV << nomeGrafo << ",ERRO,,,,,,,\n";
        }
    }

    std::cout << "Resultados salvos em 'resultados_tp3_estudo2_tempo.csv'.\n";
    pausar();
}

/**
 * @brief Estudo de Caso 3:
 * Parte 1: Grafo Transposto, Dijkstra Heap de 100 para 10, 20, 30.
 * Parte 2: Benchmark de tempo Dijkstra Heap Transposto (10 execuções aleatórias).
 */
void executarEstudoCaso3_GrafoTransposto() {
    std::cout << "\n--- Estudo de Caso 3: Grafo Transposto e Dijkstra ---\n";

    // Arquivo para Parte 1: Distâncias e Caminhos
    std::ofstream csvDist("resultados_tp3_estudo3_distancia.csv");
    if (!csvDist.is_open()) {
        std::cerr << "Erro ao criar CSV de distancias.\n";
        return;
    }
    csvDist << "Grafo,Origem_Transp,Destino_Transp,Distancia,Caminho\n";
    csvDist << std::fixed << std::setprecision(4);

    // Arquivo para Parte 2: Tempo de Execução
    std::ofstream csvTempo("resultados_tp3_estudo3_tempo.csv");
    if (!csvTempo.is_open()) {
        std::cerr << "Erro ao criar CSV de tempo.\n";
        return;
    }
    csvTempo << "Grafo,Algoritmo,NumExecucoes,TempoTotal_s,TempoMedio_s,TempoMin_s,TempoMax_s,DesvioPadrao_s\n";
    csvTempo << std::fixed << std::setprecision(6);

    // Grafos a serem analisados
    std::vector<std::string> grafos = {
        "grafo_W_2.txt", "grafo_W_3.txt", "grafo_W_4.txt", "grafo_W_5.txt"
    };

    // Parâmetros do estudo
    const int origemParte1 = 100; // No grafo original é destino, no transposto vira origem
    const std::vector<int> destinosParte1 = { 10, 20, 30 }; // No grafo original são origens, no transposto viram destinos
    const int numExecucoesBenchmark = 10;

    for (const auto& nomeGrafo : grafos) {
        std::cout << "Processando " << nomeGrafo << " (Transposto)..." << std::endl;
        try {
            std::string caminho = caminhoEstudos(nomeGrafo);

            // Carrega grafo: Direcionado = true, Transposto = true
            GrafoPesado grafo(caminho, true, true);

            if (grafo.temPesoNegativo()) {
                std::cout << "  [AVISO] Grafo tem pesos negativos. Dijkstra pode falhar se houver arestas negativas alcancaveis.\n";
            }

            // --- PARTE 1: Distâncias e Caminhos ---
            std::cout << "  [1/2] Calculando distancias de " << origemParte1 << " para {10, 20, 30}..." << std::flush;

            Dijkstra dij;
            // Executa Dijkstra a partir do 100 no grafo transposto
            auto resultado = dij.executarHeap(grafo, origemParte1);

            for (int dest : destinosParte1) {
                if (dest > grafo.obterNumeroVertices()) {
                    csvDist << nomeGrafo << "," << origemParte1 << "," << dest << ",N/A,Vertice Invalido\n";
                    continue;
                }

                double dist = resultado.dist[dest];
                std::string distStr = (dist == infinity) ? "INF" : std::to_string(dist);
                std::string caminhoStr = "N/A";

                if (dist != infinity) {
                    // Reconstrói o caminho 100 -> ... -> Destino no grafo transposto
                    auto caminho = reconstruirCaminho(resultado.pai, origemParte1, dest);
                    caminhoStr = formatarCaminho(caminho);
                }

                csvDist << nomeGrafo << "," << origemParte1 << "," << dest << "," << distStr << ",\"" << caminhoStr << "\"\n";
            }
            std::cout << " OK.\n";


            // --- PARTE 2: Benchmark de Tempo ---
            std::cout << "  [2/2] Executando benchmark (" << numExecucoesBenchmark << " rodadas)..." << std::flush;

            // Gerador de números aleatórios
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(1, grafo.obterNumeroVertices());

            std::vector<double> tempos;
            tempos.reserve(numExecucoesBenchmark);

            for (int i = 0; i < numExecucoesBenchmark; ++i) {
                int origemAleatoria = distrib(gen);

                auto inicio = std::chrono::high_resolution_clock::now();
                dij.executarHeap(grafo, origemAleatoria);
                auto fim = std::chrono::high_resolution_clock::now();

                std::chrono::duration<double> duracao = fim - inicio; // Segundos
                tempos.push_back(duracao.count());
            }

            // Cálculos Estatísticos
            double soma = std::accumulate(tempos.begin(), tempos.end(), 0.0);
            double media = soma / numExecucoesBenchmark;
            double min = *std::min_element(tempos.begin(), tempos.end());
            double max = *std::max_element(tempos.begin(), tempos.end());

            double somaQuad = 0.0;
            for (double t : tempos) somaQuad += (t - media) * (t - media);
            double desvio = (numExecucoesBenchmark > 1) ? std::sqrt(somaQuad / (numExecucoesBenchmark - 1)) : 0.0;

            csvTempo << nomeGrafo << ",DijkstraHeap_Transposto," << numExecucoesBenchmark << ","
                << soma << "," << media << "," << min << "," << max << "," << desvio << "\n";

            std::cout << " OK (Media: " << media << "s)\n";

        }
        catch (const std::exception& e) {
            std::cerr << "\n  ERRO no grafo " << nomeGrafo << ": " << e.what() << "\n";
            // Registra erro nos CSVs
            csvDist << nomeGrafo << ",ERROR,ERROR,ERROR," << e.what() << "\n";
            csvTempo << nomeGrafo << ",ERROR," << numExecucoesBenchmark << ",,,,,,\n";
        }
    }

    std::cout << "\nEstudo de Caso 3 concluido.\n";
    std::cout << "Arquivos gerados:\n - resultados_tp3_estudo3_distancia.csv\n - resultados_tp3_estudo3_tempo.csv\n";
    pausar();
}


// --- Modo Interativo ---

void executarModoInterativo() {
    while (true) {
        std::cout << "\n--- Modo Interativo ---\n";
        std::cout << "Informe o numero do grafo (1-5) ou 0 para voltar: ";
        int num;
        std::cin >> num;
        if (num == 0) break;
        if (num < 1 || num > 5) { std::cout << "Invalido.\n"; continue; }

        std::string nomeGrafo = "grafo_W_" + std::to_string(num) + ".txt";

        // Pergunta sobre direcionamento (Nova funcionalidade)
        int dirOpt;
        std::cout << "O grafo deve ser tratado como DIRECIONADO? (1-Sim / 0-Nao): ";
        std::cin >> dirOpt;
        bool direcionado = (dirOpt == 1);

        // Pergunta sobre transposição (Nova funcionalidade para TP3)
        int transpOpt = 0;
        if (direcionado) {
            std::cout << "O grafo deve ser carregado TRANSPOSTO (invertido)? (1-Sim / 0-Nao): ";
            std::cin >> transpOpt;
        }
        bool transposto = (transpOpt == 1);


        try {
            std::cout << "Carregando " << nomeGrafo << (transposto ? " [TRANSPOSTO]" : "") << "..." << std::endl;
            std::string caminho = caminhoEstudos(nomeGrafo);

            // Usa o novo construtor com parâmetro 'direcionado' e 'transposto'
            GrafoPesado grafo(caminho, direcionado, transposto);

            std::cout << "Grafo carregado com sucesso!\n";
            if (grafo.temPesoNegativo()) std::cout << "AVISO: Grafo contem pesos negativos.\n";

            menuAnaliseGrafo(grafo, nomeGrafo);

        }
        catch (const std::exception& e) {
            std::cerr << "Erro: " << e.what() << "\n";
            pausar();
        }
    }
}

void menuAnaliseGrafo(GrafoPesado& grafo, const std::string& nomeGrafo) {
    int opc = -1;
    while (opc != 0) {
        std::cout << "\n--- Analise de " << nomeGrafo << " (" << (grafo.consultaDirecionado() ? "Direcionado" : "Nao Direcionado")
            << (grafo.consultaTransposto() ? ", Transposto" : "") << ") ---\n";
        std::cout << "1. Exibir Informacoes Basicas\n";
        std::cout << "2. Executar Bellman-Ford\n";
        std::cout << "3. Executar Dijkstra (Se possivel)\n";
        std::cout << "0. Voltar\n";
        std::cout << "Escolha: ";
        std::cin >> opc;

        if (opc == 1) {
            std::cout << "Vertices: " << grafo.obterNumeroVertices() << "\n";
            std::cout << "Arestas: " << grafo.obterNumeroArestas() << "\n";
            std::cout << "Tem peso negativo: " << (grafo.temPesoNegativo() ? "Sim" : "Nao") << "\n";
        }
        else if (opc == 2) {
            int origem;
            std::cout << "Vertice de origem: ";
            std::cin >> origem;
            try {
                BellmanFord bf;
                auto inicio = std::chrono::high_resolution_clock::now();
                auto res = bf.executar(grafo, origem);
                auto fim = std::chrono::high_resolution_clock::now();

                std::cout << "Bellman-Ford concluido em "
                    << std::chrono::duration<double, std::milli>(fim - inicio).count() << " ms.\n";

                if (res.temCicloNegativo) std::cout << "ALERTA: Ciclo negativo detectado!\n";

                std::cout << "Mostrar distancia para um destino (0 para nenhum): ";
                int dest; std::cin >> dest;
                if (dest > 0 && dest <= grafo.obterNumeroVertices()) {
                    std::cout << "Distancia: " << res.dist[dest] << "\n";
                    auto caminho = reconstruirCaminho(res.pai, origem, dest);
                    std::cout << "Caminho: " << formatarCaminho(caminho) << "\n";
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Erro: " << e.what() << "\n";
            }
        }
        else if (opc == 3) {
            if (grafo.temPesoNegativo()) {
                std::cout << "Erro: Grafo tem pesos negativos. Dijkstra nao aplicavel.\n";
            }
            else {
                int origem;
                std::cout << "Vertice de origem: ";
                std::cin >> origem;
                Dijkstra dij;
                // Exemplo usando Heap
                auto res = dij.executarHeap(grafo, origem);
                std::cout << "Dijkstra (Heap) concluido.\n";

                std::cout << "Mostrar distancia para um destino (0 para nenhum): ";
                int dest; std::cin >> dest;
                if (dest > 0 && dest <= grafo.obterNumeroVertices()) {
                    std::cout << "Distancia: " << res.dist[dest] << "\n";
                    auto caminho = reconstruirCaminho(res.pai, origem, dest);
                    std::cout << "Caminho: " << formatarCaminho(caminho) << "\n";
                }
            }
        }

        if (opc != 0) pausar();
    }
}

// --- Funções Auxiliares ---

// --- Função Utilitária para Caminhos ---
// Função para obter o caminho absoluto dos grafos na pasta esperada
inline std::string caminhoEstudos(const std::string& nomeArquivo) {
    namespace fs = std::filesystem;
    fs::path pathDoArquivoAtual(__FILE__);
    fs::path pastaEstudos = pathDoArquivoAtual.parent_path(); // Assume que main.cpp está em estudos/
    fs::path pastaGrafos = pastaEstudos / "grafos_em_txt"; // Ajuste se necessário
    if (!fs::exists(pastaGrafos)) { // Tenta subir um nível se não encontrar
        pastaEstudos = pastaEstudos.parent_path();
        pastaGrafos = pastaEstudos / "grafos_em_txt";
    }
    if (!fs::exists(pastaGrafos)) {
        std::cerr << "AVISO: Nao foi possivel encontrar a pasta 'grafos_em_txt' automaticamente." << std::endl;
        // Tenta um caminho relativo padrão como fallback
        return "../grafos_em_txt/" + nomeArquivo;
    }
    // Retorna caminho absoluto e normalizado para consistência
    return fs::absolute(pastaGrafos / nomeArquivo).lexically_normal().string();
}

void pausar() {
    std::cout << "\nPressione ENTER para continuar...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

std::vector<int> reconstruirCaminho(const std::vector<int>& pai, int origem, int destino) {
    std::vector<int> caminho;
    if (destino == -1 || pai[destino] == -1) return caminho; // Inalcançável

    for (int v = destino; v != 0; v = pai[v]) {
        caminho.push_back(v);
        if (v == origem) break;
        if (caminho.size() > pai.size()) return {}; // Proteção contra ciclo infinito
    }
    std::reverse(caminho.begin(), caminho.end());

    if (caminho.empty() || caminho[0] != origem) return {}; // Caminho inválido
    return caminho;
}

std::string formatarCaminho(const std::vector<int>& caminho) {
    if (caminho.empty()) return "Inexistente";
    std::string s = "";
    for (size_t i = 0; i < caminho.size(); ++i) {
        s += std::to_string(caminho[i]);
        if (i < caminho.size() - 1) s += " -> ";
    }
    return s;
}