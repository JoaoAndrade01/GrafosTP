#pragma once
#include "IGraph.h"
#include <string>
#include <memory> // Para std::unique_ptr

class GraphFactory {
public:
    /**
     * @brief Carrega um grafo de um arquivo.
     * @param filePath O caminho para o arquivo de texto.
     * @param type A representação de grafo desejada (lista ou matriz).
     * @return Um ponteiro inteligente para o objeto de grafo criado.
    */
    static std::unique_ptr<IGraph> createGraphFromFile(
        const std::string& filePath,
        GraphRepresentation type
    );
};