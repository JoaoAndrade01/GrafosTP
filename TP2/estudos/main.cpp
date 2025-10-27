/**
 * @file main.cpp
 * @brief Programa principal para estudos de caso e análise de grafos ponderados (TP2).
 */
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <stdexcept>
#include <memory>    // Para std::unique_ptr
#include <iomanip>   // Para std::setprecision
#include <chrono>    // Para medição de tempo
#include <random>    // Para números aleatórios
#include <numeric>   // Para std::accumulate
#include <algorithm> // Para std::min/max_element, std::reverse
#include <cmath>     // Para std::sqrt
#include <filesystem>// Para a função caminhoEstudos
#include <fstream>   // Para salvar arquivos CSV

 // Includes da biblioteca TP2
#include "../biblioteca/interface/GrafoPesado.h"
#include "../biblioteca/algoritmos/Dijkstra.h"

// --- Constantes e Estruturas ---
const int NUM_EXECUCOES_BENCHMARK = 100;
const std::string CAMINHO_BASE_GRAFOS = ""; // Será preenchido por caminhoEstudos

// Estrutura para o resultado do benchmark de tempo
struct ResultadoBenchmarkTempo {
    double tempoTotalMs = 0.0;
    double tempoMedioMs = 0.0;
    double tempoMinMs = std::numeric_limits<double>::infinity();
    double tempoMaxMs = 0.0;
    double desvioPadraoMs = 0.0;
};

// --- Protótipos das Funções ---
void executarEstudoCaso1_Dijkstra();
void executarEstudoCaso2_Dijkstra();
void executarModoInterativo_Pesado();
void menuDeAnalisePesado(GrafoPesado& grafo, const std::string& nomeGrafo);
std::vector<int> reconstruirCaminho(const std::vector<int>& pai, int origem, int destino);
std::string formatarCaminho(const std::vector<int>& caminho);
void pausarParaContinuar();
std::string caminhoEstudos(const std::string& nomeArquivo);
ResultadoBenchmarkTempo executarAmostraDijkstra(const GrafoPesado& grafo, const std::string& tipoImpl, int numExecucoes, const std::vector<int>& verticesDePartida);


// --- Função Principal ---
int main() {
    int escolha = -1;

    while (escolha != 0) {
        std::cout << "\n=======================================================" << std::endl;
        std::cout << "=== TP2 - GRAFOS PONDERADOS: TESTES E ANALISES ===" << std::endl;
        std::cout << "=======================================================" << std::endl;
        std::cout << "1. Estudo de Caso 1: Caminhos Minimos Especificos (Salva CSV)" << std::endl;
        std::cout << "2. Estudo de Caso 2: Benchmark de Tempo Dijkstra (Salva CSV)" << std::endl;
        std::cout << "3. Modo Interativo de Analise" << std::endl;
        std::cout << "-------------------------------------------------------" << std::endl;
        std::cout << "0. Sair" << std::endl;
        std::cout << "-------------------------------------------------------" << std::endl;
        std::cout << "Digite sua escolha: ";
        std::cin >> escolha;

        // Limpeza robusta do buffer de entrada
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            escolha = -1; // Invalida
        }
        else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpa o Enter
        }

        switch (escolha) {
        case 1: executarEstudoCaso1_Dijkstra(); break;
        case 2: executarEstudoCaso2_Dijkstra(); break;
        case 3: executarModoInterativo_Pesado(); break;
        case 0: std::cout << "Encerrando o programa." << std::endl; break;
        default:
            std::cout << "\nOpcao invalida." << std::endl;
            pausarParaContinuar();
            break;
        }
    }
    return 0;
}

// --- Implementação dos Estudos de Caso ---

/**
 * @brief Executa o Estudo de Caso 1: Calcula distâncias e caminhos do vértice 10
 * para alvos específicos, USANDO AMBAS AS IMPLEMENTAÇÕES DE DIJKSTRA.
 */
