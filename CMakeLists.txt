cmake_minimum_required(VERSION 2.8.5)
project(chess-engine)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${FLAGS} -g3 -fsanitize=address,undefined")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${FLAGS} -O3")

set(CMAKE_CXX_STANDARD 20)

set(FLAGS "-Wall")
if (CMAKE_COMPILER_IS_GNUXX)
    set(FLAGS "${FLAGS} -Wextra")
endif (CMAKE_COMPILER_IS_GNUXX)

option(CMAKE_BUILD_TYPE "Build type" Release)

# =-=-=-=-=-=-= DOCS =-=-=-=-=-=-=
if (DOXYGEN_FOUND)
    message("Doxygen found. You can use `make docs`")

    add_custom_target(
        docs
        COMMAND ${DOXYGEN_EXECUTABLE}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        VERBATIM
    )
else (DOXYGEN_FOUND)
    message("Doxygen not found. You won't be able to use `make docs`")
endif (DOXYGEN_FOUND)

# =-=-=-=-=-=-= EXE =-=-=-=-=-=-=
if (UNIX)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif (UNIX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/src/board.cpp
    ${CMAKE_SOURCE_DIR}/src/utils.cpp
    ${CMAKE_SOURCE_DIR}/src/piecesquaretable.cpp
    ${CMAKE_SOURCE_DIR}/src/game.cpp
    ${CMAKE_SOURCE_DIR}/src/move.cpp
    ${CMAKE_SOURCE_DIR}/src/engine.cpp
    ${CMAKE_SOURCE_DIR}/src/movegen.cpp
    ${CMAKE_SOURCE_DIR}/src/perft.cpp
)

set(EXE_SOURCE_FILES
    ${SOURCE_FILES}
    ${CMAKE_SOURCE_DIR}/src/main.cpp
)

add_executable(chess-engine ${EXE_SOURCE_FILES})

# TESTS

set(TESTS_SOURCE_FILES
    ${SOURCE_FILES}
    ${CMAKE_SOURCE_DIR}/tests/main.cpp
)

add_executable(tests ${TESTS_SOURCE_FILES})

target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/tests)

