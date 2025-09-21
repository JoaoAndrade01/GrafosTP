/**
 * @file Grafo.cpp
 * @brief Implementa��o dos m�todos da classe Grafo.
 */

#include "Grafo.h" // Inclui a defini��o da pr�pria classe
#include "../representacao/MatrizAdjacencia.h"
#include "../representacao/MatrizAdjacenciaTriangular.h"
#include "../representacao/ListaAdjacencia.h"
#include "../representacao/ListaAdjacenciaSimples.h"
 // #include "../representacao/ListaAdjacencia.h" // Ser� inclu�do no futuro
#include <fstream>   // Para leitura de arquivos (ifstream)
#include <stdexcept> // Para lan�ar exce��es (runtime_error)
#include <iostream>  // Para mensagens de depura��o (opcional)

/**
 * @brief Construtor da classe Grafo.
 */
Grafo::Grafo(const std::string& caminhoArquivo, TipoRepresentacao tipo) {
    // Bloco: Abertura e valida��o do arquivo de entrada
    std::ifstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        // Lan�a uma exce��o se o arquivo n�o puder ser aberto.
        // O programa de teste (main.cpp) pode capturar esta exce��o.
        throw std::runtime_error("Nao foi possivel abrir o arquivo: " + caminhoArquivo);
    }

    // Bloco: Leitura do n�mero de v�rtices
    arquivo >> this->numeroDeVertices;
    this->numeroDeArestas = 0; // Inicializa o contador de arestas

    // Bloco: Cria��o da representa��o interna com base na escolha do usu�rio
    // Este � o ponto central que usa o padr�o de design "Strategy".
    switch (tipo) {
    case TipoRepresentacao::MATRIZ_ADJACENCIA:
        // Cria um objeto MatrizAdjacencia e o move para o ponteiro inteligente.
        representacaoInterna = std::make_unique<MatrizAdjacencia>(this->numeroDeVertices);
        break;
    case TipoRepresentacao::LISTA_ADJACENCIA:
        // Lan�a um erro claro se a funcionalidade ainda n�o foi implementada.
        representacaoInterna = std::make_unique<ListaAdjacencia>(this->numeroDeVertices);
        break;
    case TipoRepresentacao::MATRIZ_ADJACENCIA_TRIANGULAR:
        representacaoInterna = std::make_unique<MatrizAdjacenciaTriangular>(this->numeroDeVertices);
        break;
    case TipoRepresentacao::LISTA_ADJACENCIA_SIMPLES: // <-- Adicionar este novo caso
        representacaoInterna = std::make_unique<ListaAdjacenciaSimples>(this->numeroDeVertices);
        break;
    default:
        throw std::invalid_argument("Tipo de representacao desconhecido.");
    }

    // Bloco: Leitura das arestas do arquivo
    int u, v;
    // O loop continua enquanto for poss�vel ler dois inteiros da linha.
    while (arquivo >> u >> v) {
        // Delega a adi��o da aresta para o objeto de representa��o.
        // A classe Grafo n�o sabe como isso � feito, apenas manda a ordem.
        representacaoInterna->adicionarAresta(u, v);
        this->numeroDeArestas++; // Incrementa o contador de arestas
    }

    // Ap�s ler todas as arestas, chamamos o m�todo para construir a estrutura final.
    // Para a MatrizAdjacencia, isso n�o far� nada. Para a ListaAdjacencia, far� tudo.
    representacaoInterna->finalizarConstrucao();

    // O n�mero de arestas � definido internamente pela representa��o,
    // ent�o podemos busc�-lo ap�s a finaliza��o (ou ler do buffer)
    // Para simplicidade, vamos recalcular.
    arquivo.clear();
    arquivo.seekg(0);
    std::string linha;
    std::getline(arquivo, linha); // Pula a primeira linha
    this->numeroDeArestas = 0;
    while (std::getline(arquivo, linha)) this->numeroDeArestas++;

    // O arquivo � fechado automaticamente quando 'arquivo' sai de escopo.
}

/**
 * @brief Destrutor da classe Grafo.
 * @details O compilador gerar� um destrutor padr�o que chama o destrutor de
 * std::unique_ptr, que por sua vez deleta o objeto de representa��o.
 * A defini��o expl�cita aqui, mesmo que vazia, � necess�ria por causa do uso
 * de forward declaration e std::unique_ptr com tipos incompletos no header.
 */
Grafo::~Grafo() = default;

// Implementa��es dos outros m�todos (obterNumeroVertices, etc.)
int Grafo::obterNumeroVertices() const {
    return this->numeroDeVertices;
}

int Grafo::obterNumeroArestas() const {
    return this->numeroDeArestas;
}

int Grafo::obterGrau(int vertice) const {
    // Delega a chamada para o objeto de representa��o
    return representacaoInterna->obterGrau(vertice);
}

std::vector<int> Grafo::obterVizinhos(int vertice) const {
    // Delega a chamada para o objeto de representa��o
    return representacaoInterna->obterVizinhos(vertice);
}