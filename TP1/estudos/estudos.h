#pragma once

/**
 * @file estudos_de_caso.h
 * @brief Declara as funções principais para cada estudo de caso, permitindo
 * que sejam chamadas pelo programa principal (main.cpp).
 */
#include <filesystem>
#include <string>

 // Função utilitária para obter o caminho absoluto de um arquivo dentro de estudos/grafos_em_txt
inline std::string caminhoEstudos(const std::string& nomeArquivo) {
    namespace fs = std::filesystem;

    // __FILE__ = caminho absoluto deste header (estudos/estudos.h)
    fs::path base(__FILE__);

    // sobe um diretório: estudos/estudos.h -> estudos/
    base = base.parent_path();

    // Agora adiciona grafos_em_txt
    fs::path dirEstudos = base / "grafos_em_txt";

    // Retorna o caminho completo do arquivo
    return (dirEstudos / nomeArquivo).string();
}


 // Declaração da função do Estudo de Caso 1 (Memória)
void executarEstudoDeCasoMemoria();

// Declaração da função dos Estudos de Caso 2 & 3 (Tempo)
void executarEstudoDeCasoTempo();

// Declaração da função do Estudo de Caso Final (Perguntas Analíticas)
void executarEstudoDeCasoFinal();