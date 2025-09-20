#pragma once
#include "IGraph.h"
#include <string>
#include <memory>

// Estrutura para guardar estatísticas de construção
struct BuildStats {
    double tempoConstrucaoSeg = 0.0; // apenas construção, sem leitura de arquivo
};

class GraphFactory {
public:
    /**
     * @brief Carrega um grafo de um arquivo e cria a estrutura correspondente.
     * @param filePath Caminho para o arquivo de texto (.txt).
     * @param type Tipo de representação (Lista, Matriz, Vetor).
     * @return Ponteiro inteligente para o grafo criado.
     */
    static std::unique_ptr<IGraph> createGraphFromFile(
        const std::string& filePath,
        GraphRepresentation type
    );

    /// Retorna estatísticas da última construção (tempo de construção)
    static BuildStats getLastBuildStats();

private:
    static BuildStats s_stats; // estatísticas globais da última construção
};

