cmake_minimum_required(VERSION 3.16)
project(TP2 LANGUAGES CXX)

# C++20 (pode ser 17 se preferir)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Otimizações padrão (ajuste à vontade)
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()
if (CMAKE_BUILD_TYPE STREQUAL "Release")
  add_compile_options(-O3 -march=native -ffast-math)
  add_link_options(-flto)
endif()

# onde estão os headers
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/biblioteca
  ${CMAKE_CURRENT_SOURCE_DIR}/biblioteca/interface
  ${CMAKE_CURRENT_SOURCE_DIR}/biblioteca/representacao
  ${CMAKE_CURRENT_SOURCE_DIR}/biblioteca/algoritmos
  ${CMAKE_CURRENT_SOURCE_DIR}/biblioteca/util
)

# fontes
file(GLOB_RECURSE TP2_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/biblioteca/**/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/estudos/*.cpp"
)

add_executable(TP2 ${TP2_SOURCES})
