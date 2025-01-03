#include "pipe_networking.h"

static void sighandler(int signo) {
    if (signo == SIGINT) {
        printf("Control+C: Removing WKP");
        int rm_wkp_status = remove(WKP);
        if (rm_wkp_status == -1) {
            err();
        }
        printf("Exiting!");
        exit(0);
    }
}

int main() {
    signal(SIGINT, sighandler);
    while (1) {
        int to_client;
        int from_client;

        from_client = server_handshake(&to_client);
        close(to_client);
        close(from_client);
    }

    // You must now also add a sighandler for SIGINT. This will delete the WKP befor ending the server.
}
