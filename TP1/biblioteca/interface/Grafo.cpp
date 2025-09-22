/**
 * @file Grafo.cpp
 * @brief Implementa��o dos m�todos da classe Grafo.
 */

#include "Grafo.h" // Inclui a defini��o da pr�pria classe
#include "../representacao/MatrizAdjacencia.h"
#include "../representacao/MatrizAdjacenciaTriangular.h"
#include "../representacao/ListaAdjacencia.h"
#include "../representacao/ListaAdjacenciaSimples.h"
 // #include "../representacao/ListaAdjacencia.h" // Ser� inclu�do no futuro
#include <fstream>   // Para leitura de arquivos (ifstream)
#include <stdexcept> // Para lan�ar exce��es (runtime_error)
#include <iostream>  // Para mensagens de depura��o (opcional)
#include <vector>
#include <random>
#include <ostream>

 /**
 * @brief Construtor da classe Grafo.
 */
Grafo::Grafo(const std::string& caminhoArquivo, TipoRepresentacao tipo) {
    // Bloco: Abertura e valida��o do arquivo de entrada
    std::ifstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        // Lan�a uma exce��o se o arquivo n�o puder ser aberto.
        // O programa de teste (main.cpp) pode capturar esta exce��o.
        throw std::runtime_error("Nao foi possivel abrir o arquivo: " + caminhoArquivo);
    }

    // Bloco: Leitura do n�mero de v�rtices
    arquivo >> this->numeroDeVertices;
    this->numeroDeArestas = 0; // Inicializa o contador de arestas

    // Bloco: Cria��o da representa��o interna com base na escolha do usu�rio
    // Este � o ponto central que usa o padr�o de design "Strategy".
    switch (tipo) {
    case TipoRepresentacao::MATRIZ_ADJACENCIA:
        // Cria um objeto MatrizAdjacencia e o move para o ponteiro inteligente.
        representacaoInterna = std::make_unique<MatrizAdjacencia>(this->numeroDeVertices);
        break;
    case TipoRepresentacao::LISTA_ADJACENCIA:
        // Lan�a um erro claro se a funcionalidade ainda n�o foi implementada.
        representacaoInterna = std::make_unique<ListaAdjacencia>(this->numeroDeVertices);
        break;
    case TipoRepresentacao::MATRIZ_ADJACENCIA_TRIANGULAR:
        representacaoInterna = std::make_unique<MatrizAdjacenciaTriangular>(this->numeroDeVertices);
        break;
    case TipoRepresentacao::LISTA_ADJACENCIA_SIMPLES: // <-- Adicionar este novo caso
        representacaoInterna = std::make_unique<ListaAdjacenciaSimples>(this->numeroDeVertices);
        break;
    default:
        throw std::invalid_argument("Tipo de representacao desconhecido.");
    }

    // Bloco: Leitura das arestas do arquivo
    int u, v;
    // O loop continua enquanto for poss�vel ler dois inteiros da linha.
    while (arquivo >> u >> v) {
        // Delega a adi��o da aresta para o objeto de representa��o.
        // A classe Grafo n�o sabe como isso � feito, apenas manda a ordem.
        representacaoInterna->adicionarAresta(u, v);
        this->numeroDeArestas++; // Incrementa o contador de arestas
    }

    // Ap�s ler todas as arestas, chamamos o m�todo para construir a estrutura final.
    // Para a MatrizAdjacencia, isso n�o far� nada. Para a ListaAdjacencia, far� tudo.
    representacaoInterna->finalizarConstrucao();

    // O n�mero de arestas � definido internamente pela representa��o,
    // ent�o podemos busc�-lo ap�s a finaliza��o (ou ler do buffer)
    // Para simplicidade, vamos recalcular.
    arquivo.clear();
    arquivo.seekg(0);
    std::string linha;
    std::getline(arquivo, linha); // Pula a primeira linha
    this->numeroDeArestas = 0;
    while (std::getline(arquivo, linha)) this->numeroDeArestas++;

    // O arquivo � fechado automaticamente quando 'arquivo' sai de escopo.
}

