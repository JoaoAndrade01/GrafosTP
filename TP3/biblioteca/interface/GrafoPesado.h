#pragma once

#include <vector>
#include <string>
#include <memory> // Para std::unique_ptr
#include <limits> // Para std::numeric_limits

// Inclui a interface da representação pesada e a struct VizinhoComPeso
#include "../representacao/representacaoPesada.h" 
// Inclui a definição de Dijkstra e ResultadoDijkstra
#include "../algoritmos/Dijkstra.h" 

// Declaração antecipada da classe de representação concreta que usaremos
class VetorAdjacenciaPesada;

/**
 * @class GrafoPesado
 * @brief Classe principal que representa um grafo NÃO DIRECIONADO COM PESOS.
 * @details Atua como a interface pública (Fachada), escondendo a complexidade
 * da representação interna (VetorAdjacenciaPesada/CSR) e fornecendo métodos
 * para consulta e análise do grafo ponderado.
 */
class GrafoPesado {
public:
    /**
     * @brief Construtor que carrega um grafo ponderado a partir de um arquivo.
     * @details Lê o arquivo no formato 'u v peso', instancia a representação
     * interna (VetorAdjacenciaPesada) e finaliza sua construção.
     * @param caminhoArquivo O caminho para o arquivo .txt.
     * @throws std::runtime_error Se o arquivo não puder ser aberto ou tiver formato inválido.
     */
    explicit GrafoPesado(const std::string& caminhoArquivo);

    /**
     * @brief Destrutor padrão. Gerencia a memória da representação interna via unique_ptr.
     */
    ~GrafoPesado();

    
    // --- Métodos de Consulta ---

    /**
     * @brief Obtém o número total de vértices no grafo.
     * @return O número de vértices (N).
     */
    int obterNumeroVertices() const;

    /**
     * @brief Obtém o número total de arestas únicas lidas do arquivo.
     * @return O número de arestas (M).
     */
    long long obterNumeroArestas() const;

    /**
     * @brief Obtém o grau de um vértice específico.
     * @param u O vértice (indexado a partir de 1).
     * @return O grau do vértice u.
     */
    int obterGrau(int u) const;

    /**
     * @brief Obtém a lista de vizinhos de um vértice, incluindo os pesos das arestas.
     * @param u O vértice (indexado a partir de 1).
     * @return Um std::vector de structs VizinhoComPeso.
     */
    std::vector<VizinhoComPeso> obterVizinhosComPesos(int u) const;

    /**
     * @brief Obtém o peso da aresta entre dois vértices, se existir.
     * @param u Primeiro vértice.
     * @param v Segundo vértice.
     * @return O peso da aresta, ou infinito se a aresta não existir. Retorna 0.0 se u == v.
     */
    double obterPesoAresta(int u, int v) const;

    /**
    * @brief Verifica rapidamente se o grafo contém alguma aresta com peso negativo.
    * @details Útil para determinar a aplicabilidade de certos algoritmos (ex: Dijkstra).
    * @return true se existe pelo menos uma aresta com peso negativo, false caso contrário.
    */
    bool temPesoNegativo() const;

    // --- Métodos para Dijkstra ---

        /**
         * @brief Executa o algoritmo de Dijkstra usando Heap.
         * @param origem Vértice de origem (1-based).
         * @return ResultadoDijkstra contendo distâncias e predecessores.
         */
    ResultadoDijkstra executarDijkstraHeap(int origem) const;

    /**
     * @brief Executa o algoritmo de Dijkstra usando Vetor.
     * @param origem Vértice de origem (1-based).
     * @return ResultadoDijkstra contendo distâncias e predecessores.
     */
    ResultadoDijkstra executarDijkstraVetor(int origem) const;



private:
    // Ponteiro inteligente para a representação interna (sempre VetorAdjacenciaPesada por enquanto)
    std::unique_ptr<representacaoPesada> representacaoInterna;

    // Informações básicas cacheadas para acesso rápido
    int numeroDeVertices;
    long long numeroDeArestas;
    bool possuiPesoNegativo; // Cache para a verificação de peso negativo
};
