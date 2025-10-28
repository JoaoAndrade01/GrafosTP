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
#include <map>       // Para mapeamento Nome <-> ID
#include <sstream>   // Para processar o arquivo de vértices

 // Includes da biblioteca TP2
#include "../biblioteca/interface/GrafoPesado.h"
#include "../biblioteca/algoritmos/Dijkstra.h"

// --- Constantes e Estruturas ---
const int NUM_EXECUCOES_BENCHMARK = 100;
// Define o valor infinito para facilitar a leitura
const double infinity = std::numeric_limits<double>::infinity();

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
void executarEstudoCaso3_Colaboracao(); // <-- NOVO
void executarModoInterativo_Pesado();
void menuDeAnalisePesado(GrafoPesado& grafo, const std::string& nomeGrafoBase, int indiceGrafo); // Adiciona indiceGrafo
std::vector<int> reconstruirCaminho(const std::vector<int>& pai, int origem, int destino);
std::string formatarCaminho(const std::vector<int>& caminho, const std::map<int, std::string>& idParaNome); // <-- Modificado
std::string formatarCaminho(const std::vector<int>& caminho); // Sobrecarga para IDs
void pausarParaContinuar();
std::string caminhoEstudos(const std::string& nomeArquivo);
ResultadoBenchmarkTempo executarAmostraDijkstra(const GrafoPesado& grafo, const std::string& tipoImpl, int numExecucoes, const std::vector<int>& verticesDePartida);
bool carregarMapeamentoNomes(const std::string& arquivoVertices,
    std::map<std::string, int>& nomeParaId,
    std::map<int, std::string>& idParaNome); // <-- NOVO
std::string normalizarString(std::string s); // <-- ADICIONE ESTA LINHA

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


