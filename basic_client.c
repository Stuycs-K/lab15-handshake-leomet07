#include "pipe_networking.h"

int main() {
    int to_server;
    int from_server;

    from_server = client_handshake(&to_server);

    char msg_from_server[256];

    int read_hi_status = 0;
    while (read_hi_status == 0) {
        read_hi_status = read(from_server, msg_from_server, 256);
        if (read_hi_status == -1) {
            err();
        }
    }
    printf("Client successfully read (%d) bytes |%s| from server.\n", read_hi_status, msg_from_server);

    char msg_to_server[256] = "hi from client";
    int write_hi_status = write(to_server, msg_to_server, strlen(msg_to_server) + 1);
    if (write_hi_status == -1) {
        err();
    }
    printf("Client successfully wrote to server.\n");
}
