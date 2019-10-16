# CMake file for chap_03 example3

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(chap_03_ex3 example3.cpp)

target_link_libraries(chap_03_ex3 GfxModule)
