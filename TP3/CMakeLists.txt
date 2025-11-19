cmake_minimum_required(VERSION 3.16)
project(TP3 LANGUAGES CXX)

# Habilita Edit and Continue no MSVC (se estiver no Windows/Visual Studio)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build type padrão: Release (pode mudar pra Debug no preset/CLI)
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

# Otimizações extras só para GCC/Clang em Release (não mexe no MSVC)

if (CMAKE_BUILD_TYPE STREQUAL "Release" AND NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  message(STATUS "Aplicando otimizações GCC/Clang para Release build.")
  add_compile_options(-O3 -march=native -ffast-math)
  add_link_options(-flto)
endif()

# Includes da biblioteca
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/biblioteca
  ${CMAKE_CURRENT_SOURCE_DIR}/biblioteca/interface
  ${CMAKE_CURRENT_SOURCE_DIR}/biblioteca/representacao
  ${CMAKE_CURRENT_SOURCE_DIR}/biblioteca/algoritmos
)

# Fontes (usando GLOB_RECURSE para TP3)
file(GLOB_RECURSE TP3_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/biblioteca/**/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/estudos/*.cpp"
)

add_executable(TP3 ${TP3_SOURCES})

