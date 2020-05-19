include_directories(include)
file(GLOB SOURCES "src/*.c")

add_library(utils SHARED ${SOURCES})

target_link_libraries(utils -lcommons pthread)