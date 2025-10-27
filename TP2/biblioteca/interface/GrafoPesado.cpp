/**
 * @file GrafoPesado.cpp
 * @brief Implementa��o dos m�todos da classe GrafoPesado.
 */

#include "GrafoPesado.h"
#include "../representacao/VetorAdjacenciaPesada.h" // Inclui a representa��o concreta
#include <fstream>   // Para leitura de arquivos (ifstream)
#include <stdexcept> // Para lan�ar exce��es (runtime_error)
#include <vector>
#include <string>
#include <limits>    // Para std::numeric_limits

 /**
  * @brief Construtor da classe GrafoPesado.
  */
GrafoPesado::GrafoPesado(const std::string& caminhoArquivo)
    : numeroDeVertices(0), numeroDeArestas(0), possuiPesoNegativo(false) {

    // Bloco: Abertura e valida��o do arquivo de entrada
    std::ifstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo: " + caminhoArquivo);
    }

    // Bloco: Leitura do n�mero de v�rtices
    arquivo >> this->numeroDeVertices;
    if (numeroDeVertices <= 0) {
        throw std::runtime_error("Numero de vertices invalido no arquivo: " + caminhoArquivo);
    }

    // Bloco: Cria��o da representa��o interna (VetorAdjacenciaPesada/CSR)
    // Usamos make_unique para gerenciar a mem�ria automaticamente.
    representacaoInterna = std::make_unique<VetorAdjacenciaPesada>(this->numeroDeVertices);

    // Bloco: Leitura das arestas com pesos do arquivo
    int u, v;
    double peso;
    long long contadorArestasLidas = 0; // Usamos long long para seguran�a

    // O loop continua enquanto for poss�vel ler tr�s valores da linha.
    while (arquivo >> u >> v >> peso) {
        // Delega a adi��o da aresta (com peso) para o objeto de representa��o.
        representacaoInterna->adicionarArestaComPeso(u, v, peso);
        contadorArestasLidas++;

        // Verifica se h� pesos negativos
        if (peso < 0.0) {
            this->possuiPesoNegativo = true;
        }
    }

    // Verifica se houve erro na leitura antes do fim do arquivo
    if (!arquivo.eof() && arquivo.fail()) {
        throw std::runtime_error("Erro de formato ao ler aresta no arquivo: " + caminhoArquivo);
    }

    this->numeroDeArestas = contadorArestasLidas;

    // Bloco: Finaliza��o da constru��o da representa��o interna (essencial para CSR)
    representacaoInterna->finalizarConstrucao();

    // O arquivo � fechado automaticamente quando 'arquivo' sai de escopo (RAII).
}

/**
 * @brief Destrutor da classe GrafoPesado.
 * @details A defini��o expl�cita aqui, mesmo que vazia, � necess�ria por causa
 * do uso de forward declaration de RepresentacaoPesada e std::unique_ptr
 * com tipos incompletos no header. Garante a dele��o correta.
 */
GrafoPesado::~GrafoPesado() = default;

// --- Implementa��o dos M�todos de Consulta ---

int GrafoPesado::obterNumeroVertices() const {
    return this->numeroDeVertices;
}

long long GrafoPesado::obterNumeroArestas() const {
    return this->numeroDeArestas;
}

int GrafoPesado::obterGrau(int u) const {
    // Delega a chamada para o objeto de representa��o interno.
    return representacaoInterna->obterGrau(u);
}

std::vector<VizinhoComPeso> GrafoPesado::obterVizinhosComPesos(int u) const {
    // Delega a chamada para o objeto de representa��o interno.
    return representacaoInterna->obterVizinhosComPesos(u);
}

double GrafoPesado::obterPesoAresta(int u, int v) const {
    // Delega a chamada para o objeto de representa��o interno.
    return representacaoInterna->obterPesoAresta(u, v);
}

bool GrafoPesado::temPesoNegativo() const {
    return this->possuiPesoNegativo;
}

// --- Implementa��o dos M�todos de Dijkstra ---
ResultadoDijkstra GrafoPesado::executarDijkstraHeap(int origem) const {
    Dijkstra dijkstraAlgo; // Cria inst�ncia da classe especialista
    return dijkstraAlgo.executarHeap(*this, origem); // Delega a chamada
}

ResultadoDijkstra GrafoPesado::executarDijkstraVetor(int origem) const {
    Dijkstra dijkstraAlgo; // Cria inst�ncia da classe especialista
    return dijkstraAlgo.executarVetor(*this, origem); // Delega a chamada
}