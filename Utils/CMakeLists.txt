add_library(sockets SHARED src/socket.c)

target_link_libraries(sockets PUBLIC pthread)