cmake_minimum_required(VERSION 3.20)
project(2d-game-engine-with-ecs LANGUAGES C CXX)

# -------------------- Common --------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIBRARY_DIR "${CMAKE_SOURCE_DIR}/Library")

# -------------------- SDL2 + addons --------------------
# ВАЖНО: путь должен указывать на папку, где лежит *Config.cmake*.
# Если у тебя структура .../lib/cmake/<Pkg>/SDL2Config.cmake,
# поменяй пути ниже на ".../lib/cmake/<Pkg>".
set(SDL2_DIR        "${LIBRARY_DIR}/SDL2/cmake")
set(SDL2_image_DIR  "${LIBRARY_DIR}/SDL2_image/cmake")
set(SDL2_ttf_DIR    "${LIBRARY_DIR}/SDL2_ttf/cmake")
set(SDL2_mixer_DIR  "${LIBRARY_DIR}/SDL2_mixer/cmake")

find_package(SDL2        CONFIG REQUIRED)
find_package(SDL2_image  CONFIG REQUIRED)
find_package(SDL2_ttf    CONFIG REQUIRED)
find_package(SDL2_mixer  CONFIG REQUIRED)

# -------------------- GLM --------------------
# Если в Library/glm есть официальный CMakeLists.txt (из репозитория GLM):
add_subdirectory("${LIBRARY_DIR}/glm" EXCLUDE_FROM_ALL)
set(HAVE_GLM_TARGET OFF)
if (TARGET glm::glm)
  set(HAVE_GLM_TARGET ON)
endif()

# -------------------- ImGui --------------------
# Подхватим все .cpp/.c/.hpp/.h, включая backend'ы (imgui_impl_*),
# если они лежат в той же папке.
file(GLOB_RECURSE IMGUI_FILES
  "${LIBRARY_DIR}/imgui/*.cpp"
  "${LIBRARY_DIR}/imgui/*.c"
  "${LIBRARY_DIR}/imgui/*.hpp"
  "${LIBRARY_DIR}/imgui/*.h"
)
# чтобы CMake автоматически переконфигурировался при добавлении новых файлов:
set_property(GLOBAL PROPERTY CMAKE_CONFIGURE_DEPENDS "${LIBRARY_DIR}/imgui")

add_library(imgui STATIC ${IMGUI_FILES})
target_include_directories(imgui PUBLIC "${LIBRARY_DIR}/imgui")

# ВАЖНО: чтобы исходники backend'ов видели <SDL.h>:
target_link_libraries(imgui PUBLIC SDL2::SDL2 SDL2::SDL2main)

# -------------------- Lua (STATIC) --------------------
# Собираем Lua из .c исходников. Исключаем lua.c (интерпретатор) и luac.c (компилятор).
file(GLOB_RECURSE LUA_SRC "${LIBRARY_DIR}/lua/*.c")
list(FILTER LUA_SRC EXCLUDE REGEX ".*/lua\\.c$")
list(FILTER LUA_SRC EXCLUDE REGEX ".*/luac\\.c$")

add_library(lua STATIC ${LUA_SRC})
target_include_directories(lua PUBLIC "${LIBRARY_DIR}/lua")
set_target_properties(lua PROPERTIES OUTPUT_NAME "lua") # lua.lib
if (MSVC)
  target_compile_definitions(lua PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

# -------------------- sol2 (header-only) --------------------
add_library(sol INTERFACE)
# Для #include <sol/sol.hpp> инклюд должен указывать на РОДИТЕЛЯ папки sol/
target_include_directories(sol INTERFACE "${LIBRARY_DIR}")

# -------------------- Executable --------------------
add_executable(2d-game-engine-with-ecs
  "Source/Main.cpp"
)

target_link_libraries(2d-game-engine-with-ecs PRIVATE
  SDL2::SDL2
  SDL2::SDL2main
  SDL2_image::SDL2_image
  SDL2_ttf::SDL2_ttf
  SDL2_mixer::SDL2_mixer
  imgui
  lua
  sol
)

# GLM: цель или include-дир, если цели нет
if (HAVE_GLM_TARGET)
  target_link_libraries(2d-game-engine-with-ecs PRIVATE glm::glm)
else()
  target_include_directories(2d-game-engine-with-ecs PRIVATE "${LIBRARY_DIR}/glm")
endif()
