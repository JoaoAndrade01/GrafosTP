#pragma once
/**
 * @file MatrizAdjacencia.h
 * @brief Defini��o da classe MatrizAdjacencia, uma implementa��o de grafo
 * usando matriz de adjac�ncia.
 */

#include "Representacao.h" // Inclui a interface que estamos implementando
#include <vector>

 /**
  * @class MatrizAdjacencia
  * @brief Implementa��o concreta da representa��o de um grafo usando uma matriz de adjac�ncia.
  * @details Herda de Representacao e implementa seus m�todos virtuais puros.
  * A matriz armazena 'true' se uma aresta existe entre dois v�rtices e 'false' caso contr�rio.
  */
class MatrizAdjacencia : public Representacao {
public:
    /**
     * @brief Construtor da classe MatrizAdjacencia.
     * @param numeroDeVertices O n�mero total de v�rtices que o grafo ter�.
     */
    explicit MatrizAdjacencia(int numeroDeVertices);

    /**
     * @brief Destrutor padr�o.
     */
    ~MatrizAdjacencia() override = default;

    // Bloco: Implementa��o do "contrato" da interface Representacao
    void adicionarAresta(int u, int v) override;
    int obterGrau(int vertice) const override;
    std::vector<int> obterVizinhos(int vertice) const override;

private:
    // A matriz de adjac�ncia. Usamos bool para economizar espa�o.
    // O tamanho ser� (numeroDeVertices + 1) x (numeroDeVertices + 1) para suportar indexa��o a partir de 1.
    std::vector<std::vector<bool>> matriz;

    // Armazena o n�mero de v�rtices para evitar o uso de matriz.size() repetidamente,
    // e para clareza no c�digo.
    int numeroDeVertices;
};