/**
 * @brief Destrutor da classe Grafo.
 * @details O compilador gerar� um destrutor padr�o que chama o destrutor de
 * std::unique_ptr, que por sua vez deleta o objeto de representa��o.
 * A defini��o expl�cita aqui, mesmo que vazia, � necess�ria por causa do uso
 * de forward declaration e std::unique_ptr com tipos incompletos no header.
 */
Grafo::~Grafo() = default;

// Implementa��es dos outros m�todos (obterNumeroVertices, etc.)
int Grafo::obterNumeroVertices() const {
    return this->numeroDeVertices;
}

int Grafo::obterNumeroArestas() const {
    return this->numeroDeArestas;
}

int Grafo::obterGrau(int vertice) const {
    // Delega a chamada para o objeto de representa��o
    return representacaoInterna->obterGrau(vertice);
}

std::vector<int> Grafo::obterVizinhos(int vertice) const {
    // Delega a chamada para o objeto de representa��o
    return representacaoInterna->obterVizinhos(vertice);
}

/**
 * @brief Executa a Busca em Largura (BFS) a partir de um v�rtice de origem.
 * @details Este m�todo atua como uma fachada (Facade). Ele instancia a classe
 * especialista em BFS e delega a execu��o para ela, passando uma refer�ncia
 * a si mesmo (*this).
 */
ResultadoBFS Grafo::executarBFS(int verticeOrigem) const {
    // 1. Cria uma inst�ncia do nosso "trabalhador" de BFS.
    BFS algoritmoBFS;

    // 2. Chama o m�todo 'executar' do trabalhador, passando o pr�prio
    // objeto Grafo atual (*this) como par�metro, e retorna o resultado.
    return algoritmoBFS.executar(*this, verticeOrigem);
}

/**
 * @brief Executa a Busca em Profundidade (DFS) a partir de um v�rtice de origem.
 * @details Atua como uma fachada, delegando a execu��o para a classe especialista 'DFS'.
 */
ResultadoDFS Grafo::executarDFS(int verticeOrigem) const {
    DFS algoritmoDFS;
    return algoritmoDFS.executar(*this, verticeOrigem);
}

/**
 * @brief Salva a �rvore de busca (pai e n�vel de cada v�rtice) em um arquivo,
 * ORDENADA POR N�VEL.
 * @details Coleta os dados de pai e n�vel para todos os v�rtices, ordena esses
 * dados com base no n�vel (e depois por ID, como crit�rio de desempate), e
 * ent�o escreve o resultado ordenado no arquivo.
 */
void Grafo::salvarArvoreBusca(const ResultadoBFS& resultado, const std::string& caminhoArquivo) const {
    // Bloco 1: Abertura e verifica��o do arquivo
    std::ofstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo de saida: " + caminhoArquivo);
    }

    // Bloco 2: Coleta dos dados em uma estrutura tempor�ria
    // Isso � necess�rio para que possamos ordenar antes de escrever.
    struct InfoVertice {
        int id;
        int pai;
        int nivel;
    };
    std::vector<InfoVertice> verticesParaOrdenar;
    // Percorre todos os v�rtices do grafo
    for (int v = 1; v <= this->numeroDeVertices; ++v) {
        // Adiciona � lista apenas os v�rtices que foram alcan�ados pela busca
        if (resultado.nivel[v] != -1) {
            verticesParaOrdenar.push_back({ v, resultado.pai[v], resultado.nivel[v] });
        }
    }

    // Bloco 3: Ordena��o do vetor tempor�rio
    // Usamos std::sort com uma express�o lambda para definir nossa regra de ordena��o.
    std::sort(verticesParaOrdenar.begin(), verticesParaOrdenar.end(),
        [](const InfoVertice& a, const InfoVertice& b) {
            // Regra 1: O crit�rio principal � o n�vel.
            if (a.nivel != b.nivel) {
                return a.nivel < b.nivel; // Ordena por n�vel ascendente.
            }
            // Regra 2: Se os n�veis forem iguais, usamos o ID do v�rtice como desempate.
            return a.id < b.id;
        }
    );

    // Bloco 4: Escrita dos dados j� ordenados no arquivo
    arquivo << "Vertice,Pai,Nivel\n"; // Escreve o cabe�alho
    for (const auto& info : verticesParaOrdenar) {
        arquivo << info.id << "," << info.pai << "," << info.nivel << "\n";
    }
}

