# CMake file for chap_03 example5

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(chap_03_ex5 example5.cpp)

target_link_libraries(chap_03_ex5 GfxModule)
