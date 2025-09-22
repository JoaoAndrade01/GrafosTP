#pragma once
/**
 * @file Grafo.h
 * @brief Defini��o da classe Grafo, a principal interface da biblioteca.
 */

#include <string>
#include <vector>
#include <memory> // Necess�rio para std::unique_ptr

 // Inclui as defini��es dos algoritmos e de seus respectivos resultados
#include "../algoritmos/BFS.h"
#include "../algoritmos/DFS.h"

// --- DEFINI��O DAS STRUCTS DE RESULTADO ---

/**
 * @struct ComponenteConexa
 * @brief Armazena os dados de uma �nica componente conexa.
 */
struct ComponenteConexa {
    int tamanho = 0; // Quantidade de v�rtices na componente
    std::vector<int> vertices; // Lista de v�rtices que pertencem a esta componente
};

/**
 * @struct EstatisticasGrau
 * @brief Armazena estat�sticas calculadas sobre os graus dos v�rtices do grafo.
 */
struct EstatisticasGrau {
    int grauMin = 0;
    int grauMax = 0;
    double grauMedio = 0.0;
    double grauMediana = 0.0;
};


// --- DECLARA��O DA CLASSE PRINCIPAL ---

// Declara��o antecipada da classe de representa��o.
class Representacao;

/**
 * @enum TipoRepresentacao
 * @brief Enumera��o para especificar a representa��o interna do grafo a ser utilizada.
 */
enum class TipoRepresentacao {
    MATRIZ_ADJACENCIA,
    MATRIZ_ADJACENCIA_TRIANGULAR,
    LISTA_ADJACENCIA, // Vers�o Otimizada (CSR)
    LISTA_ADJACENCIA_SIMPLES
};

/**
 * @class Grafo
 * @brief Classe principal que representa um grafo e serve como a API p�blica da biblioteca.
 * @details Abstrai a representa��o interna do grafo e fornece m�todos para an�lise.
 */
class Grafo {
public:
    /**
     * @brief Construtor da classe Grafo.
     * @details Carrega um grafo a partir de um arquivo de texto, utilizando a
     * representa��o interna especificada.
     * @param caminhoArquivo O caminho para o arquivo .txt que cont�m a defini��o do grafo.
     * @param tipo O tipo de representa��o a ser usada.
     * @throws std::runtime_error se o arquivo n�o puder ser aberto.
     */
    Grafo(const std::string& caminhoArquivo, TipoRepresentacao tipo);

    /**
     * @brief Destrutor da classe Grafo.
     * @details Libera os recursos alocados pelo grafo. O uso de std::unique_ptr automatiza este processo.
     */
    ~Grafo();

    // --- M�TODOS DE CONSULTA B�SICA ---

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

    // --- M�TODOS DE ALGORITMOS ---

    /**
     * @brief Executa a Busca em Largura (BFS) a partir de um v�rtice de origem.
     * @param verticeOrigem O v�rtice (indexado a partir de 1) para iniciar a busca.
     * @return Um struct ResultadoBFS contendo a �rvore de busca (vetores de pai e n�vel).
     */
    ResultadoBFS executarBFS(int verticeOrigem) const;

    /**
     * @brief Executa a Busca em Profundidade (DFS) a partir de um v�rtice de origem.
     * @param verticeOrigem O v�rtice (indexado a partir de 1) para iniciar a busca.
     * @return Um struct ResultadoDFS contendo a �rvore de busca (vetores de pai e n�vel).
     */
    ResultadoDFS executarDFS(int verticeOrigem) const;

    /**
     * @brief Calcula a distancia (menor caminho em arestas) entre dois vertices.
     * @details Utiliza o algoritmo BFS como primitiva.
     * @param verticeU O vertice de origem.
     * @param verticeV O vertice de destino.
     * @return O n�mero de arestas no caminho mais curto. Retorna -1 se v for inalcan��vel a partir de u.
     */
    int calcularDistancia(int verticeU, int verticeV) const;

    /**
     * @brief Calcula o di�metro EXATO do grafo. CUIDADO: muito lento para grafos grandes.
     * @details Algoritmo de for�a bruta que executa um BFS a partir de cada v�rtice.
     * @return O di�metro do grafo, ou -1 se for desconexo.
     */
    int calcularDiametro() const;

    /**
     * @brief Estima o di�metro do grafo com uma heur�stica r�pida.
     * @param iteracoes O n�mero de vezes que a heur�stica ser� executada (default = 5).
     * @return O di�metro aproximado do grafo.
     */
    int calcularDiametroAproximado(int iteracoes = 5) const;

    /**
     * @brief Encontra todas as componentes conexas do grafo.
     * @details Utiliza um algoritmo de busca para encontrar todos os subgrafos conexos.
     * @return Um vetor de structs ComponenteConexa, j� ordenado por tamanho em ordem decrescente.
     */
    std::vector<ComponenteConexa> encontrarComponentesConexas() const;

    // --- M�TODOS DE AN�LISE E RELAT�RIO ---

    /**
     * @brief Calcula as estat�sticas de grau (m�n, m�x, m�dio, mediana) do grafo.
     * @return Um struct EstatisticasGrau com os resultados.
     */
    EstatisticasGrau calcularEstatisticasGrau() const;

    /**
         * @brief Gera um relatorio completo e o salva em um arquivo. (Vers�o de conveni�ncia)
         * @param caminhoArquivo O nome do arquivo onde o relatorio sera salvo.
         */
    void gerarRelatorioCompleto(const std::string& caminhoArquivo) const;

    /**
     * @brief Gera um relatorio completo e o envia para um fluxo de sa�da (arquivo, console, etc.). (Vers�o principal)
     * @param saida O fluxo de sa�da (ex: std::cout ou um std::ofstream).
     */
    void gerarRelatorioCompleto(std::ostream& saida) const;


    // --- M�TODOS DE SALVAMENTO ---

    /**
     * @brief Salva a �rvore de busca gerada pelo BFS em um arquivo de texto.
     * @param resultado O struct ResultadoBFS retornado pelo m�todo de busca.
     * @param caminhoArquivo O nome do arquivo onde a �rvore ser� salva.
     */
    void salvarArvoreBusca(const ResultadoBFS& resultado, const std::string& caminhoArquivo) const;

    /**
     * @brief Salva a �rvore de busca gerada pelo DFS em um arquivo de texto.
     * @param resultado O struct ResultadoDFS retornado pelo m�todo de busca.
     * @param caminhoArquivo O nome do arquivo onde a �rvore ser� salva.
     */
    void salvarArvoreBusca(const ResultadoDFS& resultado, const std::string& caminhoArquivo) const;


private:
    /**
     * @brief Ponteiro inteligente para a implementa��o da representa��o do grafo.
     * @details O uso de std::unique_ptr garante que a mem�ria seja gerenciada automaticamente (RAII).
     * Este � o n�cleo do padr�o de design "Strategy", que nos permite trocar a
     * representa��o sem mudar a interface p�blica.
     */
    std::unique_ptr<Representacao> representacaoInterna;

    /**
     * @brief Armazena o n�mero de v�rtices para acesso r�pido.
     */
    int numeroDeVertices;

    /**
     * @brief Armazena o n�mero de arestas para acesso r�pido.
     */
    int numeroDeArestas;
};

