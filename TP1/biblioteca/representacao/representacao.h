#pragma once
/**
 * @file Representacao.h
 * @brief Defini��o da interface base abstrata para todas as representa��es de grafo.
 * @details Esta classe define um "contrato" que as classes concretas de representa��o
 * (como MatrizAdjacencia e ListaAdjacencia) devem seguir.
 */

#include <vector>

 /**
  * @class Representacao
  * @brief Classe base abstrata para as representa��es de grafo.
  * @details Funciona como uma interface que define as opera��es essenciais que
  * a classe Grafo espera que qualquer representa��o interna possa executar.
  */
class Representacao {
public:
    /**
     * @brief Destrutor virtual padr�o.
     * @details Essencial para garantir que a mem�ria de classes derivadas seja
     * liberada corretamente quando um objeto � deletado atrav�s de um ponteiro da classe base.
     */
    virtual ~Representacao() = default;

    /**
     * @brief Adiciona uma aresta ao grafo. (M�todo virtual puro)
     * @param u V�rtice de origem da aresta (indexado em 1).
     * @param v V�rtice de destino da aresta (indexado em 1).
     */
    virtual void adicionarAresta(int u, int v) = 0;

    /**
     * @brief Obt�m o grau de um v�rtice espec�fico. (M�todo virtual puro)
     * @param vertice O v�rtice (indexado em 1) cujo grau � desejado.
     * @return O grau do v�rtice.
     */
    virtual int obterGrau(int vertice) const = 0;

    /**
     * @brief Obt�m uma lista com os vizinhos de um v�rtice. (M�todo virtual puro)
     * @param vertice O v�rtice (indexado em 1) do qual se deseja obter os vizinhos.
     * @return Um std::vector<int> contendo os �ndices dos v�rtices vizinhos.
     */
    virtual std::vector<int> obterVizinhos(int vertice) const = 0;
    /**
     * @brief Finaliza a constru��o da estrutura de dados interna, se necess�rio.
     * @details Algumas representa��es (como CSR) precisam de um passo final
     * ap�s todas as arestas serem adicionadas. Esta fun��o serve para isso.
     * A implementa��o padr�o � vazia, n�o fazendo nada.
     */
    virtual void finalizarConstrucao() { /* Vazio por padr�o */ }
};