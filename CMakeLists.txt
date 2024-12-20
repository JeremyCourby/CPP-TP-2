cmake_minimum_required(VERSION 3.28)
project(ShootEmUp LANGUAGES CXX)

# Forcer l'utilisation de C++ 23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)

set(PROJECT_FILES
        src/player.cppm
        src/projectile.cppm
        src/enemy.cppm
        src/game.cppm
        src/explosion.cppm
        src/bonus.cppm
        src/lifeBonus.cppm
        src/speedBonus.cppm
        src/bulletBonus.cppm
)
add_executable(main src/main.cpp
        src/explosion.cppm
        src/bonus.cppm
        src/lifeBonus.cppm
        src/speedBonus.cppm
        src/bulletBonus.cppm
)
target_sources(main
        PUBLIC
        FILE_SET CXX_MODULES
        FILES
        ${PROJECT_FILES}
)
target_link_libraries(main PRIVATE sfml-graphics sfml-audio)
target_compile_features(main PRIVATE cxx_std_23)

if(WIN32)
    add_custom_command(
            TARGET main
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:main>
            VERBATIM)
endif()