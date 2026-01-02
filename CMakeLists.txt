cmake_minimum_required(VERSION 3.13)
project(Minique VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    src/lexer.cpp
    src/parser.cpp
    src/codegen.cpp
)


if(NOT EMSCRIPTEN)
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

    # Native executable with LLVM
    add_executable(minique
        src/main.cpp
        ${SOURCES}
    )

    target_link_libraries(minique ${LLVM_LDFLAGS} ${LLVM_LIBS} ${LLVM_SYSTEM_LIBS})

    install(TARGETS minique DESTINATION bin)


else()
    # WASM version uses simple interpreter (no LLVM dependency)
    add_executable(minique_wasm
        src/wasm_interpreter.cpp
    )

    # WASM-specific settings
    set_target_properties(minique_wasm PROPERTIES
        SUFFIX ".js"
        PREFIX ""
    )

    # Emscripten linker flags
    target_link_options(minique_wasm PRIVATE
        -s WASM=1
        -s EXPORTED_FUNCTIONS='["_compile_minique"]'
        -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap","UTF8ToString","allocateUTF8"]'
        -s ALLOW_MEMORY_GROWTH=1
        -s MODULARIZE=1
        -s EXPORT_NAME='MiniqueModule'
        -O3
    )

    # Output to web directory
    set_target_properties(minique_wasm PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/docs/wasm"
    )
endif()
