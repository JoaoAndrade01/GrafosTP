#pragma once
#include <memory>
#include <string>

class Representacao;

std::unique_ptr<Representacao>
carregarGrafoPesadoAuto(const std::string& caminho, bool* temPesoNeg);

