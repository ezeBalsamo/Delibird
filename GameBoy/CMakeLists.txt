add_library(
        entry_point_validator
        include/entry_point_validator.h
        src/entry_point_validator.c)

target_link_libraries(entry_point_validator -lcommons)

add_executable(gameboy src/gameboy.c)
target_link_libraries(gameboy entry_point_validator)