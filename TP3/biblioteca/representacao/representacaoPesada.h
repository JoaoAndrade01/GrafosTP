#pragma once

#include "representacao.h" // Inclui a interface base do TP1
#include <vector>
#include <functional> // Para std::function

// Estrutura para retornar vizinhos com pesos (pode vir de um .h comum depois)
struct VizinhoComPeso {
    int id;
    double peso;
};

/**
 * @class RepresentacaoPesada
 * @brief Interface base abstrata para representações de grafos COM PESOS.
 * @details Herda de Representacao (TP1) e adiciona/modifica métodos para
 * manipular pesos nas arestas.
 */
class RepresentacaoPesada : public Representacao {
public:
    /**
     * @brief Destrutor virtual padrão.
     */
    virtual ~RepresentacaoPesada() override = default;

    // --- Métodos Herdados de Representacao (TP1) que precisam ser implementados ---

    /**
     * @brief Obtém o grau de um vértice específico. (Herdado)
     * @param vertice O vértice (indexado em 1).
     * @return O grau do vértice.
     */
    virtual int obterGrau(int vertice) const override = 0;

    /**
     * @brief Obtém uma lista com os IDs dos vizinhos de um vértice. (Herdado)
     * @param vertice O vértice (indexado em 1).
     * @return Um std::vector<int> contendo os índices dos vértices vizinhos.
     * @note Para grafos pesados, prefira obterVizinhosComPesos.
     */
    virtual std::vector<int> obterVizinhos(int vertice) const override = 0;

    // finalizarConstrucao() já tem implementação padrão na base Representacao.

    // --- Novos Métodos Virtuais Puros (inspirados no TP2) para Pesos ---

    /**
     * @brief Adiciona uma aresta com peso ao grafo (ou ao buffer temporário).
     * @param u Vértice de origem.
     * @param v Vértice de destino.
     * @param w Peso da aresta.
     */
    virtual void adicionarArestaComPeso(int u, int v, double w) = 0;

    /**
     * @brief Obtém a lista de vizinhos de um vértice, incluindo os pesos das arestas.
     * @param vertice O vértice (indexado a partir de 1).
     * @return Um std::vector de structs VizinhoComPeso.
     */
    virtual std::vector<VizinhoComPeso> obterVizinhosComPesos(int vertice) const = 0;

    /**
     * @brief Obtém o peso da aresta entre dois vértices, se existir.
     * @param u Primeiro vértice.
     * @param v Segundo vértice.
     * @return O peso da aresta. Retorna infinito se a aresta não existir. Retorna 0.0 se u == v.
     */
    virtual double obterPesoAresta(int u, int v) const = 0;

    /**
     * @brief Itera sobre todos os vizinhos de 'u', aplicando uma função lambda.
     * @details Permite processar vizinhos sem criar um vetor temporário.
     * @param u O vértice de origem.
     * @param fn A função a ser chamada para cada vizinho 'v' com peso 'w'. Ex: [&](int v, double w){ ... }
     */
    virtual void paraCadaVizinhoComPeso(int u, const std::function<void(int v, double w)>& fn) const = 0;
};