void executarEstudoCaso1_Dijkstra() {
    std::cout << "\n--- Estudo de Caso 1: Calculo de Caminhos Minimos (Origem 10) ---" << std::endl;
    std::ofstream arquivoResultados("resultados_estudo1_dijkstra.csv");
    if (!arquivoResultados.is_open()) {
        std::cerr << "ERRO: Nao foi possivel criar 'resultados_estudo1_dijkstra.csv'." << std::endl;
        pausarParaContinuar();
        return;
    }
    // Adiciona a coluna Implementacao ao CSV
    arquivoResultados << "Grafo,Implementacao,Destino,Distancia,Caminho\n";

    std::vector<std::string> nomesBaseGrafos;
    for (int i = 1; i <= 5; ++i) { // Assumindo 5 grafos ponderados
        nomesBaseGrafos.push_back("grafo_W_" + std::to_string(i) + ".txt"); // Novo nome
    }
    const int origem = 10;
    const std::vector<int> destinos = { 20, 30, 40, 50, 60 };

    std::cout << std::fixed << std::setprecision(4); // Precisão para pesos
    arquivoResultados << std::fixed << std::setprecision(4);

    for (const auto& nomeBase : nomesBaseGrafos) {
        std::cout << "\n--- Processando Grafo: " << nomeBase << " ---" << std::endl;
        try {
            std::string caminhoCompleto = caminhoEstudos(nomeBase);
            GrafoPesado grafo(caminhoCompleto);

            if (grafo.temPesoNegativo()) {
                std::cout << "  AVISO: Grafo contem pesos negativos. Dijkstra nao aplicavel." << std::endl;
                for (int dest : destinos) {
                    arquivoResultados << nomeBase << ",Heap," << dest << ",NEGATIVO,N/A\n";
                    arquivoResultados << nomeBase << ",Vetor," << dest << ",NEGATIVO,N/A\n";
                }
                continue; // Pula para o próximo grafo
            }

            Dijkstra dijkstraAlgo;

            // --- Execução com HEAP ---
            std::cout << "  Executando Dijkstra (Heap) a partir de " << origem << "..." << std::flush;
            ResultadoDijkstra resultadoHeap = dijkstraAlgo.executarHeap(grafo, origem);
            std::cout << " OK." << std::endl;

            // Salva resultados do Heap
            for (int dest : destinos) {
                if (dest <= 0 || dest > grafo.obterNumeroVertices()) {
                    arquivoResultados << nomeBase << ",Heap," << dest << ",INEXISTENTE,N/A\n";
                    continue;
                }
                std::string distStr;
                std::string caminhoStr = "N/A";
                if (resultadoHeap.dist[dest] == std::numeric_limits<double>::infinity()) {
                    distStr = "INFINITA";
                }
                else {
                    distStr = std::to_string(resultadoHeap.dist[dest]);
                    std::vector<int> caminho = reconstruirCaminho(resultadoHeap.pai, origem, dest);
                    caminhoStr = formatarCaminho(caminho);
                }
                arquivoResultados << nomeBase << ",Heap," << dest << "," << distStr << "," << "\"" << caminhoStr << "\"\n";
            }

            // --- Execução com VETOR ---
            std::cout << "  Executando Dijkstra (Vetor) a partir de " << origem << "..." << std::flush;
            ResultadoDijkstra resultadoVetor = dijkstraAlgo.executarVetor(grafo, origem);
            std::cout << " OK." << std::endl;

            // Salva resultados do Vetor
            for (int dest : destinos) {
                if (dest <= 0 || dest > grafo.obterNumeroVertices()) {
                    arquivoResultados << nomeBase << ",Vetor," << dest << ",INEXISTENTE,N/A\n";
                    continue;
                }
                std::string distStr;
                std::string caminhoStr = "N/A";
                if (resultadoVetor.dist[dest] == std::numeric_limits<double>::infinity()) {
                    distStr = "INFINITA";
                }
                else {
                    distStr = std::to_string(resultadoVetor.dist[dest]);
                    std::vector<int> caminho = reconstruirCaminho(resultadoVetor.pai, origem, dest);
                    caminhoStr = formatarCaminho(caminho);
                }
                arquivoResultados << nomeBase << ",Vetor," << dest << "," << distStr << "," << "\"" << caminhoStr << "\"\n";
            }


        }
        catch (const std::exception& e) {
            std::cerr << "  ERRO ao processar grafo " << nomeBase << ": " << e.what() << std::endl;
            for (int dest : destinos) { // Registra falha para ambos
                arquivoResultados << nomeBase << ",Heap," << dest << ",ERRO,N/A\n";
                arquivoResultados << nomeBase << ",Vetor," << dest << ",ERRO,N/A\n";
            }
        }
    }
    arquivoResultados.close();
    std::cout << "\n--- Estudo de Caso 1 Concluido! Resultados salvos em 'resultados_estudo1_dijkstra.csv' ---" << std::endl;
    pausarParaContinuar();
}


