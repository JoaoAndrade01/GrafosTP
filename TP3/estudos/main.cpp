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
void executarModoInterativo();
void menuAnaliseGrafo(GrafoPesado& grafo, const std::string& nomeGrafo);

// Auxiliares
std::string montarCaminhoArquivo(const std::string& nomeArquivo);
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
        std::cout << "3. Modo Interativo (Carregar grafo e analisar)\n";
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
        case 3: executarModoInterativo(); break;
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
        "grafo_W_1.txt", "grafo_W_2.txt", "grafo_W_3.txt", "grafo_W_4.txt", "grafo_W_5.txt"
    };
    std::vector<int> origens = { 10, 20, 30 };
    int destino = 100;

    // IMPORTANTE: Como não foi especificado se os grafos do estudo são direcionados,
    // assumiremos que são DIRECIONADOS para este TP3, ou perguntamos. 
    // Pelo contexto de "nova funcionalidade", vamos assumir DIRECIONADO = true.
    bool ehDirecionado = true;

    for (const auto& nomeGrafo : grafos) {
        std::cout << "Processando " << nomeGrafo << "..." << std::endl;
        try {
            std::string caminho = montarCaminhoArquivo(nomeGrafo);
            GrafoPesado grafo(caminho, ehDirecionado);

            // Bellman-Ford é usado pois é o foco do TP, mas Dijkstra também serviria se pesos >= 0.
            // Vamos usar Bellman-Ford para garantir.
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
        "grafo_W_1.txt", "grafo_W_2.txt", "grafo_W_3.txt", "grafo_W_4.txt", "grafo_W_5.txt"
    };

    bool ehDirecionado = true; // Assumindo direcionado para o teste
    int origemFixa = 10; // Vértice fixo para o teste de tempo (ou poderia ser aleatório)

    for (const auto& nomeGrafo : grafos) {
        std::cout << "Benchmarking " << nomeGrafo << "..." << std::flush;
        try {
            std::string caminho = montarCaminhoArquivo(nomeGrafo);
            GrafoPesado grafo(caminho, ehDirecionado);
            BellmanFord bf;

            std::vector<double> tempos;
            tempos.reserve(NUM_RODADAS_BELLMAN);

            for (int i = 0; i < NUM_RODADAS_BELLMAN; ++i) {
                auto inicio = std::chrono::high_resolution_clock::now();

                // Executa o algoritmo (sem contar tempo de I/O, pois grafo já está carregado)
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

        try {
            std::cout << "Carregando " << nomeGrafo << "..." << std::endl;
            std::string caminho = montarCaminhoArquivo(nomeGrafo);

            // Usa o novo construtor com parâmetro 'direcionado'
            GrafoPesado grafo(caminho, direcionado);

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
        std::cout << "\n--- Analise de " << nomeGrafo << " (" << (grafo.consultaDirecionado() ? "Direcionado" : "Nao Direcionado") << ") ---\n";
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
                // (Lógica similar de exibição...)
            }
        }

        if (opc != 0) pausar();
    }
}

// --- Funções Auxiliares ---

std::string montarCaminhoArquivo(const std::string& nomeArquivo) {
    namespace fs = std::filesystem;
    // Tenta encontrar a pasta grafos_em_txt subindo níveis se necessário
    // (Lógica simplificada para o exemplo, ajuste conforme sua estrutura de pastas real)
    fs::path atual = fs::current_path();

    // Procura em ../grafos_em_txt ou ../../grafos_em_txt
    if (fs::exists(atual / "grafos_em_txt" / nomeArquivo))
        return (atual / "grafos_em_txt" / nomeArquivo).string();

    if (fs::exists(atual / ".." / "grafos_em_txt" / nomeArquivo))
        return (atual / ".." / "grafos_em_txt" / nomeArquivo).string();

    // Caminho hardcoded para o TP1/estudos se necessário
    return "../../../TP1/estudos/grafos_em_txt/" + nomeArquivo;
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