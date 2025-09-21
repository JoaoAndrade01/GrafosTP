/**
 * @file Grafo.cpp
 * @brief Implementação dos métodos da classe Grafo.
 */

#include "Grafo.h" // Inclui a definição da própria classe
#include "../representacao/MatrizAdjacencia.h"
#include "../representacao/MatrizAdjacenciaTriangular.h"
#include "../representacao/ListaAdjacencia.h"
#include "../representacao/ListaAdjacenciaSimples.h"
 // #include "../representacao/ListaAdjacencia.h" // Será incluído no futuro
#include <fstream>   // Para leitura de arquivos (ifstream)
#include <stdexcept> // Para lançar exceções (runtime_error)
#include <iostream>  // Para mensagens de depuração (opcional)

/**
 * @brief Construtor da classe Grafo.
 */
Grafo::Grafo(const std::string& caminhoArquivo, TipoRepresentacao tipo) {
    // Bloco: Abertura e validação do arquivo de entrada
    std::ifstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        // Lança uma exceção se o arquivo não puder ser aberto.
        // O programa de teste (main.cpp) pode capturar esta exceção.
        throw std::runtime_error("Nao foi possivel abrir o arquivo: " + caminhoArquivo);
    }

    // Bloco: Leitura do número de vértices
    arquivo >> this->numeroDeVertices;
    this->numeroDeArestas = 0; // Inicializa o contador de arestas

    // Bloco: Criação da representação interna com base na escolha do usuário
    // Este é o ponto central que usa o padrão de design "Strategy".
    switch (tipo) {
    case TipoRepresentacao::MATRIZ_ADJACENCIA:
        // Cria um objeto MatrizAdjacencia e o move para o ponteiro inteligente.
        representacaoInterna = std::make_unique<MatrizAdjacencia>(this->numeroDeVertices);
        break;
    case TipoRepresentacao::LISTA_ADJACENCIA:
        // Lança um erro claro se a funcionalidade ainda não foi implementada.
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
    // O loop continua enquanto for possível ler dois inteiros da linha.
    while (arquivo >> u >> v) {
        // Delega a adição da aresta para o objeto de representação.
        // A classe Grafo não sabe como isso é feito, apenas manda a ordem.
        representacaoInterna->adicionarAresta(u, v);
        this->numeroDeArestas++; // Incrementa o contador de arestas
    }

    // Após ler todas as arestas, chamamos o método para construir a estrutura final.
    // Para a MatrizAdjacencia, isso não fará nada. Para a ListaAdjacencia, fará tudo.
    representacaoInterna->finalizarConstrucao();

    // O número de arestas é definido internamente pela representação,
    // então podemos buscá-lo após a finalização (ou ler do buffer)
    // Para simplicidade, vamos recalcular.
    arquivo.clear();
    arquivo.seekg(0);
    std::string linha;
    std::getline(arquivo, linha); // Pula a primeira linha
    this->numeroDeArestas = 0;
    while (std::getline(arquivo, linha)) this->numeroDeArestas++;

    // O arquivo é fechado automaticamente quando 'arquivo' sai de escopo.
}

/**
 * @brief Destrutor da classe Grafo.
 * @details O compilador gerará um destrutor padrão que chama o destrutor de
 * std::unique_ptr, que por sua vez deleta o objeto de representação.
 * A definição explícita aqui, mesmo que vazia, é necessária por causa do uso
 * de forward declaration e std::unique_ptr com tipos incompletos no header.
 */
Grafo::~Grafo() = default;

// Implementações dos outros métodos (obterNumeroVertices, etc.)
int Grafo::obterNumeroVertices() const {
    return this->numeroDeVertices;
}

int Grafo::obterNumeroArestas() const {
    return this->numeroDeArestas;
}

int Grafo::obterGrau(int vertice) const {
    // Delega a chamada para o objeto de representação
    return representacaoInterna->obterGrau(vertice);
}

std::vector<int> Grafo::obterVizinhos(int vertice) const {
    // Delega a chamada para o objeto de representação
    return representacaoInterna->obterVizinhos(vertice);
}