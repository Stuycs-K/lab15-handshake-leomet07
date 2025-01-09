#include "pipe_networking.h"

static void sighandler(int signo) {
    if (signo == SIGINT) {
        printf("Control+C: Removing WKP\n");
        int rm_wkp_status = remove(WKP);
        if (rm_wkp_status == -1) {
            err();
        }
        printf("Exiting!\n");
        exit(0);
    }
    if (signo == SIGPIPE){
        printf("Pipe broken... soft handling it!\n");
    }
}

int main() {
    signal(SIGINT, sighandler);
    signal(SIGPIPE, sighandler);
    int count = 0;
    while (1) {
        int to_client;
        int from_client;

        from_client = server_handshake(&to_client);

        while (1){
            char msg_to_client[256];
            sprintf(msg_to_client, "msg from server: you are the %d index client.", count);

            printf("Abt to write...\n");
            int write_hi_status = write(to_client, msg_to_client, strlen(msg_to_client) + 1);
            if (write_hi_status == -1) {
                // err(); // don't die in case pipe is broken
                break;
            }
            printf("Server successfully wrote to client.\n");

            char msg_from_client[256];
            int read_hi_status = read(from_client, msg_from_client, 256);
            if (read_hi_status == -1) {
                err();
                break;
            }
            printf("Server successfully read |%s| from client.\n", msg_from_client);
            sleep(1);
        }
        

        // finishing server_handshake deletes WKP already by this point
        close(to_client);
        close(from_client);
        count++;
    }

   
}
