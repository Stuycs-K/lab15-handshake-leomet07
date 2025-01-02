#include "pipe_networking.h"
// UPSTREAM = to the server / from the client
// DOWNSTREAM = to the client / from the server
/*=========================
  server_setup

  creates the WKP and opens it, waiting for a  connection.
  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/

void err() {
    printf("ERROR %d: %s\n", errno, strerror(errno));
    exit(errno);
}

int server_setup() {
    mkfifo(WKP, 0666);

    printf("Server: Waiting for first part of handshake from a client...\n");

    int from_client = open(WKP, O_RDONLY); // opening the pipe blocks until the other end opens, so a blocking read not needed
    if (from_client == -1) {
        err();
    }

    printf("Server: Client connection opened (Client has connected)!\n");

    // once connection made, delete disk reference to well known pipe, pipe remains open in memory
    int remove_pipe_location_file_status = remove(WKP);
    if (remove_pipe_location_file_status == -1) {
        err();
    }
    printf("Server: Removed reference to WKP on disk.\n");
    return from_client;
}

/*=========================
  server_handshake
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe (Client's private pipe).

  returns the file descriptor for the upstream pipe (see server setup).
  =========================*/
int server_handshake(int *to_client) {
    int from_client = server_setup(); // recieved a message from client and deleted pipe
    char from_client_message[256];

    printf("Server: waiting to read...\n");
    int read_status = read(from_client, from_client_message, 256); // reads fifo name
    if (read_status == -1) {
        err();
    }
    printf("Server: Message from client recieved |%s|.\n", from_client_message);

    // send into PP (SYN_ACK)
    *to_client = open(from_client_message, O_WRONLY); // open_private_pipe
    if (*to_client == -1) {
        err();
    }

    int syn_ack_int = 43187;
    char random_string[256];
    sprintf(random_string, "%d", syn_ack_int);

    int write_status = write(*to_client, random_string, strlen(random_string) + 1);
    if (write_status == -1) {
        err();
    }

    printf("Server: Sent SYN_ACK |%s|\n", random_string);

    // Wait for ack
    char client_ack[256];
    int ack_read_status = read(from_client, client_ack, 256);
    if (ack_read_status == -1) {
        err();
    }

    printf("Server: Recieved ack |%s|\n", client_ack);

    int ack_int = -1;
    sscanf(client_ack, "%d", &ack_int);
    if (ack_int == (syn_ack_int + 1)) {
        printf("Server: Expected ACK (%d) matches client ACK (%d)\n", syn_ack_int + 1, ack_int);
        printf("Server: Handshake success!\n");
    } else {
        printf("Expected ACK (%d) does not match client ACK (%d)\n", syn_ack_int + 1, ack_int);
        printf("Server: Handshake failed... quitting!\n");
        exit(1);
    }

    return from_client;
}

/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
    *to_server = open(WKP, O_WRONLY);
    if (*to_server == -1) {
        err();
    }
    char string_pid[256];

    sprintf(string_pid, "%d", getpid());

    int mkfifo_status = mkfifo(string_pid, 0666);
    if (mkfifo_status == -1) {
        err();
    }
    printf("Client: Made private pipe!\n");

    int write_pp_name_to_wkp_status = write(*to_server, string_pid, strlen(string_pid) + 1);
    if (write_pp_name_to_wkp_status == -1) {
        err();
    }
    printf("Client: Wrote private pipe name (child pid) to WKP!\n");

    // open private pipe
    int from_server = open(string_pid, O_RDONLY); // this hangs until server opens this
    if (from_server == -1) {
        err();
    }
    printf("Client: Opened private pipe and server has connected!\n");

    // delete pp
    int del_pp_status = remove(string_pid);
    if (del_pp_status == -1) {
        err();
    }
    printf("Client: Deleted PP file on disk!\n");

    // read
    char syn_ack[256];
    int read_syn_ack_status = read(from_server, syn_ack, 256);
    if (read_syn_ack_status == -1) {
        err();
    }
    printf("Client: Read syn_ack |%s|\n", syn_ack);

    // parse d
    int syn_ack_int = -1;
    sscanf(syn_ack, "%d", &syn_ack_int);
    printf("Client: Parsed int before +1: |%d|\n", syn_ack_int);
    int ack_int = syn_ack_int + 1;
    printf("Client: Parsed int after +1: |%d|\n", ack_int);

    char ack[256];
    sprintf(ack, "%d", ack_int);

    int write_ack_status = write(*to_server, ack, strlen(ack) + 1);
    if (write_ack_status == -1) {
        err();
    }

    printf("Client: Finished sending ack!\n");

    return from_server;
}

/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {
    int to_client = 0;
    return to_client;
}
