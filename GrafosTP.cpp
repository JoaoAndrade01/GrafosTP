#include <iostream>
#include <string>
#include <chrono> // Para medir o tempo de execução
#include "IGraph.h"
#include "GraphFactory.h"

// Função auxiliar para executar e medir o tempo
void processGraph(const std::string& inputFile, GraphRepresentation type) {
        std::string typeStr;
        switch (type) {
        case GraphRepresentation::ADJACENCY_LIST:   typeStr = "Lista";  break;
        case GraphRepresentation::ADJACENCY_MATRIX: typeStr = "Matriz"; break;
        case GraphRepresentation::ADJACENCY_VECTOR: typeStr = "Vector"; break;
        }

    std::cout << "Processando com representacao de " << typeStr << "..." << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    auto graph = GraphFactory::createGraphFromFile(inputFile, type);

    if (!graph) {
        std::cerr << "Falha ao criar o grafo com representacao de " << typeStr << "." << std::endl;
        return;
    }

    std::string outputFile = "relatorio_" + typeStr + ".txt";
    graph->generateReport(outputFile);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Relatorio gerado em '" << outputFile << "'." << std::endl;
    std::cout << "Tempo total: " << duration.count() << " segundos." << std::endl << std::endl;
}

int main() {
    std::string filename = "grafo_2.txt"; // <-- COLOQUE O NOME DO SEU ARQUIVO AQUI

    std::cout << "Iniciando analise do arquivo: " << filename << std::endl << std::endl;

    // Processa usando Lista de Adjacência
   // processGraph(filename, GraphRepresentation::ADJACENCY_LIST);

    // Processa usando Vetor (CSR)
    processGraph(filename, GraphRepresentation::ADJACENCY_VECTOR); //performa melhor na prática por ser contíguo em memória

     // Processa usando Matriz de Adjacência
    // AVISO: A linha abaixo pode consumir muita memória e ser extremamente lenta!
    // processGraph(filename, GraphRepresentation::ADJACENCY_MATRIX);

    std::string confirmacao;
    std::cout << "Digite 'sair' para encerrar o programa: ";
    while (true) {
        std::getline(std::cin, confirmacao);
        if (confirmacao == "sair") {
            break;
        }
        std::cout << "Comando invalido. Digite 'sair' para encerrar o programa: ";
        return 0;   
    }

}
