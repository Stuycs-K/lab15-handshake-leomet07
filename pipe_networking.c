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

    int from_client = open(WKP, O_RDONLY);
    if (from_client == -1) {
        err();
    }
    printf("Waiting for first part of handshake from a client...\n");
    char from_client_message[256];
    int connection_made = read(from_client, from_client_message, 256);
    if (connection_made == -1) {
        err();
    }
    printf("Message from client recieved |%s|.\n", from_client_message);
    // once connection made, delete disk reference to well known pipe, pipe remains open in memory
    int remove_pipe_location_file_status = remove(WKP);
    if (remove_pipe_location_file_status == -1) {
        err();
    }
    printf("Removed reference to WKP on disk.\n");
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
    int from_client = server_setup();
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
    int from_server;
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
