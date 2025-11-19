/**
 * @file VetorAdjacenciaPesada.cpp
 * @brief Implementação dos métodos da classe VetorAdjacenciaPesada (CSR com pesos).
 */

#include "VetorAdjacenciaPesada.h"
#include <stdexcept>
#include <algorithm> // Para std::sort, std::find_if
#include <vector>
#include <limits>    // Para std::numeric_limits

 /**
  * @brief Construtor da VetorAdjacenciaPesada.
  */
VetorAdjacenciaPesada::VetorAdjacenciaPesada(int n, bool direcionado)
    : numeroDeVertices(n), numeroDeArestas(0), ehDirecionado(direcionado),
    // Aloca N+2 para simplificar o cálculo do grau do último vértice (N)
    ponteirosInicio(n + 2, 0),
    grausTemporarios(n + 1, 0) {
    if (n <= 0) {
        throw std::invalid_argument("O numero de vertices deve ser positivo.");
    }
}

/**
 * @brief Adiciona uma aresta com peso ao buffer temporário.
 */
void VetorAdjacenciaPesada::adicionarArestaComPeso(int u, int v, double w) {
    // Validação básica (ignora laços e vértices inválidos)
    if (u > 0 && u <= numeroDeVertices && v > 0 && v <= numeroDeVertices && u != v) {
        arestasTemporarias.push_back({ u, v, w });
        // O número total de arestas será definido em finalize
    }
}

/**
 * @brief Constrói a estrutura CSR final (offsets, vizinhos, pesos).
 */
