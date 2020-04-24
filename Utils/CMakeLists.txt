add_library(sockets SHARED src/socket.c src/operation_serialization.c)

target_link_libraries(sockets PUBLIC pthread)