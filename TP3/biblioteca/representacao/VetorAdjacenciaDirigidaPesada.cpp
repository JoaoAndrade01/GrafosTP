/**
 * @file VetorAdjacenciaDirigidaPesada.cpp
 * @brief Implementação dos métodos da classe VetorAdjacenciaDirigidaPesada (CSR dirigido com pesos).
 */

#include "VetorAdjacenciaDirigidaPesada.h"
#include <stdexcept>
#include <algorithm> // std::sort
#include <limits>    // std::numeric_limits

/**
 * @brief Construtor da VetorAdjacenciaDirigidaPesada.
 */
VetorAdjacenciaDirigidaPesada::VetorAdjacenciaDirigidaPesada(int n)
    : numeroDeVertices(n),
      numeroDeArestas(0),
      // Aloca N+2 para simplificar o acesso a ponteirosInicio[N+1]
      ponteirosInicio(n + 2, 0),
      grausTemporarios(n + 1, 0)
{
    if (n <= 0) {
        throw std::invalid_argument("O numero de vertices deve ser positivo.");
    }
}

/**
 * @brief Adiciona uma aresta dirigida com peso ao buffer temporário.
 *
 * Aqui interpretamos SEMPRE como u -> v (grafo dirigido).
 */
void VetorAdjacenciaDirigidaPesada::adicionarArestaComPeso(int u, int v, double w) {
    // Validação básica (ignora laços e vértices inválidos)
    if (u > 0 && u <= numeroDeVertices &&
        v > 0 && v <= numeroDeVertices &&
        u != v)
    {
        arestasTemporarias.push_back({ u, v, w });
        // numeroDeArestas será definido em finalizarConstrucao()
    }
}

/**
 * @brief Constrói a estrutura CSR final (offsets, vizinhos, pesos) para grafo DIRIGIDO.
 */
void VetorAdjacenciaDirigidaPesada::finalizarConstrucao() {
    this->numeroDeArestas = static_cast<long long>(arestasTemporarias.size());

    // 1. Contagem dos graus de SAÍDA (out-degree)
    for (const auto& aresta : arestasTemporarias) {
        grausTemporarios[aresta.u]++;   // apenas u -> v
    }

    // 2. Cálculo da soma de prefixos (offsets)
    ponteirosInicio[1] = 0;
    for (int i = 1; i <= numeroDeVertices; ++i) {
        ponteirosInicio[i + 1] = ponteirosInicio[i] + grausTemporarios[i];
    }

    // 3. Alocação e preenchimento dos vetores de vizinhos e pesos
    int tamanhoTotalVizinhos = ponteirosInicio[numeroDeVertices + 1];
    listaVizinhos.assign(tamanhoTotalVizinhos, -1);
    listaPesos.assign(tamanhoTotalVizinhos,
                      std::numeric_limits<double>::infinity());

    // Cópia dos offsets para uso como "cursor" de escrita
    std::vector<int> ponteirosEscrita = ponteirosInicio;

    for (const auto& aresta : arestasTemporarias) {
        int u = aresta.u;
        int v = aresta.v;
        double peso = aresta.peso;

        // Adiciona v na lista de saída de u (u -> v)
        int indiceUV = ponteirosEscrita[u]++;
        listaVizinhos[indiceUV] = v;
        listaPesos[indiceUV] = peso;
    }

    // 4. Ordenação dos vizinhos de cada vértice (opcional, mas útil)
    for (int u = 1; u <= numeroDeVertices; ++u) {
        int inicio = ponteirosInicio[u];
        int fim = ponteirosInicio[u + 1];

        if (inicio < fim) {
            std::vector<std::pair<int, double>> vizinhosComPesos;
            vizinhosComPesos.reserve(fim - inicio);

            for (int i = inicio; i < fim; ++i) {
                vizinhosComPesos.emplace_back(listaVizinhos[i], listaPesos[i]);
            }

            std::sort(vizinhosComPesos.begin(), vizinhosComPesos.end()); // ordena por ID do vizinho

            for (int i = 0; i < static_cast<int>(vizinhosComPesos.size()); ++i) {
                listaVizinhos[inicio + i] = vizinhosComPesos[i].first;
                listaPesos[inicio + i]   = vizinhosComPesos[i].second;
            }
        }
    }

    // 5. Liberação da memória temporária
    arestasTemporarias.clear();
    arestasTemporarias.shrink_to_fit();
    grausTemporarios.clear();
    grausTemporarios.shrink_to_fit();
}

/**
 * @brief Obtém o grau de saída de um vértice (O(1)).
 */
int VetorAdjacenciaDirigidaPesada::obterGrau(int vertice) const {
    if (vertice > 0 && vertice <= numeroDeVertices) {
        // Diferença entre os ponteiros define o número de vizinhos de saída
        return ponteirosInicio[vertice + 1] - ponteirosInicio[vertice];
    }
    throw std::out_of_range("Vertice invalido ao obter grau.");
}

/**
 * @brief Obtém apenas os IDs dos vizinhos de saída (para compatibilidade com Representacao).
 */
std::vector<int> VetorAdjacenciaDirigidaPesada::obterVizinhos(int vertice) const {
    if (vertice <= 0 || vertice > numeroDeVertices) {
        throw std::out_of_range("Vertice invalido ao obter vizinhos.");
    }

    auto inicio = listaVizinhos.begin() + ponteirosInicio[vertice];
    auto fim    = listaVizinhos.begin() + ponteirosInicio[vertice + 1];
    return std::vector<int>(inicio, fim);
}

/**
 * @brief Obtém os vizinhos de saída e os pesos das arestas correspondentes.
 */
std::vector<VizinhoComPeso> VetorAdjacenciaDirigidaPesada::obterVizinhosComPesos(int vertice) const {
    if (vertice <= 0 || vertice > numeroDeVertices) {
        throw std::out_of_range("Vertice invalido ao obter vizinhos com pesos.");
    }

    std::vector<VizinhoComPeso> resultado;
    int inicio = ponteirosInicio[vertice];
    int fim    = ponteirosInicio[vertice + 1];
    resultado.reserve(fim - inicio);

    for (int i = inicio; i < fim; ++i) {
        resultado.push_back({ listaVizinhos[i], listaPesos[i] });
    }
    return resultado;
}

/**
 * @brief Itera sobre vizinhos de saída e pesos usando uma função lambda.
 */
void VetorAdjacenciaDirigidaPesada::paraCadaVizinhoComPeso(
    int u,
    const std::function<void(int v, double w)>& fn) const
{
    if (u <= 0 || u > numeroDeVertices) {
        throw std::out_of_range("Vertice invalido em paraCadaVizinhoComPeso.");
    }

    int inicio = ponteirosInicio[u];
    int fim    = ponteirosInicio[u + 1];
    for (int i = inicio; i < fim; ++i) {
        fn(listaVizinhos[i], listaPesos[i]);
    }
}

/**
 * @brief Obtém o peso da aresta dirigida u -> v (O(grau(u))).
 */
double VetorAdjacenciaDirigidaPesada::obterPesoAresta(int u, int v) const {
    if (u <= 0 || u > numeroDeVertices ||
        v <= 0 || v > numeroDeVertices) {
        throw std::out_of_range("Vertices invalidos ao obter peso da aresta.");
    }
    if (u == v) {
        return 0.0;
    }

    int inicio = ponteirosInicio[u];
    int fim    = ponteirosInicio[u + 1];

    for (int i = inicio; i < fim; ++i) {
        if (listaVizinhos[i] == v) {
            return listaPesos[i];
        }
    }

    return std::numeric_limits<double>::infinity(); // aresta u->v não existe
}
