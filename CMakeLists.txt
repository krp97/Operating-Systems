cmake_minimum_required(VERSION 3.12)

set(CMAKE_BUILD_TYPE Debug)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(Multithreading-Opsys DESCRIPTION "Multithreading tasks" LANGUAGES CXX)

set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CURSES_USE_NCURSES TRUE)
find_package(Curses REQUIRED)
find_package(Threads REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
if(CMAKE_CXX_CPPCHECK)
    list(APPEND CMAKE_CXX_CPPCHECK
            "--enable=warning"
            "--inconclusive"
            "--force"
            "--inline-suppr")
endif()

# ------------------------ Task1 ------------------------
add_executable(task1)

target_include_directories(task1 PUBLIC
    ${CMAKE_SOURCE_DIR}/Task1/include
)

target_sources(task1 PUBLIC
    ${CMAKE_SOURCE_DIR}/Task1/src/main.cpp
    ${CMAKE_SOURCE_DIR}/Task1/src/window.cpp
    ${CMAKE_SOURCE_DIR}/Task1/src/direction.cpp
    ${CMAKE_SOURCE_DIR}/Task1/src/ball.cpp)


# ------------------------ Task2 ------------------------
add_executable(task2)

target_include_directories(task2 PUBLIC
    ${CMAKE_SOURCE_DIR}/Task2/include
)

target_sources(task2 PUBLIC
    ${CMAKE_SOURCE_DIR}/Task2/src/main.cpp
    ${CMAKE_SOURCE_DIR}/Task2/src/main_handler.cpp
    ${CMAKE_SOURCE_DIR}/Task2/src/window.cpp
    ${CMAKE_SOURCE_DIR}/Task2/src/direction.cpp
    ${CMAKE_SOURCE_DIR}/Task2/src/ball.cpp)


# ----------- Common properties and libraries -----------
set_target_properties(task1 task2
                      PROPERTIES CXX_STANDARD
                                 17
                                 CXX_STANDARD_REQUIRED
                                 ON)

set(LIBS ${CURSES_LIBRARY} Threads::Threads)

target_link_libraries(task1 ${LIBS})
target_link_libraries(task2 ${LIBS})