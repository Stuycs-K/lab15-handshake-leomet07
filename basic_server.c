#include "pipe_networking.h"

int main() {
    int to_client;
    int from_client;

    from_client = server_handshake(&to_client);

    char msg_to_client[256] = "hi from server";
    int write_hi_status = write(to_client, msg_to_client, strlen(msg_to_client) + 1);
    if (write_hi_status == -1) {
        err();
    }
    printf("Server successfully wrote to client.\n");

    char msg_from_client[256];
    int read_hi_status = read(from_client, msg_from_client, 256);
    if (read_hi_status == -1) {
        err();
    }
    printf("Server successfully read |%s| from client.\n");
}
