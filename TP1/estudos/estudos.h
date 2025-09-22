#pragma once

/**
 * @file estudos_de_caso.h
 * @brief Declara as funções principais para cada estudo de caso, permitindo
 * que sejam chamadas pelo programa principal (main.cpp).
 */
#include <filesystem>

// Função utilitária para obter o caminho absoluto de um arquivo dentro de estudos / grafos_em_txt
inline std::string caminhoEstudos(const std::string& nomeArquivo) {
    namespace fs = std::filesystem;

    // Pega o caminho absoluto do arquivo atual (estudos.h) no disco
    fs::path base(__FILE__);

    // Sobe dois diretórios: estudos.h -> estudos -> TP1
    base = base.parent_path().parent_path();

    // Agora adiciona o caminho fixo até grafos_em_txt
    fs::path dirEstudos = base / "estudos" / "grafos_em_txt";

    // Retorna o caminho completo do arquivo
    return (dirEstudos / nomeArquivo).string();
}

 // Declaração da função do Estudo de Caso 1 (Memória)
void executarEstudoDeCasoMemoria();

// Declaração da função dos Estudos de Caso 2 & 3 (Tempo)
void executarEstudoDeCasoTempo();

// Declaração da função do Estudo de Caso Final (Perguntas Analíticas)
void executarEstudoDeCasoFinal();