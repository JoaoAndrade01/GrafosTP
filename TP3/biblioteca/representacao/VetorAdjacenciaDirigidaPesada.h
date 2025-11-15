#ifndef VETOR_ADJACENCIA_DIRIGIDA_PESADA_H
#define VETOR_ADJACENCIA_DIRIGIDA_PESADA_H

#include <vector>
#include <functional>
#include "representacaoPesada.h"

/**
 * @brief Representação CSR para grafo DIRIGIDO e PONDERADO.
 * Usa a interface abstrata representacaoPesada.
 */
class VetorAdjacenciaDirigidaPesada : public representacaoPesada {
public:
    explicit VetorAdjacenciaDirigidaPesada(int n);
    ~VetorAdjacenciaDirigidaPesada() override = default;

    // Construção
    void adicionarArestaComPeso(int u, int v, double w) override;
    void finalizarConstrucao() override;

    // Consultas
    int obterGrau(int vertice) const override;                 // grau de saída
    std::vector<int> obterVizinhos(int vertice) const override;
    std::vector<VizinhoComPeso> obterVizinhosComPesos(int vertice) const override;
    double obterPesoAresta(int u, int v) const override;
    void paraCadaVizinhoComPeso(
        int u,
        const std::function<void(int v, double w)>& fn
    ) const override;

private:
    int numeroDeVertices;
    long long numeroDeArestas;

    // CSR:
    // ponteirosInicio[u] e ponteirosInicio[u+1] delimitam o intervalo
    // de vizinhos de u em listaVizinhos/listaPesos
    std::vector<int> ponteirosInicio;
    std::vector<int> listaVizinhos;
    std::vector<double> listaPesos;

    // buffers temporários para construção
    struct ArestaTemp {
        int u, v;
        double peso;
    };
    std::vector<ArestaTemp> arestasTemporarias;
    std::vector<int> grausTemporarios;
};

#endif // VETOR_ADJACENCIA_DIRIGIDA_PESADA_H

