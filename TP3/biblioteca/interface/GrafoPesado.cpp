/**
 * @file GrafoPesado.cpp
 * @brief Implementação dos métodos da classe GrafoPesado.
 */

#include "GrafoPesado.h"
#include "../representacao/VetorAdjacenciaPesada.h" // Inclui a representação concreta
#include <fstream>   // Para leitura de arquivos (ifstream)
#include <stdexcept> // Para lançar exceções (runtime_error)
#include <vector>
#include <string>
#include <limits>    // Para std::numeric_limits

 /**
  * @brief Construtor da classe GrafoPesado.
  */
GrafoPesado::GrafoPesado(const std::string& caminhoArquivo, bool direcionado)
    : numeroDeVertices(0), numeroDeArestas(0), possuiPesoNegativo(false), ehDirecionado(direcionado) {

    // Bloco: Abertura e validação do arquivo de entrada
    std::ifstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo: " + caminhoArquivo);
    }

    // Bloco: Leitura do número de vértices
    arquivo >> this->numeroDeVertices;
    if (numeroDeVertices <= 0) {
        throw std::runtime_error("Numero de vertices invalido no arquivo: " + caminhoArquivo);
    }

    // Bloco: Criação da representação interna (VetorAdjacenciaPesada/CSR)
    // Usamos make_unique para gerenciar a memória automaticamente.
    representacaoInterna = std::make_unique<VetorAdjacenciaPesada>(this->numeroDeVertices, ehDirecionado);

    // Bloco: Leitura das arestas com pesos do arquivo
    int u, v;
    double peso;
    long long contadorArestasLidas = 0; // Usamos long long para segurança

    // O loop continua enquanto for possível ler três valores da linha.
    while (arquivo >> u >> v >> peso) {
        // Delega a adição da aresta (com peso) para o objeto de representação.
        representacaoInterna->adicionarArestaComPeso(u, v, peso);
        contadorArestasLidas++;

        // Verifica se há pesos negativos
        if (peso < 0.0) {
            this->possuiPesoNegativo = true;
        }
    }

    // Verifica se houve erro na leitura antes do fim do arquivo
    if (!arquivo.eof() && arquivo.fail()) {
        throw std::runtime_error("Erro de formato ao ler aresta no arquivo: " + caminhoArquivo);
    }

    this->numeroDeArestas = contadorArestasLidas;

    // Bloco: Finalização da construção da representação interna (essencial para CSR)
    representacaoInterna->finalizarConstrucao();

    // O arquivo é fechado automaticamente quando 'arquivo' sai de escopo (RAII).
}

/**
 * @brief Destrutor da classe GrafoPesado.
 * @details A definição explícita aqui, mesmo que vazia, é necessária por causa
 * do uso de forward declaration de RepresentacaoPesada e std::unique_ptr
 * com tipos incompletos no header. Garante a deleção correta.
 */
GrafoPesado::~GrafoPesado() = default;

// --- Implementação dos Métodos de Consulta ---

int GrafoPesado::obterNumeroVertices() const {
    return this->numeroDeVertices;
}

long long GrafoPesado::obterNumeroArestas() const {
    return this->numeroDeArestas;
}

int GrafoPesado::obterGrau(int u) const {
    // Delega a chamada para o objeto de representação interno.
    return representacaoInterna->obterGrau(u);
}

std::vector<VizinhoComPeso> GrafoPesado::obterVizinhosComPesos(int u) const {
    // Delega a chamada para o objeto de representação interno.
    return representacaoInterna->obterVizinhosComPesos(u);
}

double GrafoPesado::obterPesoAresta(int u, int v) const {
    // Delega a chamada para o objeto de representação interno.
    return representacaoInterna->obterPesoAresta(u, v);
}

bool GrafoPesado::temPesoNegativo() const {
    return this->possuiPesoNegativo;
}

bool GrafoPesado::ehDirecionado() const {
    return this->ehDirecionado;
}



// --- Implementação dos Métodos de Dijkstra ---
ResultadoDijkstra GrafoPesado::executarDijkstraHeap(int origem) const {
    Dijkstra dijkstraAlgo; // Cria instância da classe especialista
    return dijkstraAlgo.executarHeap(*this, origem); // Delega a chamada
}

ResultadoDijkstra GrafoPesado::executarDijkstraVetor(int origem) const {
    Dijkstra dijkstraAlgo; // Cria instância da classe especialista
    return dijkstraAlgo.executarVetor(*this, origem); // Delega a chamada
}