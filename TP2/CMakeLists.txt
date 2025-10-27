cmake_minimum_required(VERSION 3.16)
project(TP2 LANGUAGES CXX)

# Habilite a Recarga Dinâmica (Editar e Continuar) para compiladores MSVC.
# (Inspirado no seu .txt do TP1)
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Otimizações padrão
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

# Define otimizações específicas para compiladores GCC/Clang (como no WSL)
# mas pula essas flags no MSVC (Visual Studio), que usa suas próprias.
if (CMAKE_BUILD_TYPE STREQUAL "Release" AND NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  message(STATUS "Aplicando otimizações GCC/Clang para Release build.")
  add_compile_options(-O3 -march=native -ffast-math)
  add_link_options(-flto)
endif()

# Onde estão os headers
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/biblioteca
  ${CMAKE_CURRENT_SOURCE_DIR}/biblioteca/interface
  ${CMAKE_CURRENT_SOURCE_DIR}/biblioteca/representacao
  ${CMAKE_CURRENT_SOURCE_DIR}/biblioteca/algoritmos
)

# Fontes (usando GLOB_RECURSE, que é mais limpo que a lista manual do TP1)
file(GLOB_RECURSE TP2_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/biblioteca/**/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/estudos/*.cpp"
)

add_executable(TP2 ${TP2_SOURCES})