# CMake file for chap_02 example4

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(chap_02_ex4 example4.cpp)

target_link_libraries(chap_02_ex4 GfxModule)