/**
 * @brief Executa o Estudo de Caso 2: Benchmark de tempo das implementações de Dijkstra.
 */
void executarEstudoCaso2_Dijkstra() {
    std::cout << "\n--- Estudo de Caso 2: Benchmark de Tempo Dijkstra (Vetor vs Heap) ---" << std::endl;
    std::ofstream arquivoResultados("resultados_estudo2_dijkstra.csv");
    if (!arquivoResultados.is_open()) {
        std::cerr << "ERRO: Nao foi possivel criar 'resultados_estudo2_dijkstra.csv'." << std::endl;
        pausarParaContinuar();
        return;
    }
    // Cabeçalho CSV conforme exemplo
    arquivoResultados << "Grafo,Algoritmo,Vertices,Arestas,NumExecucoes,TempoTotal_ms,TempoMedio_ms,TempoMin_ms,TempoMax_ms,DesvioPadrao_ms,Status\n";
    arquivoResultados << std::fixed << std::setprecision(6); // Maior precisão para tempos

    std::vector<std::string> nomesBaseGrafos;
    for (int i = 1; i <= 5; ++i) {
        nomesBaseGrafos.push_back("grafo_W_" + std::to_string(i) + ".txt");
    }

    std::cout << "Configurado para " << NUM_EXECUCOES_BENCHMARK << " execucoes por amostra." << std::endl;
    char continuar;
    std::cout << "Deseja iniciar o benchmark completo? (Pode levar tempo) (s/n): ";
    std::cin >> continuar;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpa buffer
    if (continuar != 's' && continuar != 'S') {
        std::cout << "Benchmark cancelado." << std::endl;
        arquivoResultados.close(); // Fecha o arquivo vazio
        return;
    }

    std::cout << "\nIniciando benchmark..." << std::endl;

    for (const auto& nomeBase : nomesBaseGrafos) {
        std::cout << "\nProcessando Grafo: " << nomeBase << "..." << std::endl;
        try {
            std::string caminhoCompleto = caminhoEstudos(nomeBase);
            GrafoPesado grafo(caminhoCompleto);
            const int numVertices = grafo.obterNumeroVertices();
            const long long numArestas = grafo.obterNumeroArestas();

            if (grafo.temPesoNegativo()) {
                std::cout << "  AVISO: Grafo com pesos negativos. Pulando benchmark Dijkstra." << std::endl;
                arquivoResultados << nomeBase << ",Vetor," << numVertices << "," << numArestas << "," << NUM_EXECUCOES_BENCHMARK << ",-1,-1,-1,-1,-1,NEGATIVO\n";
                arquivoResultados << nomeBase << ",Heap," << numVertices << "," << numArestas << "," << NUM_EXECUCOES_BENCHMARK << ",-1,-1,-1,-1,-1,NEGATIVO\n";
                continue;
            }

            // Gera K vértices aleatórios
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(1, numVertices);
            std::vector<int> verticesDePartida;
            verticesDePartida.reserve(NUM_EXECUCOES_BENCHMARK);
            for (int k = 0; k < NUM_EXECUCOES_BENCHMARK; ++k) {
                verticesDePartida.push_back(distrib(gen));
            }

            // --- Benchmark Vetor ---
            std::cout << "  - Executando Amostra Dijkstra (Vetor)..." << std::flush;
            ResultadoBenchmarkTempo resVetor = executarAmostraDijkstra(grafo, "Vetor", NUM_EXECUCOES_BENCHMARK, verticesDePartida);
            std::cout << " OK (Media: " << resVetor.tempoMedioMs << " ms)" << std::endl;
            arquivoResultados << nomeBase << ",Vetor," << numVertices << "," << numArestas << "," << NUM_EXECUCOES_BENCHMARK << ","
                << resVetor.tempoTotalMs << "," << resVetor.tempoMedioMs << "," << resVetor.tempoMinMs << ","
                << resVetor.tempoMaxMs << "," << resVetor.desvioPadraoMs << ",OK\n";

            // --- Benchmark Heap ---
            std::cout << "  - Executando Amostra Dijkstra (Heap)..." << std::flush;
            ResultadoBenchmarkTempo resHeap = executarAmostraDijkstra(grafo, "Heap", NUM_EXECUCOES_BENCHMARK, verticesDePartida);
            std::cout << " OK (Media: " << resHeap.tempoMedioMs << " ms)" << std::endl;
            arquivoResultados << nomeBase << ",Heap," << numVertices << "," << numArestas << "," << NUM_EXECUCOES_BENCHMARK << ","
                << resHeap.tempoTotalMs << "," << resHeap.tempoMedioMs << "," << resHeap.tempoMinMs << ","
                << resHeap.tempoMaxMs << "," << resHeap.desvioPadraoMs << ",OK\n";

        }
        catch (const std::exception& e) {
            std::cerr << "  ERRO ao processar grafo " << nomeBase << ": " << e.what() << std::endl;
            arquivoResultados << nomeBase << ",Vetor,-1,-1," << NUM_EXECUCOES_BENCHMARK << ",-1,-1,-1,-1,-1,ERRO\n";
            arquivoResultados << nomeBase << ",Heap,-1,-1," << NUM_EXECUCOES_BENCHMARK << ",-1,-1,-1,-1,-1,ERRO\n";
        }
    }

    arquivoResultados.close();
    std::cout << "\n--- Estudo de Caso 2 Concluido! Resultados salvos em 'resultados_estudo2_dijkstra.csv' ---" << std::endl;
    pausarParaContinuar();
}



