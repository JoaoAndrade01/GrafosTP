/**
 * @file GrafoDirigidoPesado.cpp
 * @brief Implementação dos métodos da classe GrafoDirigidoPesado (TP3 - grafos dirigidos).
 */

#include "GrafoDirigidoPesado.h"
#include "../representacao/VetorAdjacenciaDirigidaPesada.h" // Representação concreta DIRIGIDA
#include <fstream>    // std::ifstream
#include <stdexcept>  // std::runtime_error
#include <vector>
#include <string>
#include <limits>     // std::numeric_limits

/**
 * @brief Construtor da classe GrafoDirigidoPesado.
 *
 * Formato esperado do arquivo:
 *   - Primeira linha: N (número de vértices, N > 0)
 *   - Demais linhas: u v peso   (aresta dirigida u -> v, com peso double)
 */
GrafoDirigidoPesado::GrafoDirigidoPesado(const std::string& caminhoArquivo)
    : representacaoInterna(nullptr),
      numeroDeVertices(0),
      numeroDeArestas(0),
      possuiPesoNegativo(false)
{
    // 1) Abrir e validar arquivo
    std::ifstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo: " + caminhoArquivo);
    }

    // 2) Ler o número de vértices
    arquivo >> this->numeroDeVertices;
    if (numeroDeVertices <= 0) {
        throw std::runtime_error("Numero de vertices invalido no arquivo: " + caminhoArquivo);
    }

    // 3) Criar a representação interna (CSR dirigida com pesos)
    //    VetorAdjacenciaDirigidaPesada deve herdar de RepresentacaoPesada.
    representacaoInterna = std::make_unique<VetorAdjacenciaDirigidaPesada>(this->numeroDeVertices);

    // 4) Ler arestas dirigidas (u -> v, peso)
    int    u, v;
    double peso;
    long long contadorArestasLidas = 0; // Usa long long por segurança

    // Lê até não conseguir mais extrair "u v peso"
    while (arquivo >> u >> v >> peso) {
        // Delega a adição para a representação interna
        // Aqui assumimos SEMPRE direção: u -> v
        representacaoInterna->adicionarArestaComPeso(u, v, peso);
        contadorArestasLidas++;

        if (peso < 0.0) {
            this->possuiPesoNegativo = true;
        }
    }

    // 5) Verificar erro de formato antes do EOF
    if (!arquivo.eof() && arquivo.fail()) {
        throw std::runtime_error("Erro de formato ao ler aresta no arquivo: " + caminhoArquivo);
    }

    this->numeroDeArestas = contadorArestasLidas;

    // 6) Finalizar construção da estrutura CSR dirigida
    representacaoInterna->finalizarConstrucao();
    // arquivo fecha automaticamente ao sair do escopo (RAII)
}

/**
 * @brief Destrutor da classe GrafoDirigidoPesado.
 *
 * Necessário pois usamos forward declaration + unique_ptr no header.
 */
GrafoDirigidoPesado::~GrafoDirigidoPesado() = default;

// --- Métodos de consulta ---

int GrafoDirigidoPesado::obterNumeroVertices() const {
    return this->numeroDeVertices;
}

long long GrafoDirigidoPesado::obterNumeroArestas() const {
    return this->numeroDeArestas;
}

int GrafoDirigidoPesado::obterGrau(int u) const {
    // Em grafo dirigido, aqui interpretamos como grau de SAÍDA (out-degree).
    return representacaoInterna->obterGrau(u);
}

std::vector<VizinhoComPeso> GrafoDirigidoPesado::obterVizinhosComPesos(int u) const {
    return representacaoInterna->obterVizinhosComPesos(u);
}

double GrafoDirigidoPesado::obterPesoAresta(int u, int v) const {
    return representacaoInterna->obterPesoAresta(u, v);
}

bool GrafoDirigidoPesado::temPesoNegativo() const {
    return this->possuiPesoNegativo;
}

// --- Métodos para Dijkstra ---

ResultadoDijkstra GrafoDirigidoPesado::executarDijkstraHeap(int origem) const {
    Dijkstra dijkstraAlgo;
    return dijkstraAlgo.executarHeap(*this, origem);
}

ResultadoDijkstra GrafoDirigidoPesado::executarDijkstraVetor(int origem) const {
    Dijkstra dijkstraAlgo;
    return dijkstraAlgo.executarVetor(*this, origem);
}
