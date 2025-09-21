#pragma once
/**
 * @file Grafo.h
 * @brief Defini��o da classe Grafo, a principal interface da biblioteca.
 * @author Seu Nome (ou o nome do Gemini)
 * @date 20 de Setembro de 2025
 */

#include <string>
#include <vector>
#include <memory> // Necess�rio para std::unique_ptr

 // Declara��o antecipada (forward declaration) da classe de representa��o.
 // Isso evita a necessidade de incluir o cabe�alho completo da representa��o aqui,
 // o que ajuda a manter a interface limpa e diminui o acoplamento do c�digo.
class Representacao;

/**
 * @enum TipoRepresentacao
 * @brief Enumera��o para especificar a representa��o interna do grafo a ser utilizada.
 *
 * O usu�rio da biblioteca deve escolher uma destas op��es ao construir um objeto Grafo.
 */
enum class TipoRepresentacao {
    MATRIZ_ADJACENCIA, ///< Representar o grafo usando uma matriz de adjac�ncia.
    LISTA_ADJACENCIA,   ///< Representar o grafo usando uma lista de adjac�ncia.
    MATRIZ_ADJACENCIA_TRIANGULAR,
	LISTA_ADJACENCIA_SIMPLES

};

/**
 * @class Grafo
 * @brief Classe principal que representa um grafo e serve como a API p�blica da biblioteca.
 * @details Esta classe abstrai a representa��o interna do grafo (matriz, lista ou vetor)
 * e fornece m�todos para consultar suas propriedades e, futuramente, executar algoritmos.
 */
class Grafo {
public:
    /**
     * @brief Construtor da classe Grafo.
     * @details Carrega um grafo a partir de um arquivo de texto, utilizando a
     * representa��o interna especificada.
     * @param caminhoArquivo O caminho para o arquivo .txt que cont�m a defini��o do grafo.
     * @param tipo O tipo de representa��o a ser usada (MATRIZ_ADJACENCIA ou LISTA_ADJACENCIA).
     * @throws std::runtime_error se o arquivo n�o puder ser aberto.
     */
    Grafo(const std::string& caminhoArquivo, TipoRepresentacao tipo);

    /**
     * @brief Destrutor da classe Grafo.
     * @details Libera os recursos alocados pelo grafo, especialmente a mem�ria
     * utilizada pela representa��o interna. O uso de std::unique_ptr automatiza este processo.
     */
    ~Grafo();

    // --- M�todos de Consulta de Propriedades B�sicas ---

    /**
     * @brief Obt�m o n�mero total de v�rtices no grafo.
     * @return O n�mero de v�rtices.
     */
    int obterNumeroVertices() const;

    /**
     * @brief Obt�m o n�mero total de arestas no grafo.
     * @return O n�mero de arestas.
     */
    int obterNumeroArestas() const;

    /**
     * @brief Obt�m o grau de um v�rtice espec�fico.
     * @param vertice O v�rtice (indexado a partir de 1) cujo grau � desejado.
     * @return O grau do v�rtice.
     */
    int obterGrau(int vertice) const;

    /**
     * @brief Obt�m uma lista com os vizinhos de um v�rtice.
     * @details Este m�todo � uma primitiva essencial para a maioria dos algoritmos em grafos.
     * @param vertice O v�rtice (indexado a partir de 1) do qual se deseja obter os vizinhos.
     * @return Um std::vector<int> contendo os �ndices dos v�rtices vizinhos.
     */
    std::vector<int> obterVizinhos(int vertice) const;


    // --- Futuros M�todos para Algoritmos e Relat�rios ---
    // (Ser�o implementados nos pr�ximos passos)
    //
    // void gerarRelatorioCompleto(const std::string& arquivoSaida) const;
    // ResultadoBusca executarBFS(int verticeInicial) const;
    // ... etc ...


private:
    // Ponteiro inteligente para a implementa��o da representa��o do grafo.
    // O uso de std::unique_ptr garante que a mem�ria seja gerenciada automaticamente (RAII).
    // Este � o n�cleo do padr�o de design "Ponte para Implementa��o" (PImpl) ou "Strategy",
    // que nos permite trocar a representa��o sem mudar a interface p�blica (Grafo).
    std::unique_ptr<Representacao> representacaoInterna;

    // Armazenamos contadores b�sicos aqui para acesso r�pido, evitando
    // a necessidade de consultar a representa��o interna a todo momento.
    int numeroDeVertices;
    int numeroDeArestas;
};