void Grafo::salvarArvoreBusca(const ResultadoDFS& resultado, const std::string& caminhoArquivo) const {
    // Bloco 1: Abertura e verifica��o do arquivo
    std::ofstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo de saida: " + caminhoArquivo);
    }

    // Bloco 2: Coleta dos dados em uma estrutura tempor�ria
    // Isso � necess�rio para que possamos ordenar antes de escrever.
    struct InfoVertice {
        int id;
        int pai;
        int nivel;
    };
    std::vector<InfoVertice> verticesParaOrdenar;
    // Percorre todos os v�rtices do grafo
    for (int v = 1; v <= this->numeroDeVertices; ++v) {
        // Adiciona � lista apenas os v�rtices que foram alcan�ados pela busca
        if (resultado.nivel[v] != -1) {
            verticesParaOrdenar.push_back({ v, resultado.pai[v], resultado.nivel[v] });
        }
    }

    // Bloco 3: Ordena��o do vetor tempor�rio
    // Usamos std::sort com uma express�o lambda para definir nossa regra de ordena��o.
    std::sort(verticesParaOrdenar.begin(), verticesParaOrdenar.end(),
        [](const InfoVertice& a, const InfoVertice& b) {
            // Regra 1: O crit�rio principal � o n�vel.
            if (a.nivel != b.nivel) {
                return a.nivel < b.nivel; // Ordena por n�vel ascendente.
            }
            // Regra 2: Se os n�veis forem iguais, usamos o ID do v�rtice como desempate.
            return a.id < b.id;
        }
    );

    // Bloco 4: Escrita dos dados j� ordenados no arquivo
    arquivo << "Vertice,Pai,Nivel\n"; // Escreve o cabe�alho
    for (const auto& info : verticesParaOrdenar) {
        arquivo << info.id << "," << info.pai << "," << info.nivel << "\n";
    }
}

/**
 * @brief Implementa��o do c�lculo de dist�ncia.
 */
int Grafo::calcularDistancia(int verticeU, int verticeV) const {
    // Bloco 1: Valida��o dos v�rtices (boa pr�tica)
    if (verticeU <= 0 || verticeU > this->numeroDeVertices || verticeV <= 0 || verticeV > this->numeroDeVertices) {
        throw std::out_of_range("V�rtice(s) inv�lido(s) fornecido(s) para calcularDistancia.");
    }

    // A dist�ncia de um v�rtice para ele mesmo � 0.
    if (verticeU == verticeV) {
        return 0;
    }

    // Bloco 2: Execu��o do BFS a partir da origem 'u'
    // Delegamos todo o trabalho pesado para o m�todo que j� temos.
    ResultadoBFS resultadoBFS = this->executarBFS(verticeU);

    // Bloco 3: Retorno do resultado
    // A propriedade fundamental do BFS � que o vetor 'nivel' cont�m a menor
    // dist�ncia da origem para todos os outros v�rtices.
    // Se o v�rtice 'v' for inalcan��vel, seu n�vel j� ser� -1, que � o
    // valor correto a ser retornado.
    return resultadoBFS.nivel[verticeV];
}

/**
 * @brief Implementa��o do c�lculo de di�metro EXATO.
 */
int Grafo::calcularDiametro() const {
    int diametroGeral = 0;

    // Itera por cada v�rtice, tratando-o como uma poss�vel origem de um BFS.
    for (int v_origem = 1; v_origem <= this->numeroDeVertices; ++v_origem) {
        // Roda um BFS completo a partir do v�rtice atual.
        ResultadoBFS resultado = this->executarBFS(v_origem);
        int excentricidadeLocal = 0;

        // Encontra a maior dist�ncia (excentricidade) a partir de v_origem.
        for (int v_destino = 1; v_destino <= this->numeroDeVertices; ++v_destino) {
            // Se encontrarmos um v�rtice inalcan��vel (-1), o grafo � desconexo.
            // O di�metro de um grafo desconexo � considerado infinito. Retornamos -1.
            if (resultado.nivel[v_destino] == -1) {
                return -1;
            }
            excentricidadeLocal = std::max(excentricidadeLocal, resultado.nivel[v_destino]);
        }
        // Atualiza o di�metro geral com a maior excentricidade encontrada at� agora.
        diametroGeral = std::max(diametroGeral, excentricidadeLocal);
    }
    return diametroGeral;
}

