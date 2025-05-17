cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CURRENT_SOURCE_DIR D:/) 
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")


if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (Bauer)

find_package(Boost REQUIRED COMPONENTS asio)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} Boost::asio)

add_executable(test test/test.cpp)

add_library(ThreadPool  threadPool.hpp)
target_link_libraries(ThreadPool Boost::asio)

add_library(TaskQueue INTERFACE taskQueue.hpp )
target_include_directories(TaskQueue INTERFACE include)

target_link_libraries(ThreadPool TaskQueue)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Bauer PROPERTY CXX_STANDARD 20)
endif()
