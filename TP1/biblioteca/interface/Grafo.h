#pragma once
/**
 * @file Grafo.h
 * @brief Definição da classe Grafo, a principal interface da biblioteca.
 */

#include <string>
#include <vector>
#include <memory> // Necessário para std::unique_ptr

 // Inclui as definições dos algoritmos e de seus respectivos resultados
#include "../algoritmos/BFS.h"
#include "../algoritmos/DFS.h"

// --- DEFINIÇÃO DAS STRUCTS DE RESULTADO ---

/**
 * @struct ComponenteConexa
 * @brief Armazena os dados de uma única componente conexa.
 */
struct ComponenteConexa {
    int tamanho = 0; // Quantidade de vértices na componente
    std::vector<int> vertices; // Lista de vértices que pertencem a esta componente
};

/**
 * @struct EstatisticasGrau
 * @brief Armazena estatísticas calculadas sobre os graus dos vértices do grafo.
 */
struct EstatisticasGrau {
    int grauMin = 0;
    int grauMax = 0;
    double grauMedio = 0.0;
    double grauMediana = 0.0;
};


// --- DECLARAÇÃO DA CLASSE PRINCIPAL ---

// Declaração antecipada da classe de representação.
class Representacao;

/**
 * @enum TipoRepresentacao
 * @brief Enumeração para especificar a representação interna do grafo a ser utilizada.
 */
enum class TipoRepresentacao {
    MATRIZ_ADJACENCIA,
    MATRIZ_ADJACENCIA_TRIANGULAR,
    LISTA_ADJACENCIA, // Versão Otimizada (CSR)
    LISTA_ADJACENCIA_SIMPLES
};

/**
 * @class Grafo
 * @brief Classe principal que representa um grafo e serve como a API pública da biblioteca.
 * @details Abstrai a representação interna do grafo e fornece métodos para análise.
 */
class Grafo {
public:
    /**
     * @brief Construtor da classe Grafo.
     * @details Carrega um grafo a partir de um arquivo de texto, utilizando a
     * representação interna especificada.
     * @param caminhoArquivo O caminho para o arquivo .txt que contém a definição do grafo.
     * @param tipo O tipo de representação a ser usada.
     * @throws std::runtime_error se o arquivo não puder ser aberto.
     */
    Grafo(const std::string& caminhoArquivo, TipoRepresentacao tipo);

    /**
     * @brief Destrutor da classe Grafo.
     * @details Libera os recursos alocados pelo grafo. O uso de std::unique_ptr automatiza este processo.
     */
    ~Grafo();

    // --- MÉTODOS DE CONSULTA BÁSICA ---

    /**
     * @brief Obtém o número total de vértices no grafo.
     * @return O número de vértices.
     */
    int obterNumeroVertices() const;

    /**
     * @brief Obtém o número total de arestas no grafo.
     * @return O número de arestas.
     */
    int obterNumeroArestas() const;

    /**
     * @brief Obtém o grau de um vértice específico.
     * @param vertice O vértice (indexado a partir de 1) cujo grau é desejado.
     * @return O grau do vértice.
     */
    int obterGrau(int vertice) const;

    /**
     * @brief Obtém uma lista com os vizinhos de um vértice.
     * @details Este método é uma primitiva essencial para a maioria dos algoritmos em grafos.
     * @param vertice O vértice (indexado a partir de 1) do qual se deseja obter os vizinhos.
     * @return Um std::vector<int> contendo os índices dos vértices vizinhos.
     */
    std::vector<int> obterVizinhos(int vertice) const;

    // --- MÉTODOS DE ALGORITMOS ---

    /**
     * @brief Executa a Busca em Largura (BFS) a partir de um vértice de origem.
     * @param verticeOrigem O vértice (indexado a partir de 1) para iniciar a busca.
     * @return Um struct ResultadoBFS contendo a árvore de busca (vetores de pai e nível).
     */
    ResultadoBFS executarBFS(int verticeOrigem) const;

