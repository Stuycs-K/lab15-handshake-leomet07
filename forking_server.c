#include "pipe_networking.h"

void remove_wkp_if_exist(){
    if (access(WKP, F_OK) == 0){ // if WKP file exists
        int rm_wkp_status = remove(WKP);
        if (rm_wkp_status == -1) {
            err();
        }
    }
}

static void sighandler(int signo) {
    if (signo == SIGINT) {
        printf("Control+C: Handling WKP deletion!\n");
        remove_wkp_if_exist();
        printf("Exiting!\n");
        exit(0);
    }
}

int main() {
    int count = 0;
    signal(SIGINT, sighandler);
    remove_wkp_if_exist(); // just in case left over from earlier versions!
    while (1) {
        int to_client;
        int from_client;

        // from_client = server_handshake(&to_client);
        from_client = server_setup(); // recieved a message from client and deleted pipe

        pid_t child_pid;
        child_pid = fork();

        if (child_pid == 0) {
            signal(SIGINT, SIG_IGN); // disable interrupt for child so sighandler only runs in parent!
            // finish the other half of hanshake inside child
            server_handshake_half(&to_client, from_client);

            // looping convo with child
            while (1) {
                char msg_to_client[256];
                sprintf(msg_to_client, "msg from server: ping (%d)", count);

                int write_hi_status = write(to_client, msg_to_client, strlen(msg_to_client) + 1);
                if (write_hi_status == -1) {
                    err();
                }
                printf("Server successfully wrote |%s| to client.\n", msg_to_client);

                char msg_from_client[256];
                int read_hi_status = read(from_client, msg_from_client, 256);
                if (read_hi_status == -1) {
                    err();
                }
                printf("Server successfully read |%s| from client.\n", msg_from_client);

                count++;

                sleep(1); // sleep for 1 second
            }
            // finishing server_handshake deletes WKP already by this point
            close(to_client);
            close(from_client);

            exit(0); // make sure child dies after it is done
        } else {
            
        }
    }
}
