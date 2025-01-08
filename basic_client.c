#include "pipe_networking.h"

int main() {
    int to_server;
    int from_server;

    from_server = client_handshake(&to_server);

    while (1) {
        char *msg_from_server = calloc(256, sizeof(char)); // make sure mem is zeroed out to not fight with previous message

        int read_hi_status = 0;

        read_hi_status = read(from_server, msg_from_server, 256);
        if (read_hi_status == -1) {
            err();
        }

        if (read_hi_status == 0) {
            printf("Pipe was just closed (read zero bytes!)... quitting!\n");
            exit(0);
        }
        printf("Client successfully read |%s| from server.\n", msg_from_server);

        char msg_to_server[256] = "hi from client";
        int write_hi_status = write(to_server, msg_to_server, strlen(msg_to_server) + 1);
        if (write_hi_status == -1) {
            err();
        }
        printf("Client successfully wrote to server.\n");
        free(msg_from_server);
    }
}
