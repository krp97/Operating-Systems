cmake_minimum_required(VERSION 2.8.2)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(${CMAKE_VERSION} VERSION_LESS 2.8.2)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(mt1 DESCRIPTION "Multithreading Task 1" LANGUAGES CXX)

set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CURSES_USE_NCURSES TRUE)
find_package(Curses REQUIRED)
find_package(Threads REQUIRED)

include_directories(${CURSES_INCLUDE_DIRS})

set(HEADERS
    ${CMAKE_SOURCE_DIR}/include/ball.hpp
    ${CMAKE_SOURCE_DIR}/include/window.hpp
    ${CMAKE_SOURCE_DIR}/include/direction.hpp
)

set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/window.cpp
    ${CMAKE_SOURCE_DIR}/src/ball.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})


set_target_properties(${PROJECT_NAME}
                      PROPERTIES CXX_STANDARD
                                 17
                                 CXX_STANDARD_REQUIRED
                                 ON)

target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARY})
target_link_libraries(${PROJECT_NAME} Threads::Threads)