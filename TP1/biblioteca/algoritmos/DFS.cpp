/**
 * @file DFS.cpp
 * @brief Implementação da classe DFS.
 */
#include "DFS.h"
#include "../../biblioteca/interface/Grafo.h"
#include <stack>   // Usaremos a pilha padrão do C++
#include <utility> // Para std::pair
#include <algorithm> // Para std::reverse

ResultadoDFS DFS::executar(const Grafo& grafo, int verticeOrigem) const {
    // Bloco 1: Preparação (similar ao seu "Desmarcar todos os vértices")
    const int numVertices = grafo.obterNumeroVertices();
    ResultadoDFS resultado;
    resultado.pai.assign(numVertices + 1, -1);
    resultado.nivel.assign(numVertices + 1, -1);
    std::vector<bool> visitado(numVertices + 1, false);

    // Bloco 2: Definir pilha P com um elemento s
    // Usamos uma pilha de pares para rastrear o vértice e seu pai na busca.
    std::stack<std::pair<int, int>> pilha;
    pilha.push({ verticeOrigem, 0 }); // O par é {vértice, pai_do_vértice}. Pai 0 para a origem.

    // Bloco 3: Enquanto P não estiver vazia
    while (!pilha.empty()) {
        // Bloco 4: Remover u de P
        std::pair<int, int> par = pilha.top();
        pilha.pop();
        int u = par.first;
        int pai_de_u = par.second;

        // Bloco 5: Se u não estiver marcado
        if (!visitado[u]) {
            // Bloco 6: Marcar u
            visitado[u] = true;
            resultado.pai[u] = pai_de_u;
            // O nível do filho é o nível do pai + 1
            resultado.nivel[u] = (pai_de_u == 0) ? 0 : resultado.nivel[pai_de_u] + 1;

            // Bloco 7: Para cada aresta (u,v) incidente a u
            // O DFS explora os vizinhos em ordem inversa para que a pilha se comporte
            // de forma mais parecida com a recursão. Podemos usar um vetor temporário para isso.
            std::vector<int> vizinhos = grafo.obterVizinhos(u);
            std::reverse(vizinhos.begin(), vizinhos.end());

            for (int v : vizinhos) {
                if (!visitado[v]) {
                    // Bloco 8: Adicionar v em P
                    pilha.push({ v, u }); // Adicionamos o vizinho e quem o descobriu (seu pai, u)
                }
            }
        }
    }
    return resultado;
}