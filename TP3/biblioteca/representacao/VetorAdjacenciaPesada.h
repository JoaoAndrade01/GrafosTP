#pragma once

#include "representacaoPesada.h" 
#include <vector>
#include <utility> // Para std::pair

// Estrutura temporária para armazenar arestas durante a leitura (pode ser movida para um .h comum)
struct ArestaComPeso {
    int u, v;
    double peso;
};

/**
 * @class VetorAdjacenciaPesada
 * @brief Implementação CSR otimizada para grafos COM PESOS, direcionados ou não.
 * @details Herda de RepresentacaoPesada e implementa a lógica de armazenamento
 * compacto usando vetores para offsets, vizinhos e pesos.
 */
class VetorAdjacenciaPesada : public RepresentacaoPesada {
public:
    /**
     * @brief Construtor da classe VetorAdjacenciaPesada.
     * @param numeroDeVertices O número total de vértices que o grafo terá.
     * @param direcionado Indica se o grafo é direcionado (false por padrão para
     * manter compatibilidade com grafos não direcionados).
     */
    explicit VetorAdjacenciaPesada(int numeroDeVertices, bool direcionado = false);

    /**
     * @brief Destrutor padrão.
     */
    ~VetorAdjacenciaPesada() override = default;

    // --- Implementação dos Métodos Virtuais ---

    // Métodos específicos da RepresentacaoPesada
    void adicionarArestaComPeso(int u, int v, double w) override;
    std::vector<VizinhoComPeso> obterVizinhosComPesos(int vertice) const override;
    double obterPesoAresta(int u, int v) const override;
    void paraCadaVizinhoComPeso(int u, const std::function<void(int v, double w)>& fn) const override;

    // Métodos originalmente da Representacao (TP1), herdados via RepresentacaoPesada
    int obterGrau(int vertice) const override;
    std::vector<int> obterVizinhos(int vertice) const override;
    void finalizarConstrucao() override; // Essencial para CSR

    // adicionarAresta(u, v) sem peso já tem implementação final na RepresentacaoPesada

private:
    int numeroDeVertices;
    int numeroDeArestas; // Número de arestas únicas (linhas no arquivo de entrada)
	bool ehDirecionado;

    // Estrutura principal do CSR com pesos
    std::vector<int> ponteirosInicio;    // Offsets
    std::vector<int> listaVizinhos;      // Vizinhos concatenados
    std::vector<double> listaPesos;      // Pesos correspondentes

    // Buffers temporários usados durante a construção
    std::vector<int> grausTemporarios;
    std::vector<ArestaComPeso> arestasTemporarias; // Armazena arestas lidas
};