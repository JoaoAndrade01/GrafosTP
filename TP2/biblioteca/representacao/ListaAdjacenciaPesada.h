#pragma once
#include "representacao.h"
#include <vector>
#include <functional>

class ListaAdjacenciaPesada : public Representacao {
public:
    explicit ListaAdjacenciaPesada(int n);

    // Construção
    void adicionarAresta(int u, int v, double w) override;
    void finalizarConstrucao() override;

    // Consulta
    int        obterNumeroDeVertices() const override;
    long long  obterNumeroDeArestas()  const override;
    void paraCadaVizinho(int u, const std::function<void(int,double)>& fn) const override;

private:
    int N;              // nº de vértices (1..N)
    long long M;        // nº de arestas armazenadas (após duplicação u->v e v->u)

    // CSR
    std::vector<int>    head; // size N+2, usa índices [1..N+1]
    std::vector<int>    nbr;  // size M
    std::vector<double> wgt;  // size M

    // buffers de construção
    std::vector<int>    edges_u, edges_v;
    std::vector<double> edges_w;
};

