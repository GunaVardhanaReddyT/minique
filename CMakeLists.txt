cmake_minimum_required(VERSION 3.13)
project(Minique VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find LLVM using llvm-config
execute_process(
    COMMAND llvm-config-14 --cxxflags
    OUTPUT_VARIABLE LLVM_CXXFLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND llvm-config-14 --ldflags
    OUTPUT_VARIABLE LLVM_LDFLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND llvm-config-14 --libs core support native executionengine
    OUTPUT_VARIABLE LLVM_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND llvm-config-14 --system-libs
    OUTPUT_VARIABLE LLVM_SYSTEM_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LLVM_CXXFLAGS}")

add_executable(minique
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/codegen.cpp
)

target_link_libraries(minique ${LLVM_LDFLAGS} ${LLVM_LIBS} ${LLVM_SYSTEM_LIBS})

install(TARGETS minique DESTINATION bin)
