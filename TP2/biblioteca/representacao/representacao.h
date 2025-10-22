#pragma once
#include <functional>

class Representacao {
public:
    virtual ~Representacao() = default;

    // Consulta
    virtual int        obterNumeroDeVertices() const = 0;
    virtual long long  obterNumeroDeArestas()  const = 0;

    // Itera vizinhos de u chamando fn(v, w)
    virtual void paraCadaVizinho(int u, const std::function<void(int,double)>& fn) const = 0;

    // Construção
    virtual void adicionarAresta(int u, int v, double w) = 0;
    virtual void finalizarConstrucao() {} // opcional
};

