cmake_minimum_required(VERSION 3.22)
project(rat C)

set(CMAKE_C_STANDARD 23)

add_executable(rat
        src/main.c
        src/def/options.h src/def/program.h src/def/style.h src/def/style.c
        src/def/alloc.c src/def/alloc.h src/def/term.c src/def/term.h
        src/print_file.c src/print_file.h
        src/print_dir.c src/print_dir.h
        src/print_help.c src/print_help.h
        src/string_manipulation.c src/string_manipulation.h)

install(TARGETS rat DESTINATION /usr/local/bin)
