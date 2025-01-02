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
    int open_private_pipe = open(from_client_message, O_WRONLY);
    if (open_private_pipe == -1) {
        err();
    }

    char random_string[256] = "43187";

    int write_status = write(open_private_pipe, random_string, strlen(random_string) + 1);
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

    // from_client_message should be private pipe name, aka child process PID

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
    char ack[256];
    int read_ack_status = read(from_server, ack, 256);
    if (read_ack_status == -1) {
        err();
    }
    printf("Client: Read ack |%s|\n", ack);

    
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
