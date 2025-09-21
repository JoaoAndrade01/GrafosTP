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

/**
 * @brief Executa a Busca em Largura (BFS) a partir de um vértice de origem.
 * @details Este método atua como uma fachada (Facade). Ele instancia a classe
 * especialista em BFS e delega a execução para ela, passando uma referência
 * a si mesmo (*this).
 */
ResultadoBFS Grafo::executarBFS(int verticeOrigem) const {
    // 1. Cria uma instância do nosso "trabalhador" de BFS.
    BFS algoritmoBFS;

    // 2. Chama o método 'executar' do trabalhador, passando o próprio
    // objeto Grafo atual (*this) como parâmetro, e retorna o resultado.
    return algoritmoBFS.executar(*this, verticeOrigem);
}

/**
 * @brief Executa a Busca em Profundidade (DFS) a partir de um vértice de origem.
 * @details Atua como uma fachada, delegando a execução para a classe especialista 'DFS'.
 */
ResultadoDFS Grafo::executarDFS(int verticeOrigem) const {
    DFS algoritmoDFS;
    return algoritmoDFS.executar(*this, verticeOrigem);
}

/**
 * @brief Salva a árvore de busca (pai e nível de cada vértice) em um arquivo,
 * ORDENADA POR NÍVEL.
 * @details Coleta os dados de pai e nível para todos os vértices, ordena esses
 * dados com base no nível (e depois por ID, como critério de desempate), e
 * então escreve o resultado ordenado no arquivo.
 */
void Grafo::salvarArvoreBusca(const ResultadoBFS& resultado, const std::string& caminhoArquivo) const {
    // Bloco 1: Abertura e verificação do arquivo
    std::ofstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo de saida: " + caminhoArquivo);
    }

    // Bloco 2: Coleta dos dados em uma estrutura temporária
    // Isso é necessário para que possamos ordenar antes de escrever.
    struct InfoVertice {
        int id;
        int pai;
        int nivel;
    };
    std::vector<InfoVertice> verticesParaOrdenar;
    // Percorre todos os vértices do grafo
    for (int v = 1; v <= this->numeroDeVertices; ++v) {
        // Adiciona à lista apenas os vértices que foram alcançados pela busca
        if (resultado.nivel[v] != -1) {
            verticesParaOrdenar.push_back({ v, resultado.pai[v], resultado.nivel[v] });
        }
    }

    // Bloco 3: Ordenação do vetor temporário
    // Usamos std::sort com uma expressão lambda para definir nossa regra de ordenação.
    std::sort(verticesParaOrdenar.begin(), verticesParaOrdenar.end(),
        [](const InfoVertice& a, const InfoVertice& b) {
            // Regra 1: O critério principal é o nível.
            if (a.nivel != b.nivel) {
                return a.nivel < b.nivel; // Ordena por nível ascendente.
            }
            // Regra 2: Se os níveis forem iguais, usamos o ID do vértice como desempate.
            return a.id < b.id;
        }
    );

    // Bloco 4: Escrita dos dados já ordenados no arquivo
    arquivo << "Vertice,Pai,Nivel\n"; // Escreve o cabeçalho
    for (const auto& info : verticesParaOrdenar) {
        arquivo << info.id << "," << info.pai << "," << info.nivel << "\n";
    }
}

void Grafo::salvarArvoreBusca(const ResultadoDFS& resultado, const std::string& caminhoArquivo) const {
    // Bloco 1: Abertura e verificação do arquivo
    std::ofstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo de saida: " + caminhoArquivo);
    }

    // Bloco 2: Coleta dos dados em uma estrutura temporária
    // Isso é necessário para que possamos ordenar antes de escrever.
    struct InfoVertice {
        int id;
        int pai;
        int nivel;
    };
    std::vector<InfoVertice> verticesParaOrdenar;
    // Percorre todos os vértices do grafo
    for (int v = 1; v <= this->numeroDeVertices; ++v) {
        // Adiciona à lista apenas os vértices que foram alcançados pela busca
        if (resultado.nivel[v] != -1) {
            verticesParaOrdenar.push_back({ v, resultado.pai[v], resultado.nivel[v] });
        }
    }

    // Bloco 3: Ordenação do vetor temporário
    // Usamos std::sort com uma expressão lambda para definir nossa regra de ordenação.
    std::sort(verticesParaOrdenar.begin(), verticesParaOrdenar.end(),
        [](const InfoVertice& a, const InfoVertice& b) {
            // Regra 1: O critério principal é o nível.
            if (a.nivel != b.nivel) {
                return a.nivel < b.nivel; // Ordena por nível ascendente.
            }
            // Regra 2: Se os níveis forem iguais, usamos o ID do vértice como desempate.
            return a.id < b.id;
        }
    );

    // Bloco 4: Escrita dos dados já ordenados no arquivo
    arquivo << "Vertice,Pai,Nivel\n"; // Escreve o cabeçalho
    for (const auto& info : verticesParaOrdenar) {
        arquivo << info.id << "," << info.pai << "," << info.nivel << "\n";
    }
}

/**
 * @brief Implementação do cálculo de distância.
 */
int Grafo::calcularDistancia(int verticeU, int verticeV) const {
    // Bloco 1: Validação dos vértices (boa prática)
    if (verticeU <= 0 || verticeU > this->numeroDeVertices || verticeV <= 0 || verticeV > this->numeroDeVertices) {
        throw std::out_of_range("Vértice(s) inválido(s) fornecido(s) para calcularDistancia.");
    }

    // A distância de um vértice para ele mesmo é 0.
    if (verticeU == verticeV) {
        return 0;
    }

    // Bloco 2: Execução do BFS a partir da origem 'u'
    // Delegamos todo o trabalho pesado para o método que já temos.
    ResultadoBFS resultadoBFS = this->executarBFS(verticeU);

    // Bloco 3: Retorno do resultado
    // A propriedade fundamental do BFS é que o vetor 'nivel' contém a menor
    // distância da origem para todos os outros vértices.
    // Se o vértice 'v' for inalcançável, seu nível já será -1, que é o
    // valor correto a ser retornado.
    return resultadoBFS.nivel[verticeV];
}