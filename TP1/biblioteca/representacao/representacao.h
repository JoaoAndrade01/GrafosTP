#pragma once
/**
 * @file Representacao.h
 * @brief Definição da interface base abstrata para todas as representações de grafo.
 * @details Esta classe define um "contrato" que as classes concretas de representação
 * (como MatrizAdjacencia e ListaAdjacencia) devem seguir.
 */

#include <vector>

 /**
  * @class Representacao
  * @brief Classe base abstrata para as representações de grafo.
  * @details Funciona como uma interface que define as operações essenciais que
  * a classe Grafo espera que qualquer representação interna possa executar.
  */
class Representacao {
public:
    /**
     * @brief Destrutor virtual padrão.
     * @details Essencial para garantir que a memória de classes derivadas seja
     * liberada corretamente quando um objeto é deletado através de um ponteiro da classe base.
     */
    virtual ~Representacao() = default;

    /**
     * @brief Adiciona uma aresta ao grafo. (Método virtual puro)
     * @param u Vértice de origem da aresta (indexado em 1).
     * @param v Vértice de destino da aresta (indexado em 1).
     */
    virtual void adicionarAresta(int u, int v) = 0;

    /**
     * @brief Obtém o grau de um vértice específico. (Método virtual puro)
     * @param vertice O vértice (indexado em 1) cujo grau é desejado.
     * @return O grau do vértice.
     */
    virtual int obterGrau(int vertice) const = 0;

    /**
     * @brief Obtém uma lista com os vizinhos de um vértice. (Método virtual puro)
     * @param vertice O vértice (indexado em 1) do qual se deseja obter os vizinhos.
     * @return Um std::vector<int> contendo os índices dos vértices vizinhos.
     */
    virtual std::vector<int> obterVizinhos(int vertice) const = 0;
    /**
     * @brief Finaliza a construção da estrutura de dados interna, se necessário.
     * @details Algumas representações (como CSR) precisam de um passo final
     * após todas as arestas serem adicionadas. Esta função serve para isso.
     * A implementação padrão é vazia, não fazendo nada.
     */
    virtual void finalizarConstrucao() { /* Vazio por padrão */ }
};