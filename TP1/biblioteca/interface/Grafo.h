#pragma once
/**
 * @file Grafo.h
 * @brief Definição da classe Grafo, a principal interface da biblioteca.
 * @author Seu Nome (ou o nome do Gemini)
 * @date 20 de Setembro de 2025
 */

#include <string>
#include <vector>
#include <memory> // Necessário para std::unique_ptr

 // Declaração antecipada (forward declaration) da classe de representação.
 // Isso evita a necessidade de incluir o cabeçalho completo da representação aqui,
 // o que ajuda a manter a interface limpa e diminui o acoplamento do código.
class Representacao;

/**
 * @enum TipoRepresentacao
 * @brief Enumeração para especificar a representação interna do grafo a ser utilizada.
 *
 * O usuário da biblioteca deve escolher uma destas opções ao construir um objeto Grafo.
 */
enum class TipoRepresentacao {
    MATRIZ_ADJACENCIA, ///< Representar o grafo usando uma matriz de adjacência.
    LISTA_ADJACENCIA,   ///< Representar o grafo usando uma lista de adjacência.
    MATRIZ_ADJACENCIA_TRIANGULAR,
	LISTA_ADJACENCIA_SIMPLES

};

/**
 * @class Grafo
 * @brief Classe principal que representa um grafo e serve como a API pública da biblioteca.
 * @details Esta classe abstrai a representação interna do grafo (matriz, lista ou vetor)
 * e fornece métodos para consultar suas propriedades e, futuramente, executar algoritmos.
 */
class Grafo {
public:
    /**
     * @brief Construtor da classe Grafo.
     * @details Carrega um grafo a partir de um arquivo de texto, utilizando a
     * representação interna especificada.
     * @param caminhoArquivo O caminho para o arquivo .txt que contém a definição do grafo.
     * @param tipo O tipo de representação a ser usada (MATRIZ_ADJACENCIA ou LISTA_ADJACENCIA).
     * @throws std::runtime_error se o arquivo não puder ser aberto.
     */
    Grafo(const std::string& caminhoArquivo, TipoRepresentacao tipo);

    /**
     * @brief Destrutor da classe Grafo.
     * @details Libera os recursos alocados pelo grafo, especialmente a memória
     * utilizada pela representação interna. O uso de std::unique_ptr automatiza este processo.
     */
    ~Grafo();

    // --- Métodos de Consulta de Propriedades Básicas ---

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


    // --- Futuros Métodos para Algoritmos e Relatórios ---
    // (Serão implementados nos próximos passos)
    //
    // void gerarRelatorioCompleto(const std::string& arquivoSaida) const;
    // ResultadoBusca executarBFS(int verticeInicial) const;
    // ... etc ...


private:
    // Ponteiro inteligente para a implementação da representação do grafo.
    // O uso de std::unique_ptr garante que a memória seja gerenciada automaticamente (RAII).
    // Este é o núcleo do padrão de design "Ponte para Implementação" (PImpl) ou "Strategy",
    // que nos permite trocar a representação sem mudar a interface pública (Grafo).
    std::unique_ptr<Representacao> representacaoInterna;

    // Armazenamos contadores básicos aqui para acesso rápido, evitando
    // a necessidade de consultar a representação interna a todo momento.
    int numeroDeVertices;
    int numeroDeArestas;
};