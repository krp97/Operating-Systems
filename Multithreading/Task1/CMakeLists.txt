cmake_minimum_required(VERSION 2.8.2)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(${CMAKE_VERSION} VERSION_LESS 2.8.2)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(mt1 DESCRIPTION "Multithreading Task 1" LANGUAGES CXX)

#set(HEADERS)

set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} #${HEADERS}
)

set_target_properties(${PROJECT_NAME}
                      PROPERTIES CXX_STANDARD
                                 17
                                 CXX_STANDARD_REQUIRED
                                 ON)
