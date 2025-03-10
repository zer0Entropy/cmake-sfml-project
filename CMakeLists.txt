cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

find_package(nlohmann_json 3.11.3 REQUIRED)

add_executable(main
                src/main.cpp
                src/log.cpp
                src/game.cpp
                src/state.cpp
                src/level.cpp
                src/map.cpp
                src/terrain.cpp
                src/tile.cpp
                src/player.cpp
                src/mainMenu.cpp
                src/gameplay.cpp
                src/resource/resourceMgr.cpp
                src/component/component.cpp
                src/component/text.cpp
                src/component/boundingBox.cpp
                src/component/mouseOver.cpp
                src/component/leftClick.cpp
                src/component/creature.cpp
                src/system/input.cpp
                src/system/render.cpp)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics SFML::Audio nlohmann_json::nlohmann_json)