/**
 * @brief Orquestra o modo interativo para grafos ponderados.
 */
void executarModoInterativo_Pesado() {
    while (true) {
        std::cout << "\n--- MODO INTERATIVO (GRAFOS PONDERADOS) ---" << std::endl;
        std::cout << "Escolha um grafo (1-5) ou 0 para voltar ao menu principal: ";
        int escolhaGrafo;
        std::cin >> escolhaGrafo;
        if (std::cin.fail() || escolhaGrafo == 0) {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            else {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpa Enter
            }
            break; // Sai do modo interativo
        }
        else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpa Enter
        }

        if (escolhaGrafo < 1 || escolhaGrafo > 5) { std::cout << "Escolha invalida." << std::endl; continue; }

        try {
            std::string nomeGrafoBase = "grafo_W_" + std::to_string(escolhaGrafo) + ".txt"; // Novo nome
            std::string caminho = caminhoEstudos(nomeGrafoBase); // Usa a função de caminho

            std::cout << "\nCarregando " << nomeGrafoBase << "..." << std::endl;
            // Usamos unique_ptr para gerenciar a memória automaticamente
            auto grafo = std::make_unique<GrafoPesado>(caminho);
            std::cout << "Grafo carregado com sucesso." << std::endl;

            // Entra no menu de análise para este grafo
            menuDeAnalisePesado(*grafo, nomeGrafoBase);

        }
        catch (const std::exception& e) {
            std::cerr << "ERRO ao carregar ou analisar o grafo: " << e.what() << std::endl;
        }
    }
}