    /**
     * @brief Executa a Busca em Profundidade (DFS) a partir de um vértice de origem.
     * @param verticeOrigem O vértice (indexado a partir de 1) para iniciar a busca.
     * @return Um struct ResultadoDFS contendo a árvore de busca (vetores de pai e nível).
     */
    ResultadoDFS executarDFS(int verticeOrigem) const;

    /**
     * @brief Calcula a distancia (menor caminho em arestas) entre dois vertices.
     * @details Utiliza o algoritmo BFS como primitiva.
     * @param verticeU O vertice de origem.
     * @param verticeV O vertice de destino.
     * @return O número de arestas no caminho mais curto. Retorna -1 se v for inalcançável a partir de u.
     */
    int calcularDistancia(int verticeU, int verticeV) const;

    /**
     * @brief Calcula o diâmetro EXATO do grafo. CUIDADO: muito lento para grafos grandes.
     * @details Algoritmo de força bruta que executa um BFS a partir de cada vértice.
     * @return O diâmetro do grafo, ou -1 se for desconexo.
     */
    int calcularDiametro() const;

    /**
     * @brief Estima o diâmetro do grafo com uma heurística rápida.
     * @param iteracoes O número de vezes que a heurística será executada (default = 5).
     * @return O diâmetro aproximado do grafo.
     */
    int calcularDiametroAproximado(int iteracoes = 5) const;

    /**
     * @brief Encontra todas as componentes conexas do grafo.
     * @details Utiliza um algoritmo de busca para encontrar todos os subgrafos conexos.
     * @return Um vetor de structs ComponenteConexa, já ordenado por tamanho em ordem decrescente.
     */
    std::vector<ComponenteConexa> encontrarComponentesConexas() const;

    // --- MÉTODOS DE ANÁLISE E RELATÓRIO ---

    /**
     * @brief Calcula as estatísticas de grau (mín, máx, médio, mediana) do grafo.
     * @return Um struct EstatisticasGrau com os resultados.
     */
    EstatisticasGrau calcularEstatisticasGrau() const;

    /**
         * @brief Gera um relatorio completo e o salva em um arquivo. (Versão de conveniência)
         * @param caminhoArquivo O nome do arquivo onde o relatorio sera salvo.
         */
    void gerarRelatorioCompleto(const std::string& caminhoArquivo) const;

    /**
     * @brief Gera um relatorio completo e o envia para um fluxo de saída (arquivo, console, etc.). (Versão principal)
     * @param saida O fluxo de saída (ex: std::cout ou um std::ofstream).
     */
    void gerarRelatorioCompleto(std::ostream& saida) const;


    // --- MÉTODOS DE SALVAMENTO ---

    /**
     * @brief Salva a árvore de busca gerada pelo BFS em um arquivo de texto.
     * @param resultado O struct ResultadoBFS retornado pelo método de busca.
     * @param caminhoArquivo O nome do arquivo onde a árvore será salva.
     */
    void salvarArvoreBusca(const ResultadoBFS& resultado, const std::string& caminhoArquivo) const;

    /**
     * @brief Salva a árvore de busca gerada pelo DFS em um arquivo de texto.
     * @param resultado O struct ResultadoDFS retornado pelo método de busca.
     * @param caminhoArquivo O nome do arquivo onde a árvore será salva.
     */
    void salvarArvoreBusca(const ResultadoDFS& resultado, const std::string& caminhoArquivo) const;


private:
    /**
     * @brief Ponteiro inteligente para a implementação da representação do grafo.
     * @details O uso de std::unique_ptr garante que a memória seja gerenciada automaticamente (RAII).
     * Este é o núcleo do padrão de design "Strategy", que nos permite trocar a
     * representação sem mudar a interface pública.
     */
    std::unique_ptr<Representacao> representacaoInterna;

    /**
     * @brief Armazena o número de vértices para acesso rápido.
     */
    int numeroDeVertices;

    /**
     * @brief Armazena o número de arestas para acesso rápido.
     */
    int numeroDeArestas;
};

