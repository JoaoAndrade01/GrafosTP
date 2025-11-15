#pragma once

#include <vector>
#include <string>
#include <memory>  // std::unique_ptr
#include <limits>  // std::numeric_limits

// Interface da representação pesada e a struct VizinhoComPeso
#include "../representacao/representacaoPesada.h"
// Definição de Dijkstra e ResultadoDijkstra
#include "../algoritmos/Dijkstra.h"

// Declaração antecipada da representação concreta que usaremos no TP3
class VetorAdjacenciaDirigidaPesada;

/**
 * @class GrafoDirigidoPesado
 * @brief Classe principal que representa um grafo DIRECIONADO COM PESOS.
 *
 * @details Atua como uma fachada: esconde a complexidade da representação
 * interna (por exemplo, VetorAdjacenciaDirigidaPesada em CSR) e expõe
 * métodos de consulta e análise para grafos ponderados direcionados.
 *
 * Convenção: as arestas lidas do arquivo são do tipo (u -> v, peso),
 * ou seja, apenas a direção u -> v é armazenada.
 */
class GrafoDirigidoPesado {
public:
    /**
     * @brief Construtor que carrega um grafo ponderado direcionado a partir de um arquivo.
     *
     * @details
     * Formato esperado do arquivo:
     *   - Primeira linha: um inteiro N > 0 representando o número de vértices.
     *   - Demais linhas: triplas "u v peso", representando uma aresta
     *     dirigida de u para v com um dado peso (double).
     *
     * @param caminhoArquivo Caminho para o arquivo .txt contendo o grafo.
     * @throws std::runtime_error Se o arquivo não puder ser aberto ou tiver formato inválido.
     */
    explicit GrafoDirigidoPesado(const std::string& caminhoArquivo);

    /**
     * @brief Destrutor padrão.
     *
     * @details Necessário porque usamos forward declaration + unique_ptr
     * para tipos incompletos no header. Garante destruição correta.
     */
    ~GrafoDirigidoPesado();

    // --- Métodos de Consulta ---

    /**
     * @brief Obtém o número total de vértices no grafo.
     * @return O número de vértices (N).
     */
    int obterNumeroVertices() const;

    /**
     * @brief Obtém o número total de arestas direcionadas lidas do arquivo.
     * @return O número de arestas (M).
     */
    long long obterNumeroArestas() const;

    /**
     * @brief Obtém o grau de saída de um vértice específico.
     *
     * @details
     * Como o grafo é direcionado, "grau" aqui será entendido como
     * **grau de saída** (out-degree), ou seja, número de arestas
     * que saem do vértice u.
     *
     * @param u Vértice (indexado a partir de 1).
     * @return O grau de saída de u.
     */
    int obterGrau(int u) const;

    /**
     * @brief Obtém a lista de vizinhos de um vértice, incluindo pesos das arestas.
     *
     * @details
     * Para um vértice u, retorna todos os vértices v tais que exista
     * uma aresta dirigida (u -> v).
     *
     * @param u Vértice de origem (1-based).
     * @return std::vector<VizinhoComPeso> com (id, peso).
     */
    std::vector<VizinhoComPeso> obterVizinhosComPesos(int u) const;

    /**
     * @brief Obtém o peso da aresta dirigida entre dois vértices, se existir.
     *
     * @param u Vértice de origem.
     * @param v Vértice de destino.
     *
     * @return
     *  - Peso da aresta (u -> v), se existir.
     *  - 0.0, se u == v.
     *  - +infinito (std::numeric_limits<double>::infinity()), se não existir aresta (u -> v).
     */
    double obterPesoAresta(int u, int v) const;

    /**
     * @brief Verifica rapidamente se o grafo contém alguma aresta com peso negativo.
     *
     * @details Útil para decidir se é seguro rodar Dijkstra, que não suporta
     * pesos negativos.
     *
     * @return true se existir pelo menos uma aresta com peso < 0.0.
     * @return false caso contrário.
     */
    bool temPesoNegativo() const;

    // --- Métodos para Dijkstra ---

    /**
     * @brief Executa o algoritmo de Dijkstra usando Heap.
     *
     * @param origem Vértice de origem (1-based).
     * @return ResultadoDijkstra contendo distâncias e predecessores.
     *
     * @throws std::runtime_error se houver peso negativo.
     */
    ResultadoDijkstra executarDijkstraHeap(int origem) const;

    /**
     * @brief Executa o algoritmo de Dijkstra usando Vetor.
     *
     * @param origem Vértice de origem (1-based).
     * @return ResultadoDijkstra contendo distâncias e predecessores.
     *
     * @throws std::runtime_error se houver peso negativo.
     */
    ResultadoDijkstra executarDijkstraVetor(int origem) const;

private:
    // Ponteiro inteligente para a representação interna
    // (por enquanto será sempre algo como VetorAdjacenciaDirigidaPesada)
    std::unique_ptr<representacaoPesada> representacaoInterna;

    // Informações básicas, cacheadas
    int        numeroDeVertices;
    long long  numeroDeArestas;
    bool       possuiPesoNegativo;
};
