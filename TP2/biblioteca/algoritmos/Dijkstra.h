#pragma once
#include <vector>
#include <limits>
#include <cmath>
#include "../representacao/representacao.h"

namespace tp2 {

using Dist = double;
inline constexpr Dist INF = std::numeric_limits<Dist>::infinity();

struct DijkstraResultado {
    std::vector<Dist> dist;  // distância mínima s->v
    std::vector<int>  pai;   // predecessor
};

// Implementação com vetor
DijkstraResultado dijkstra_vector(const Representacao& G, int s);

} // namespace tp2

