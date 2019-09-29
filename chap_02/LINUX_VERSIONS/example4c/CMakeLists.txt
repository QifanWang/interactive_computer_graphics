# CMake file for chap_02 example4c

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(chap_02_ex4c example4c.cpp)

target_link_libraries(chap_02_ex4c GfxModule)
