cmake_minimum_required(VERSION 3.29)
project(SS)

set(CMAKE_CXX_STANDARD 17)

add_executable(SS
        src/assembler_section.cpp
        inc/assembler_section.hpp
        src/assembler_symtab.cpp
        inc/assembler_symtab.hpp
        src/assembler.cpp
        inc/linker_section.hpp
        inc/linker_section.hpp
        src/linker_section.cpp
        inc/linker_symtab.hpp
        src/linker_symtab.cpp
        src/linker.cpp)