/**
 * @brief Implementa��o do c�lculo de di�metro APROXIMADO.
 */
int Grafo::calcularDiametroAproximado(int iteracoes) const {
    int diametroAproximado = 0;
    if (this->numeroDeVertices == 0) return 0;

    // Configura o gerador de n�meros aleat�rios.
    std::random_device rd;
    // Substitua std::mt19337 por std::mt19937 (correto para o gerador Mersenne Twister)
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, this->numeroDeVertices);

    // Repete a heur�stica 'iteracoes' vezes para aumentar a qualidade da aproxima��o.
    for (int i = 0; i < iteracoes; ++i) {
        // Passo 1: Escolha um v�rtice de partida aleat�rio 's'.
        int s = distrib(gen);

        // Passo 2: Rode um BFS de 's' e encontre o v�rtice 'u' mais distante.
        ResultadoBFS res1 = this->executarBFS(s);
        int maxNivel1 = -1;
        int u = s;
        for (int v = 1; v <= this->numeroDeVertices; ++v) {
            if (res1.nivel[v] > maxNivel1) {
                maxNivel1 = res1.nivel[v];
                u = v;
            }
        }

        // Passo 3: Rode um BFS de 'u' e encontre a maior dist�ncia a partir dele.
        ResultadoBFS res2 = this->executarBFS(u);
        int maxNivel2 = -1;
        for (int v = 1; v <= this->numeroDeVertices; ++v) {
            if (res2.nivel[v] > maxNivel2) {
                maxNivel2 = res2.nivel[v];
            }
        }

        // Passo 4: A maior dist�ncia encontrada (maxNivel2) � uma aproxima��o.
        // Atualizamos nosso m�ximo com o resultado desta itera��o.
        diametroAproximado = std::max(diametroAproximado, maxNivel2);
    }

    return diametroAproximado;
}

/**
 * @brief Implementa��o do algoritmo para encontrar componentes conexas,
 * REUTILIZANDO o m�todo executarBFS.
 */
std::vector<ComponenteConexa> Grafo::encontrarComponentesConexas() const {
    // Bloco 1: Prepara��o
    std::vector<ComponenteConexa> todasAsComponentes;
    std::vector<bool> visitado(this->numeroDeVertices + 1, false);

    // Bloco 2: Loop principal para encontrar os pontos de partida das componentes
    // Itera por todos os v�rtices do grafo para garantir que nenhuma componente seja esquecida.
    for (int v_inicial = 1; v_inicial <= this->numeroDeVertices; ++v_inicial) {

        // Se o v�rtice atual ainda n�o foi visitado, ele pertence a uma nova componente.
        if (!visitado[v_inicial]) {

            // Bloco 3: REUTILIZA��O do BFS que j� implementamos!
            // Em vez de reescrever a l�gica da fila aqui, simplesmente chamamos o m�todo principal.
            // O resultado conter� todos os v�rtices alcan��veis a partir de 'v_inicial'.
            ResultadoBFS resultadoParcial = this->executarBFS(v_inicial);

            // Bloco 4: Processamento do resultado do BFS
            // Agora, iteramos pelos resultados para construir a componente
            // e marcar todos os seus membros como visitados.
            ComponenteConexa novaComponente;
            for (int v_componente = 1; v_componente <= this->numeroDeVertices; ++v_componente) {
                // Se o v�rtice 'v_componente' foi alcan�ado nesta busca (tem um pai ou � a pr�pria origem)...
                if (resultadoParcial.pai[v_componente] != -1 || v_componente == v_inicial) {
                    novaComponente.vertices.push_back(v_componente);
                    // CRUCIAL: Marcamos o v�rtice como visitado no controle do loop principal
                    // para n�o iniciarmos uma nova busca para um v�rtice que j� pertence a esta componente.
                    visitado[v_componente] = true;
                }
            }

            // Adiciona a componente completa � lista de resultados.
            if (!novaComponente.vertices.empty()) {
                novaComponente.tamanho = novaComponente.vertices.size();
                todasAsComponentes.push_back(novaComponente);
            }
        }
    }

    // Bloco 5: Ordena��o das componentes (l�gica inalterada)
    std::sort(todasAsComponentes.begin(), todasAsComponentes.end(),
        [](const ComponenteConexa& a, const ComponenteConexa& b) {
            return a.tamanho > b.tamanho;
        }
    );

    return todasAsComponentes;
}

