#ifndef REPRESENTACAO_PESADA_H
#define REPRESENTACAO_PESADA_H

#include <vector>
#include <functional>

// Estrutura básica de vizinho com peso
struct VizinhoComPeso {
    int id;
    double peso;
};

/**
 * @brief Classe base abstrata para representações de grafos ponderados (CSR, etc.).
 * Interface independente, sem herdar de outra classe.
 */
class representacaoPesada {
public:
    virtual ~representacaoPesada() = default;

    // --- Construção do grafo ---
    virtual void adicionarArestaComPeso(int u, int v, double w) = 0;
    virtual void finalizarConstrucao() = 0;

    // --- Consultas básicas ---
    virtual int obterGrau(int vertice) const = 0;
    virtual std::vector<int> obterVizinhos(int vertice) const = 0;

    // --- Consultas com pesos ---
    virtual std::vector<VizinhoComPeso> obterVizinhosComPesos(int vertice) const = 0;
    virtual double obterPesoAresta(int u, int v) const = 0;

    // Iterador com lambda
    virtual void paraCadaVizinhoComPeso(
        int u,
        const std::function<void(int v, double w)>& fn
    ) const = 0;
};

#endif // REPRESENTACAO_PESADA_H

