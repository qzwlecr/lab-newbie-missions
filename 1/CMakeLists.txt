project(index_and_search)
set(CMAKE_C_FLAGS "-lmagic -lhiredis")

add_executable(index index.c)
add_executable(search search.c)