/**
 * @brief Gerencia o menu de análise para um grafo ponderado já carregado.
 */
void menuDeAnalisePesado(GrafoPesado& grafo, const std::string& nomeGrafo) {
    int escolha = -1;
    while (escolha != 0) {
        std::cout << "\n--- Analisando o grafo '" << nomeGrafo << "' ---" << std::endl;
        std::cout << "1. Exibir Metricas Basicas (Vertices, Arestas, Peso Negativo?)" << std::endl;
        std::cout << "2. Executar Dijkstra (Heap ou Vetor)" << std::endl;
        std::cout << "0. Voltar (para escolher outro grafo)" << std::endl;
        std::cout << "Escolha uma analise: ";
        std::cin >> escolha;

        // Limpeza robusta do buffer
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            escolha = -1; // Invalida
        }
        else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpa Enter
        }

        try {
            switch (escolha) {
            case 1: {
                std::cout << "  > Vertices: " << grafo.obterNumeroVertices() << std::endl;
                std::cout << "  > Arestas: " << grafo.obterNumeroArestas() << std::endl;
                std::cout << "  > Possui Peso Negativo? " << (grafo.temPesoNegativo() ? "Sim" : "Nao") << std::endl;
                break;
            }
            case 2: { // Executar Dijkstra
                if (grafo.temPesoNegativo()) {
                    std::cout << "  AVISO: Dijkstra nao pode ser executado em grafos com pesos negativos." << std::endl;
                    break;
                }
                std::cout << "Qual vertice de origem? (1-" << grafo.obterNumeroVertices() << "): ";
                int origem;
                std::cin >> origem;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::cout << "Qual implementacao? (1 = Heap, 2 = Vetor): ";
                int impl;
                std::cin >> impl;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (origem < 1 || origem > grafo.obterNumeroVertices() || (impl != 1 && impl != 2)) {
                    std::cout << "Entrada invalida." << std::endl; break;
                }

                Dijkstra dijkstraAlgo;
                ResultadoDijkstra resultado;
                std::string nomeImpl = (impl == 1) ? "Heap" : "Vetor";
                std::cout << "Executando Dijkstra (" << nomeImpl << ") a partir de " << origem << "..." << std::flush;

                auto inicio = std::chrono::high_resolution_clock::now();
                if (impl == 1) resultado = dijkstraAlgo.executarHeap(grafo, origem);
                else           resultado = dijkstraAlgo.executarVetor(grafo, origem);
                auto fim = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> duracaoMs = fim - inicio;
                std::cout << " Concluido em " << duracaoMs.count() << " ms." << std::endl;

                std::cout << "Digite um vertice de destino para ver o caminho (ou 0 para ver todas as distancias): ";
                int destino;
                std::cin >> destino;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (destino > 0 && destino <= grafo.obterNumeroVertices()) {
                    std::cout << "  > Distancia para " << destino << ": ";
                    if (resultado.dist[destino] == std::numeric_limits<double>::infinity()) {
                        std::cout << "Infinita" << std::endl;
                    }
                    else {
                        std::cout << resultado.dist[destino] << std::endl;
                        std::vector<int> caminho = reconstruirCaminho(resultado.pai, origem, destino);
                        if (!caminho.empty()) {
                            std::cout << "  > Caminho: " << caminho[0];
                            for (size_t i = 1; i < caminho.size(); ++i) std::cout << "->" << caminho[i];
                        }
                        else {
                            std::cout << "  > Caminho: Nao encontrado ou destino == origem";
                        }
                        std::cout << std::endl;
                    }
                }
                else if (destino == 0) {
                    std::cout << "  > Distancias a partir de " << origem << ":" << std::endl;
                    for (int v = 1; v <= grafo.obterNumeroVertices(); ++v) {
                        std::cout << "    - Vertice " << v << ": " << (resultado.dist[v] == std::numeric_limits<double>::infinity() ? "Inf" : std::to_string(resultado.dist[v])) << " (Pai: " << resultado.pai[v] << ")" << std::endl;
                    }
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
            pausarParaContinuar(); // Usa a pausa principal
        }
    }
}

// --- Funções Auxiliares ---

/**
 * @brief Função para obter o caminho absoluto dos grafos na pasta esperada
 */
inline std::string caminhoEstudos(const std::string& nomeArquivo) {
    namespace fs = std::filesystem;
    fs::path pathDoArquivoAtual(__FILE__);
    fs::path pastaEstudos = pathDoArquivoAtual.parent_path(); // Assume que main.cpp está em estudos/
    fs::path pastaGrafos = pastaEstudos / "grafos_em_txt"; // Ajuste se necessário
    return (pastaGrafos / nomeArquivo).string();
}

/**
 * @brief Reconstrói o caminho mínimo de um destino até a origem usando o vetor de pais.
 */
std::vector<int> reconstruirCaminho(const std::vector<int>& pai, int origem, int destino) {
    std::vector<int> caminho;
    if (destino < 0 || destino >= pai.size() || (pai[destino] == -1 && destino != origem)) {
        return caminho; // Destino inválido ou inalcançável
    }
    int atual = destino;
    while (atual != 0 && atual != -1) { // Pai da origem é 0, inalcançável é -1
        caminho.push_back(atual);
        if (atual == origem) break;
        atual = pai[atual];
    }
    // Se o loop terminou e o último elemento não é a origem, algo deu errado (inalcançável)
    if (caminho.empty() || caminho.back() != origem) return {};

    std::reverse(caminho.begin(), caminho.end());
    return caminho;
}

/**
 * @brief Formata um vetor de caminho em uma string "v1->v2->v3".
 */
std::string formatarCaminho(const std::vector<int>& caminho) {
    if (caminho.empty()) {
        return "N/A";
    }
    std::string s = std::to_string(caminho[0]);
    for (size_t i = 1; i < caminho.size(); ++i) {
        s += "->" + std::to_string(caminho[i]);
    }
    return s;
}


/**
 * @brief Executa uma "amostra" de benchmark Dijkstra, medindo cada execução.
 */
ResultadoBenchmarkTempo executarAmostraDijkstra(const GrafoPesado& grafo, const std::string& tipoImpl, int numExecucoes, const std::vector<int>& verticesDePartida) {
    ResultadoBenchmarkTempo resultado;
    if (numExecucoes <= 0 || verticesDePartida.size() != numExecucoes) return resultado; // Validação

    Dijkstra dijkstraAlgo;
    std::vector<double> temposIndividuais;
    temposIndividuais.reserve(numExecucoes);

    bool isHeap = (tipoImpl == "Heap");

    for (int vertice : verticesDePartida) {
        auto inicio = std::chrono::high_resolution_clock::now();
        if (isHeap) dijkstraAlgo.executarHeap(grafo, vertice);
        else        dijkstraAlgo.executarVetor(grafo, vertice);
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
        // Usa N-1 para desvio padrão amostral (mais comum em benchmarks)
        double variancia = (numExecucoes > 1) ? (somaDosDesviosQuadrados / (numExecucoes - 1)) : 0.0;
        resultado.desvioPadraoMs = std::sqrt(variancia);
    }
    return resultado;
}

/**
 * @brief Pausa a execução e espera o usuário pressionar Enter.
 */
void pausarParaContinuar() {
    std::cout << "\nPressione ENTER para continuar...";
    // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Já limpo após cada escolha
    std::cin.get(); // Apenas espera o Enter
}