/**
 * @brief Implementa��o PRINCIPAL que escreve o relat�rio em um fluxo gen�rico.
 */
void Grafo::gerarRelatorioCompleto(std::ostream& saida) const {
    saida << "--- RELATORIO COMPLETO DO GRAFO ---\n\n";

    // --- Bloco 1: Estat�sticas B�sicas ---
    saida << "1. ESTATISTICAS BASICAS\n";
    saida << "   - Numero de Vertices: " << this->numeroDeVertices << "\n";
    saida << "   - Numero de Arestas: " << this->numeroDeArestas << "\n\n";

    // --- Bloco 2: Estat�sticas de Grau ---
    EstatisticasGrau estatisticas = this->calcularEstatisticasGrau();
    saida << "2. ESTATISTICAS DE GRAU\n";
    saida << "   - Grau Minimo: " << estatisticas.grauMin << "\n";
    saida << "   - Grau Maximo: " << estatisticas.grauMax << "\n";
    saida << "   - Grau Medio: " << estatisticas.grauMedio << "\n";
    saida << "   - Mediana de Grau: " << estatisticas.grauMediana << "\n\n";

    // --- Bloco 3: Componentes Conexas ---
    std::vector<ComponenteConexa> componentes = this->encontrarComponentesConexas();
    saida << "3. COMPONENTES CONEXAS\n";
    saida << "   - Numero total de componentes: " << componentes.size() << "\n";

    for (size_t i = 0; i < componentes.size(); ++i) {
        saida << "   - Componente " << i + 1 << " (Tamanho: " << componentes[i].tamanho << " vertices)\n";
    }

    saida << "\n--- FIM DO RELATORIO ---" << std::endl;
}

/**
 * @brief Implementa��o de conveni�ncia que salva o relat�rio em um arquivo.
 * @details Abre um arquivo e chama a implementa��o principal passando o arquivo como fluxo.
 */
void Grafo::gerarRelatorioCompleto(const std::string& caminhoArquivo) const {
    std::ofstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Nao foi possivel criar o arquivo de relatorio: " + caminhoArquivo);
    }
    // Chama a outra vers�o da fun��o, passando o arquivo como o fluxo de sa�da
    this->gerarRelatorioCompleto(arquivo);
}

// Nota: Precisamos garantir que o m�todo calcularEstatisticasGrau exista em Grafo.cpp
// Se ele ainda n�o foi movido para c�, aqui est� a implementa��o dele:
EstatisticasGrau Grafo::calcularEstatisticasGrau() const {
    EstatisticasGrau est;
    if (this->numeroDeVertices == 0) return est;

    std::vector<int> graus;
    graus.reserve(this->numeroDeVertices);
    long long somaGraus = 0;

    for (int v = 1; v <= this->numeroDeVertices; ++v) {
        int grau = this->obterGrau(v);
        graus.push_back(grau);
        somaGraus += grau;
    }

    std::sort(graus.begin(), graus.end());
    est.grauMin = graus.front();
    est.grauMax = graus.back();
    est.grauMedio = static_cast<double>(somaGraus) / this->numeroDeVertices;

    if (this->numeroDeVertices % 2 == 1) {
        est.grauMediana = graus[this->numeroDeVertices / 2];
    }
    else {
        est.grauMediana = (graus[this->numeroDeVertices / 2 - 1] + graus[this->numeroDeVertices / 2]) / 2.0;
    }

    return est;
}