// --- Função Principal ---
int main() {
    int escolha = -1;

    while (escolha != 0) {
        std::cout << "\n=======================================================" << std::endl;
        std::cout << "=== TP2 - GRAFOS PONDERADOS: TESTES E ANALISES ===" << std::endl;
        std::cout << "=======================================================" << std::endl;
        std::cout << "1. Estudo de Caso 1: Caminhos Minimos (Origem 10)" << std::endl;
        std::cout << "2. Estudo de Caso 2: Benchmark de Tempo Dijkstra" << std::endl;
        std::cout << "3. Estudo de Caso 3: Rede de Colaboracao" << std::endl; // <-- NOVO
        std::cout << "4. Modo Interativo de Analise" << std::endl; // Renumerado
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
            // Limpa o Enter apenas se a leitura foi bem-sucedida
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        switch (escolha) {
        case 1: executarEstudoCaso1_Dijkstra(); break;
        case 2: executarEstudoCaso2_Dijkstra(); break;
        case 3: executarEstudoCaso3_Colaboracao(); break; // <-- NOVO
        case 4: executarModoInterativo_Pesado(); break;   // Renumerado
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
 * @brief Executa o Estudo de Caso 1, com restrição para Vetor nos grafos 4 e 5.
 */
void executarEstudoCaso1_Dijkstra() {
    std::cout << "\n--- Estudo de Caso 1: Calculo de Caminhos Minimos (Origem 10) ---" << std::endl;
    std::ofstream arquivoResultados("resultados_estudo1_dijkstra.csv");
    if (!arquivoResultados.is_open()) {
        std::cerr << "ERRO: Nao foi possivel criar 'resultados_estudo1_dijkstra.csv'." << std::endl;
        pausarParaContinuar();
        return;
    }
    arquivoResultados << "Grafo,Implementacao,Destino,Distancia,Caminho\n";

    std::map<int, std::string> nomesBaseGrafos; // Usar map para associar índice ao nome
    for (int i = 1; i <= 5; ++i) {
        nomesBaseGrafos[i] = "grafo_W_" + std::to_string(i) + ".txt";
    }
    const int origem = 10;
    const std::vector<int> destinos = { 20, 30, 40, 50, 60 };

    std::cout << std::fixed << std::setprecision(4); // Precisão para pesos
    arquivoResultados << std::fixed << std::setprecision(4);

    for (const auto& pair : nomesBaseGrafos) {
        int indiceGrafo = pair.first;
        const std::string& nomeBase = pair.second;
        bool executarVetor = (indiceGrafo <= 3); // <<< RESTRIÇÃO APLICADA AQUI

        std::cout << "\n--- Processando Grafo: " << nomeBase << " (" << indiceGrafo << ") ---" << std::endl;
        try {
            std::string caminhoCompleto = caminhoEstudos(nomeBase);
            GrafoPesado grafo(caminhoCompleto);

            if (grafo.temPesoNegativo()) {
                std::cout << "  AVISO: Grafo contem pesos negativos. Dijkstra nao aplicavel." << std::endl;
                for (int dest : destinos) {
                    arquivoResultados << nomeBase << ",Heap," << dest << ",NEGATIVO,N/A\n";
                    arquivoResultados << nomeBase << ",Vetor," << dest << ",NEGATIVO,N/A\n";
                }
                continue;
            }

            Dijkstra dijkstraAlgo;

            // --- Execução com HEAP (sempre executa) ---
            std::cout << "  Executando Dijkstra (Heap) a partir de " << origem << "..." << std::flush;
            ResultadoDijkstra resultadoHeap = dijkstraAlgo.executarHeap(grafo, origem);
            std::cout << " OK." << std::endl;
            for (int dest : destinos) {
                if (dest <= 0 || dest > grafo.obterNumeroVertices()) {
                    arquivoResultados << nomeBase << ",Heap," << dest << ",INEXISTENTE,N/A\n"; continue;
                }
                std::string distStr = (resultadoHeap.dist[dest] == infinity) ? "INFINITA" : std::to_string(resultadoHeap.dist[dest]);
                std::string caminhoStr = formatarCaminho(reconstruirCaminho(resultadoHeap.pai, origem, dest));
                arquivoResultados << nomeBase << ",Heap," << dest << "," << distStr << "," << "\"" << caminhoStr << "\"\n";
            }

            // --- Execução com VETOR (condicional) ---
            if (executarVetor) {
                std::cout << "  Executando Dijkstra (Vetor) a partir de " << origem << "..." << std::flush;
                ResultadoDijkstra resultadoVetor = dijkstraAlgo.executarVetor(grafo, origem);
                std::cout << " OK." << std::endl;
                for (int dest : destinos) {
                    if (dest <= 0 || dest > grafo.obterNumeroVertices()) {
                        arquivoResultados << nomeBase << ",Vetor," << dest << ",INEXISTENTE,N/A\n"; continue;
                    }
                    std::string distStr = (resultadoVetor.dist[dest] == infinity) ? "INFINITA" : std::to_string(resultadoVetor.dist[dest]);
                    std::string caminhoStr = formatarCaminho(reconstruirCaminho(resultadoVetor.pai, origem, dest));
                    arquivoResultados << nomeBase << ",Vetor," << dest << "," << distStr << "," << "\"" << caminhoStr << "\"\n";
                }
            }
            else {
                std::cout << "  Skipping Dijkstra (Vetor) para grafo grande (" << indiceGrafo << ")." << std::endl;
                for (int dest : destinos) {
                    arquivoResultados << nomeBase << ",Vetor," << dest << ",N/A (Skipped),N/A\n";
                }
            }

        }
        catch (const std::exception& e) {
            std::cerr << "  ERRO ao processar grafo " << nomeBase << ": " << e.what() << std::endl;
            for (int dest : destinos) {
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
 * @brief Executa o Estudo de Caso 2, com restrição para Vetor nos grafos 4 e 5.
 */
void executarEstudoCaso2_Dijkstra() {
    std::cout << "\n--- Estudo de Caso 2: Benchmark de Tempo Dijkstra (Vetor vs Heap) ---" << std::endl;
    std::ofstream arquivoResultados("resultados_estudo2_dijkstra.csv");
    if (!arquivoResultados.is_open()) { /* ... erro ... */ return; }
    arquivoResultados << "Grafo,Algoritmo,Vertices,Arestas,NumExecucoes,TempoTotal_ms,TempoMedio_ms,TempoMin_ms,TempoMax_ms,DesvioPadrao_ms,Status\n";
    arquivoResultados << std::fixed << std::setprecision(6);

    std::map<int, std::string> nomesBaseGrafos; // Usar map
    for (int i = 1; i <= 5; ++i) {
        nomesBaseGrafos[i] = "grafo_W_" + std::to_string(i) + ".txt";
    }

    // ... (confirmação do usuário) ...
    std::cout << "\nIniciando benchmark..." << std::endl;

    for (const auto& pair : nomesBaseGrafos) {
        int indiceGrafo = pair.first;
        const std::string& nomeBase = pair.second;
        bool executarVetor = (indiceGrafo <= 3); // <<< RESTRIÇÃO APLICADA AQUI

        std::cout << "\nProcessando Grafo: " << nomeBase << " (" << indiceGrafo << ")..." << std::endl;
        try {
            std::string caminhoCompleto = caminhoEstudos(nomeBase);
            GrafoPesado grafo(caminhoCompleto);
            const int numVertices = grafo.obterNumeroVertices();
            const long long numArestas = grafo.obterNumeroArestas();

            if (grafo.temPesoNegativo()) { /* ... registro erro negativo ... */ continue; }

            // Gera K vértices aleatórios
            std::vector<int> verticesDePartida;
            // ... (lógica para gerar vértices aleatórios) ...
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(1, numVertices);
            verticesDePartida.reserve(NUM_EXECUCOES_BENCHMARK);
            for (int k = 0; k < NUM_EXECUCOES_BENCHMARK; ++k) {
                verticesDePartida.push_back(distrib(gen));
            }

            // --- Benchmark Vetor (Condicional) ---
            if (executarVetor) {
                std::cout << "  - Executando Amostra Dijkstra (Vetor)..." << std::flush;
                ResultadoBenchmarkTempo resVetor = executarAmostraDijkstra(grafo, "Vetor", NUM_EXECUCOES_BENCHMARK, verticesDePartida);
                std::cout << " OK (Media: " << resVetor.tempoMedioMs << " ms)" << std::endl;
                arquivoResultados << nomeBase << ",Vetor," << numVertices << "," << numArestas << "," << NUM_EXECUCOES_BENCHMARK << ","
                    << resVetor.tempoTotalMs << "," << resVetor.tempoMedioMs << "," << resVetor.tempoMinMs << ","
                    << resVetor.tempoMaxMs << "," << resVetor.desvioPadraoMs << ",OK\n";
            }
            else {
                std::cout << "  - Skipping Amostra Dijkstra (Vetor) para grafo grande (" << indiceGrafo << ")..." << std::endl;
                arquivoResultados << nomeBase << ",Vetor," << numVertices << "," << numArestas << "," << NUM_EXECUCOES_BENCHMARK << ",-1,-1,-1,-1,-1,SKIPPED\n";
            }

            // --- Benchmark Heap (Sempre executa) ---
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
 * @brief Executa o Estudo de Caso 3: Rede de Colaboração.
 */
void executarEstudoCaso3_Colaboracao() {
    std::cout << "\n--- Estudo de Caso 3: Rede de Colaboracao ---" << std::endl;
    std::string arquivoVertices = "rede_colaboracao_vertices.txt";
    std::string arquivoGrafo = "rede_colaboracao.txt";
    std::map<std::string, int> nomeParaId; // Chave será o nome NORMALIZADO
    std::map<int, std::string> idParaNome; // Valor será o nome ORIGINAL

    // 1. Carregar Mapeamento Nome <-> ID
    std::cout << "Carregando mapeamento de pesquisadores..." << std::flush;
    // Esta função agora preenche nomeParaId com chaves normalizadas
    if (!carregarMapeamentoNomes(caminhoEstudos(arquivoVertices), nomeParaId, idParaNome)) {
        pausarParaContinuar();
        return;
    }
    std::cout << " OK (" << nomeParaId.size() << " entradas normalizadas)." << std::endl;

    // 2. Definir Pesquisadores e Encontrar IDs
    std::string origemNome = "Edsger W. Dijkstra";
    std::vector<std::string> destinosNomes = {
        "Alan M. Turing", "J. B. Kruskal", "Jon M. Kleinberg",
        "Eva Tardos", // <-- MODIFICADO (sem acento) para testar
        "Daniel R. Figueiredo"
    };

    int origemId = -1;
    std::vector<int> destinosIds;

    // Normaliza a string de busca da origem
    std::string origemNomeNorm = normalizarString(origemNome); // <-- MODIFICADO
    if (nomeParaId.count(origemNomeNorm)) { // <-- MODIFICADO
        origemId = nomeParaId[origemNomeNorm]; // <-- MODIFICADO
    }
    else {
        std::cerr << "ERRO: Pesquisador de origem '" << origemNome << "' (Normalizado: '" << origemNomeNorm << "') nao encontrado no mapeamento." << std::endl;
        pausarParaContinuar();
        return;
    }

    std::cout << "Encontrando IDs dos pesquisadores alvo..." << std::endl;
    for (const std::string& nome : destinosNomes) {
        // Normaliza a string de busca do destino
        std::string nomeNorm = normalizarString(nome); // <-- MODIFICADO

        if (nomeParaId.count(nomeNorm)) { // <-- MODIFICADO
            destinosIds.push_back(nomeParaId[nomeNorm]); // <-- MODIFICADO
            std::cout << "  - Encontrado: " << nome << " (ID: " << nomeParaId[nomeNorm] << ")" << std::endl;
        }
        else {
            std::cerr << "  AVISO: Pesquisador '" << nome << "' (Normalizado: '" << nomeNorm << "') nao encontrado. Sera ignorado." << std::endl;
            destinosIds.push_back(-1); // Indica não encontrado
        }
    }

    // 3. Carregar Grafo e Executar Dijkstra
    try {
        std::cout << "\nCarregando grafo de colaboracao (" << arquivoGrafo << ")..." << std::flush;
        GrafoPesado grafo(caminhoEstudos(arquivoGrafo));
        std::cout << " OK (Vertices: " << grafo.obterNumeroVertices() << ", Arestas: " << grafo.obterNumeroArestas() << ")." << std::endl;

        if (grafo.temPesoNegativo()) {
            std::cout << "  AVISO: Grafo contem pesos negativos. Dijkstra pode nao ser o algoritmo correto." << std::endl;
        }

        Dijkstra dijkstraAlgo;
        // Busca a partir do nome original (com acento) salvo no idParaNome
        std::cout << "Executando Dijkstra a partir de '" << idParaNome[origemId] << "' (ID: " << origemId << ")..." << std::flush;
        ResultadoDijkstra resultado = dijkstraAlgo.executarHeap(grafo, origemId); // Usando Heap
        std::cout << " OK." << std::endl;

        // 4. Apresentar Resultados
        std::cout << "\n--- Resultados: Distancia e Caminho Minimo a partir de " << idParaNome[origemId] << " ---" << std::endl;
        std::cout << std::fixed << std::setprecision(4);
        std::cout << "+------------------------+-----------+----------------------------------------------------+" << std::endl;
        std::cout << "| Pesquisador Alvo       | Distancia | Caminho Minimo (Nomes)                             |" << std::endl;
        std::cout << "+------------------------+-----------+----------------------------------------------------+" << std::endl;

        for (size_t i = 0; i < destinosNomes.size(); ++i) {
            int destId = destinosIds[i];
            // Pega o nome original da busca (ex: "Eva Tardos")
            const std::string& destNomeBusca = destinosNomes[i];

            // Tenta exibir o nome original (com acento) se encontrado
            std::string nomeExibicao = destNomeBusca;
            if (destId != -1 && idParaNome.count(destId)) {
                nomeExibicao = idParaNome[destId]; // Exibe "Éva Tardos"
            }

            std::cout << "| " << std::left << std::setw(22) << nomeExibicao << " | ";

            if (destId == -1) {
                std::cout << std::right << std::setw(9) << "N/A" << " | Pesquisador nao encontrado no mapeamento.        |" << std::endl;
            }
            else if (destId <= 0 || destId > grafo.obterNumeroVertices()) {
                std::cout << std::right << std::setw(9) << "N/A" << " | ID invalido ou fora do grafo.                  |" << std::endl;
            }
            else if (resultado.dist[destId] == infinity) {
                std::cout << std::right << std::setw(9) << "Infinita" << " | Inalcancavel                                       |" << std::endl;
            }
            else {
                std::cout << std::right << std::setw(9) << resultado.dist[destId] << " | ";
                std::vector<int> caminhoIds = reconstruirCaminho(resultado.pai, origemId, destId);
                // formatarCaminho usa o idParaNome, que tem os nomes originais
                std::string caminhoNomes = formatarCaminho(caminhoIds, idParaNome);
                // Limita o tamanho da string do caminho para caber na tabela
                if (caminhoNomes.length() > 48) {
                    caminhoNomes = caminhoNomes.substr(0, 45) + "...";
                }
                std::cout << std::left << std::setw(50) << caminhoNomes << "|" << std::endl;
            }
        }
        std::cout << "+------------------------+-----------+----------------------------------------------------+" << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << "\nERRO durante o Estudo de Caso 3: " << e.what() << std::endl;
    }

    pausarParaContinuar();
}


/**
 * @brief Orquestra o modo interativo, com restrição para Vetor nos grafos 4 e 5.
 */
void executarModoInterativo_Pesado() {
    while (true) {
        std::cout << "\n--- MODO INTERATIVO (GRAFOS PONDERADOS) ---" << std::endl;
        std::cout << "Escolha um grafo (1-5) ou 0 para voltar ao menu principal: ";
        int escolhaGrafo;
        std::cin >> escolhaGrafo;
        // ... (validação e limpeza de buffer) ...
        if (std::cin.fail() || escolhaGrafo == 0) {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            else {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpa Enter
            }
            break;
        }
        else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpa Enter
        }
        if (escolhaGrafo < 1 || escolhaGrafo > 5) { std::cout << "Escolha invalida." << std::endl; continue; }

        try {
            std::string nomeGrafoBase = "grafo_W_" + std::to_string(escolhaGrafo) + ".txt";
            std::string caminho = caminhoEstudos(nomeGrafoBase);

            std::cout << "\nCarregando " << nomeGrafoBase << "..." << std::endl;
            auto grafo = std::make_unique<GrafoPesado>(caminho);
            std::cout << "Grafo carregado." << std::endl;

            // Passa o índice do grafo para o menu de análise
            menuDeAnalisePesado(*grafo, nomeGrafoBase, escolhaGrafo);

        }
        catch (const std::exception& e) {
            std::cerr << "ERRO ao carregar ou analisar o grafo: " << e.what() << std::endl;
        }
    }
}


/**
 * @brief Gerencia o menu de análise, com restrição para Vetor nos grafos 4 e 5.
 */
void menuDeAnalisePesado(GrafoPesado& grafo, const std::string& nomeGrafoBase, int indiceGrafo) { // Recebe indiceGrafo
    int escolha = -1;
    bool podeExecutarVetor = (indiceGrafo <= 3); // <<< RESTRIÇÃO APLICADA AQUI

    while (escolha != 0) {
        std::cout << "\n--- Analisando o grafo '" << nomeGrafoBase << "' ---" << std::endl;
        std::cout << "1. Exibir Metricas Basicas" << std::endl;
        std::cout << "2. Executar Dijkstra" << std::endl;
        std::cout << "0. Voltar" << std::endl;
        std::cout << "Escolha: ";
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
            case 1: { // Exibir Métricas
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
                // Validação e Limpeza
                if (std::cin.fail() || origem < 1 || origem > grafo.obterNumeroVertices()) {
                    std::cout << "Vertice invalido." << std::endl;
                    if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); }
                    else { std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); }
                    break;
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpa Enter


                int impl = 1; // Padrão é Heap
                if (podeExecutarVetor) { // <<< RESTRIÇÃO APLICADA AQUI
                    std::cout << "Qual implementacao? (1 = Heap, 2 = Vetor): ";
                    std::cin >> impl;
                    if (std::cin.fail() || (impl != 1 && impl != 2)) {
                        std::cout << "Implementacao invalida." << std::endl;
                        if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); }
                        else { std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); }
                        break;
                    }
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                else {
                    std::cout << "Usando implementacao Heap (Vetor nao disponivel para este grafo)." << std::endl;
                    impl = 1; // Força Heap para grafos 4 e 5
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
                if (std::cin.fail()) {
                    std::cout << "Entrada invalida." << std::endl;
                    std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (destino > 0 && destino <= grafo.obterNumeroVertices()) {
                    std::cout << "  > Distancia para " << destino << ": ";
                    if (resultado.dist[destino] == infinity) {
                        std::cout << "Infinita" << std::endl;
                    }
                    else {
                        std::cout << resultado.dist[destino] << std::endl;
                        std::vector<int> caminho = reconstruirCaminho(resultado.pai, origem, destino);
                        std::cout << "  > Caminho: " << formatarCaminho(caminho) << std::endl;
                    }
                }
                else if (destino == 0) {
                    std::cout << "  > Distancias a partir de " << origem << ":" << std::endl;
                    for (int v = 1; v <= grafo.obterNumeroVertices(); ++v) {
                        std::cout << "    - Vertice " << std::setw(std::to_string(grafo.obterNumeroVertices()).length()) << v << ": "
                            << std::setw(12) << (resultado.dist[v] == infinity ? "Inf" : std::to_string(resultado.dist[v]))
                            << " (Pai: " << resultado.pai[v] << ")" << std::endl;
                    }
                }
                else {
                    std::cout << "Destino invalido." << std::endl;
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
            pausarParaContinuar();
        }
    }
}


// --- Funções Auxiliares ---

/**
 * @brief Reconstrói o caminho mínimo de um destino até a origem usando o vetor de pais.
 */
std::vector<int> reconstruirCaminho(const std::vector<int>& pai, int origem, int destino) {
    std::vector<int> caminho;
    // Adiciona validação para tamanho do vetor pai
    if (destino < 0 || destino >= pai.size() || origem < 0 || origem >= pai.size()) {
        return caminho; // Índice inválido
    }
    if (pai[destino] == -1 && destino != origem) {
        return caminho; // Destino inalcançável
    }
    int atual = destino;
    while (atual != 0 && atual != -1) { // Pai da origem é 0, inalcançável é -1
        caminho.push_back(atual);
        if (atual == origem) break;
        atual = pai[atual];
        // Adiciona um limite para evitar loops infinitos em caso de erro
        if (caminho.size() > pai.size()) return {};
    }
    // Verifica se o caminho realmente começou na origem
    if (caminho.empty() || caminho.back() != origem) return {};

    std::reverse(caminho.begin(), caminho.end());
    return caminho;
}

/**
 * @brief Carrega o mapeamento ID <-> Nome do arquivo de vértices.
 */
bool carregarMapeamentoNomes(const std::string& arquivoVertices,
    std::map<std::string, int>& nomeParaId,
    std::map<int, std::string>& idParaNome) {
    std::ifstream arquivo(arquivoVertices);
    if (!arquivo.is_open()) {
        std::cerr << "\nERRO CRITICO: Nao foi possivel abrir o arquivo de mapeamento de vertices: " << arquivoVertices << std::endl;
        return false;
    }

    std::string linha;
    int contadorLinhas = 0;
    while (std::getline(arquivo, linha)) {
        contadorLinhas++;
        std::stringstream ss(linha);
        std::string segmentoId;
        std::string nome;
        int id = -1;

        // Pega o ID (primeiro campo antes da primeira vírgula)
        if (std::getline(ss, segmentoId, ',')) {
            try {
                id = std::stoi(segmentoId);
            }
            catch (...) { // Captura qualquer erro de conversão
                std::cerr << "AVISO [Linha " << contadorLinhas << "]: ID invalido ('" << segmentoId << "') em: " << linha << std::endl;
                continue;
            }
        }
        else {
            std::cerr << "AVISO [Linha " << contadorLinhas << "]: Formato invalido (sem virgula?) em: " << linha << std::endl;
            continue;
        }

        // Pega o Nome (todo o resto da linha após a primeira vírgula)
        if (std::getline(ss, nome)) {
            // Remove espaços extras no início/fim
            nome.erase(0, nome.find_first_not_of(" \t\r\n"));
            nome.erase(nome.find_last_not_of(" \t\r\n") + 1);

            if (id > 0 && !nome.empty()) { // ID deve ser positivo
                // MODIFICAÇÃO: Armazena o nome original e o normalizado
                std::string nomeNormalizado = normalizarString(nome); // <-- MODIFICADO

                if (nomeParaId.count(nomeNormalizado)) { // <-- MODIFICADO (checa pelo nome normalizado)
                    // Aviso de colisão de nome normalizado
                    std::cerr << "AVISO [Linha " << contadorLinhas << "]: Nome normalizado duplicado: '" << nomeNormalizado << "' (Original: '" << nome << "')" << std::endl;
                }

                // Armazena a chave normalizada
                nomeParaId[nomeNormalizado] = id; // <-- MODIFICADO

                // Armazena o nome original para exibição
                idParaNome[id] = nome; // <-- SEM MUDANÇA
            }
            else {
                std::cerr << "AVISO [Linha " << contadorLinhas << "]: ID invalido (" << id << ") ou nome vazio em: " << linha << std::endl;
            }
        }
        else {
            std::cerr << "AVISO [Linha " << contadorLinhas << "]: Nome ausente em: " << linha << std::endl;
        }
    }
    arquivo.close();
    if (nomeParaId.empty()) {
        std::cerr << "\nERRO CRITICO: Nenhum mapeamento valido foi carregado de " << arquivoVertices << std::endl;
        return false;
    }
    return true;
}

/**
 * @brief Formata um vetor de caminho de IDs em uma string "Nome1 -> Nome2 -> Nome3".
 */
std::string formatarCaminho(const std::vector<int>& caminho, const std::map<int, std::string>& idParaNome) {
    if (caminho.empty()) {
        return "N/A";
    }
    std::string s = "";
    for (size_t i = 0; i < caminho.size(); ++i) {
        int id = caminho[i];
        try {
            // Usa .at() para lançar exceção se ID não existir (segurança)
            s += idParaNome.at(id);
        }
        catch (const std::out_of_range& oor) {
            std::cerr << "\nAVISO: ID " << id << " no caminho nao encontrado no mapeamento!" << std::endl;
            s += "ID(" + std::to_string(id) + "?)"; // Indica erro no nome
        }
        if (i < caminho.size() - 1) {
            s += " -> ";
        }
    }
    return s;
}

/**
 * @brief Sobrecarga para formatar caminho apenas com IDs.
 */
std::string formatarCaminho(const std::vector<int>& caminho) {
    if (caminho.empty()) { return "N/A"; }
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

/**
 * @brief Normaliza uma string: converte para minúsculas e remove acentos comuns.
 * @param s A string original (assume-se UTF-8 ou ISO-8859-1).
 * @return A string normalizada.
 */
std::string normalizarString(std::string s) {
    std::string result = "";
    for (size_t i = 0; i < s.length(); ++i) {
        unsigned char c = s[i];

        // 1. Converter ASCII para minúsculas
        if (c >= 'A' && c <= 'Z') {
            result += (c + 32); // tolower
            continue;
        }

        // 2. Lidar com sequências UTF-8 de 2 bytes (comuns para acentos latinos)
        if (c == 0xC3) {
            if (i + 1 < s.length()) {
                unsigned char c2 = s[i + 1];
                switch (c2) {
                case 0x81: case 0x82: case 0x83: case 0x80: // Á, Â, Ã, À
                case 0xA1: case 0xA2: case 0xA3: case 0xA0: // á, â, ã, à
                    result += 'a';
                    break;
                case 0x87: case 0xA7: // Ç, ç
                    result += 'c';
                    break;
                case 0x89: case 0x8A: // É, Ê
                case 0xA9: case 0xAA: // é, ê
                    result += 'e';
                    break;
                case 0x8D: case 0xAD: // Í, í
                    result += 'i';
                    break;
                case 0x93: case 0x94: case 0x95: // Ó, Ô, Õ
                case 0xB3: case 0xB4: case 0xB5: // ó, ô, õ
                    result += 'o';
                    break;
                case 0x9A: case 0xBA: // Ú, ú
                    result += 'u';
                    break;
                default: // Outro caractere UTF-8 que não mapeamos
                    result += c;
                    result += c2;
                    break;
                }
                i++; // Pulamos o segundo byte da sequência
            }
            else {
                result += c; // Sequência incompleta
            }
        }
        // 3. Lidar com caracteres de 1 byte (ISO-8859-1 / Windows-1252)
        else if (c >= 0xC0 && c <= 0xFF) {
            switch (c) {
            case 0xC1: case 0xC2: case 0xC3: case 0xC0: // Á, Â, Ã, À
            case 0xE1: case 0xE2: case 0xE3: case 0xE0: // á, â, ã, à
                result += 'a';
                break;
            case 0xC7: case 0xE7: // Ç, ç
                result += 'c';
                break;
            case 0xC9: case 0xCA: // É, Ê
            case 0xE9: case 0xEA: // é, ê
                result += 'e';
                break;
            case 0xCD: case 0xED: // Í, í
                result += 'i';
                break;
            case 0xD3: case 0xD4: case 0xD5: // Ó, Ô, Õ
            case 0xF3: case 0xF4: case 0xF5: // ó, ô, õ
                result += 'o';
                break;
            case 0xDA: case 0xFA: // Ú, ú
                result += 'u';
                break;
            default: // Outro caractere que não mapeamos
                result += c;
                break;
            }
        }
        // 4. Manter caracteres ASCII padrão (minúsculas, números, símbolos)
        else {
            result += c;
        }
    }
    return result;
}