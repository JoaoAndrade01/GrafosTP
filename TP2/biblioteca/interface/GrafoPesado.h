#pragma once
#include <vector>
#include <string>
#include <stdexcept>

struct VizinhoPeso {
    int v;   // vizinho (1..N)
    double w;   // peso (pode ser negativo nos arquivos; Dijkstra não aceita)
};

class IGrafoPesado {
public:
    virtual ~IGrafoPesado() = default;

    virtual int  numVertices()                           const = 0;
    virtual long numArestas()                            const = 0;
    virtual const std::vector<VizinhoPeso>& vizinhos(int u) const = 0; // retorna a lista de (v, w) de u
    virtual int  grau(int u)                             const = 0;

    // informação útil para escolher algoritmo
    virtual bool temPesoNegativo()                       const = 0;
};
