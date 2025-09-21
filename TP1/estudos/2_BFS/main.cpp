#include <iostream>
#include "../../biblioteca/interface/Grafo.h"

int main() {
    try {
        // Carrega um grafo conhecido
        std::string arquivoGrafo = "C:/Users/João - Dynatest/source/repos/GrafosTP/TP1/estudos/grafos_em_txt/grafo_1.txt";
        Grafo grafo(arquivoGrafo, TipoRepresentacao::LISTA_ADJACENCIA);

        int u = 1;
        int v = 3;

        std::cout << "Calculando a distancia entre " << u << " e " << v << "..." << std::endl;

        // Chama o novo método
        int dist = grafo.calcularDistancia(u, v);

        if (dist != -1) {
            std::cout << "A distancia e: " << dist << std::endl;
        } else {
            std::cout << "Nao ha caminho entre os vertices." << std::endl;
        }

        // Para o grafo_1.txt, a distancia de 1 a 3 é 2 (caminho 1 -> 5 -> 3).
        // A saída esperada é "A distancia e: 2".

    } catch (const std::exception& e) {
        std::cerr << "ERRO: " << e.what() << std::endl;
    }
    return 0;
}