cmake_minimum_required(VERSION 3.20)

# Pobiera automatycznie nazwę, którą nadałeś projektowi przy tworzeniu
project(${CMAKE_PROJECT_NAME} CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Flaga statyczna dla FreeGLUT (naprawia błędy __imp_)
add_definitions(-DFREEGLUT_STATIC)

# Mówimy kompilatorowi, żeby widział folder GL/ w tym projekcie
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Automatycznie zbiera wszystkie pliki .cpp w tym nowym folderze
file(GLOB SOURCES "*.cpp")
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

# Szukamy libfreeglut.a, który przed chwilą tam wkleiłeś
find_library(FREEGLUT_LIB NAMES freeglut PATHS ${CMAKE_CURRENT_SOURCE_DIR} NO_DEFAULT_PATH)

# Linkujemy FreeGLUT oraz domyślny OpenGL Windowsa
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        ${FREEGLUT_LIB}
        opengl32
        glu32
)