# CMakeList.txt: projeto de CMake para TP1, incluir origem e definir
# lógica específica a um projeto aqui.
#
cmake_minimum_required (VERSION 3.8)

# Habilite a Recarga Dinâmica para compiladores MSVC, se houver suporte.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("TP1")

# Adicione a origem ao executável deste projeto.
add_executable (TP1   "biblioteca/interface/Grafo.h" "biblioteca/representacao/representacao.h" "biblioteca/representacao/MatrizAdjacencia.h" "biblioteca/representacao/MatrizAdjacencia.cpp" "biblioteca/interface/Grafo.cpp" "biblioteca/representacao/ListaAdjacencia.h" "biblioteca/representacao/ListaAdjacencia.cpp" "biblioteca/representacao/MatrizAdjacenciaTriangular.h" "biblioteca/representacao/MatrizAdjacenciaTriangular.cpp" "biblioteca/representacao/ListaAdjacenciaSimples.h" "biblioteca/representacao/ListaAdjacenciaSimples.cpp" "biblioteca/algoritmos/BFS.cpp" "biblioteca/algoritmos/DFS.cpp" "biblioteca/algoritmos/DFS.h" "estudos/2_BFS_DFS/estudo_caso_2_3.cpp" "estudos/estudos.h" "estudos/1_comparacao_memoria/estudo_caso_1.cpp" "estudos/3_Outros/estudo_caso_final.cpp""estudos/main.cpp" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TP1 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Adicione testes e instale destinos, se necessário.