void VetorAdjacenciaPesada::finalizarConstrucao() {
    this->numeroDeArestas = arestasTemporarias.size();

    // 1. Contagem dos graus
	std::fill(grausTemporarios.begin(), grausTemporarios.end(), 0);
    for (const auto& aresta : arestasTemporarias) {
        grausTemporarios[aresta.u]++;
        if (!ehDirecionado) {
            grausTemporarios[aresta.v]++;
        }
    }

    // 2. Cálculo da soma de prefixos (offsets)
    ponteirosInicio[1] = 0;
    for (int i = 1; i <= numeroDeVertices; ++i) {
        ponteirosInicio[i + 1] = ponteirosInicio[i] + grausTemporarios[i];
    }

    // 3. Alocação e Preenchimento dos vetores de vizinhos e pesos
    // O tamanho total é 2*M (cada aresta entra duas vezes)
    int tamanhoTotalVizinhos = ponteirosInicio[numeroDeVertices + 1];
    listaVizinhos.assign(tamanhoTotalVizinhos, -1);
    listaPesos.assign(tamanhoTotalVizinhos, std::numeric_limits<double>::infinity()); // Inicializa pesos com infinito

    std::vector<int> ponteirosEscrita = ponteirosInicio; // Cópia para usar como cursor
    for (const auto& aresta : arestasTemporarias) {
        int u = aresta.u;
        int v = aresta.v;
        double peso = aresta.peso;

        // Adiciona v na lista de u
        int indiceUV = ponteirosEscrita[u]++;
        listaVizinhos[indiceUV] = v;
        listaPesos[indiceUV] = peso;

		// Adiciona u na lista de v somente se for não direcionado
        if (!ehDirecionado) {
            int indiceVU = ponteirosEscrita[v]++;
            listaVizinhos[indiceVU] = u;
            listaPesos[indiceVU] = peso;
		}
    }

    // 4. Ordenação (Opcional, mas útil para obterPesoAresta e consistência)
    // Precisamos ordenar vizinhos e pesos juntos. Criamos pares temporários.
    for (int u = 1; u <= numeroDeVertices; ++u) {
        int inicio = ponteirosInicio[u];
        int fim = ponteirosInicio[u + 1];
        if (inicio < fim) {
            std::vector<std::pair<int, double>> vizinhosComPesos;
            vizinhosComPesos.reserve(fim - inicio);
            for (int i = inicio; i < fim; ++i) {
                vizinhosComPesos.emplace_back(listaVizinhos[i], listaPesos[i]);
            }

            std::sort(vizinhosComPesos.begin(), vizinhosComPesos.end()); // Ordena por ID do vizinho

            // Reescreve os vetores ordenados
            for (int i = 0; i < vizinhosComPesos.size(); ++i) {
                listaVizinhos[inicio + i] = vizinhosComPesos[i].first;
                listaPesos[inicio + i] = vizinhosComPesos[i].second;
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
 * @brief Obtém o grau de um vértice (O(1)). Para grafos direcionados, equivale
 * ao grau de saída (out-degree).
 */
int VetorAdjacenciaPesada::obterGrau(int vertice) const {
    if (vertice > 0 && vertice <= numeroDeVertices) {
        // A diferença entre os ponteiros de início dá o número de vizinhos
        return ponteirosInicio[vertice + 1] - ponteirosInicio[vertice];
    }
    throw std::out_of_range("Vertice invalido ao obter grau.");
}

/**
 * @brief Obtém apenas os IDs dos vizinhos (para compatibilidade com Representacao).
 */
std::vector<int> VetorAdjacenciaPesada::obterVizinhos(int vertice) const {
    if (vertice <= 0 || vertice > numeroDeVertices) {
        throw std::out_of_range("Vertice invalido ao obter vizinhos.");
    }
    // Copia a fatia correspondente do vetor listaVizinhos
    auto inicio = listaVizinhos.begin() + ponteirosInicio[vertice];
    auto fim = listaVizinhos.begin() + ponteirosInicio[vertice + 1];
    return std::vector<int>(inicio, fim);
}

/**
 * @brief Obtém os vizinhos e os pesos das arestas correspondentes.
 */
std::vector<VizinhoComPeso> VetorAdjacenciaPesada::obterVizinhosComPesos(int vertice) const {
    if (vertice <= 0 || vertice > numeroDeVertices) {
        throw std::out_of_range("Vertice invalido ao obter vizinhos com pesos.");
    }
    std::vector<VizinhoComPeso> resultado;
    int inicio = ponteirosInicio[vertice];
    int fim = ponteirosInicio[vertice + 1];
    resultado.reserve(fim - inicio); // Pre-aloca memória

    for (int i = inicio; i < fim; ++i) {
        resultado.push_back({ listaVizinhos[i], listaPesos[i] });
    }
    return resultado;
}

/**
 * @brief Itera sobre vizinhos e pesos usando uma função lambda.
 */
void VetorAdjacenciaPesada::paraCadaVizinhoComPeso(int u, const std::function<void(int v, double w)>& fn) const {
    if (u <= 0 || u > numeroDeVertices) {
        throw std::out_of_range("Vertice invalido em paraCadaVizinhoComPeso.");
    }
    int inicio = ponteirosInicio[u];
    int fim = ponteirosInicio[u + 1];
    for (int i = inicio; i < fim; ++i) {
        fn(listaVizinhos[i], listaPesos[i]); // Chama a função lambda passada
    }
}


/**
 * @brief Obtém o peso da aresta entre u e v (O(grau(u)) se ordenado, O(grau(u)) pior caso se não).
 */
double VetorAdjacenciaPesada::obterPesoAresta(int u, int v) const {
    if (u <= 0 || u > numeroDeVertices || v <= 0 || v > numeroDeVertices) {
        throw std::out_of_range("Vertices invalidos ao obter peso da aresta.");
    }
    if (u == v) {
        return 0.0;
    }

    int inicio = ponteirosInicio[u];
    int fim = ponteirosInicio[u + 1];

    // Busca linear (ou binária se os vizinhos estiverem ordenados)
    for (int i = inicio; i < fim; ++i) {
        if (listaVizinhos[i] == v) {
            return listaPesos[i];
        }
    }

    // Se não encontrou, a aresta não existe
    return std::numeric_limits<double>::infinity();
}