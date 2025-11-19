#pragma once

#include "representacaoPesada.h" 
#include <vector>
#include <utility> // Para std::pair

struct ArestaComPeso {
    int u, v;
    double peso;
};

/**
 * @class VetorAdjacenciaPesada
 * @brief Implementação CSR otimizada para grafos COM PESOS, direcionados ou não.
 */
class VetorAdjacenciaPesada : public RepresentacaoPesada {
public:
    /**
     * @brief Construtor da classe VetorAdjacenciaPesada.
     * @param numeroDeVertices O número total de vértices que o grafo terá.
     * @param direcionado Indica se o grafo é direcionado (default false).
     * @param transposto Indica se as arestas devem ser invertidas na carga (default false).
     */
    explicit VetorAdjacenciaPesada(int numeroDeVertices, bool direcionado = false, bool transposto = false);

    ~VetorAdjacenciaPesada() override = default;

    // --- Implementação dos Métodos Virtuais ---
    void adicionarArestaComPeso(int u, int v, double w) override;
    std::vector<VizinhoComPeso> obterVizinhosComPesos(int vertice) const override;
    double obterPesoAresta(int u, int v) const override;
    void paraCadaVizinhoComPeso(int u, const std::function<void(int v, double w)>& fn) const override;

    int obterGrau(int vertice) const override;
    std::vector<int> obterVizinhos(int vertice) const override;
    void finalizarConstrucao() override;

private:
    int numeroDeVertices;
    int numeroDeArestas;
    bool ehDirecionado;
    bool ehTransposto; // Novo campo para controlar a inversão

    // Estrutura principal do CSR
    std::vector<int> ponteirosInicio;
    std::vector<int> listaVizinhos;
    std::vector<double> listaPesos;

    // Buffers temporários
    std::vector<int> grausTemporarios;
    std::vector<ArestaComPeso> arestasTemporarias;
};