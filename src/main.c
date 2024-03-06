#include <arpa/inet.h>
#include <bits/pthreadtypes.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>

#include "handler.h"

#define PORT 45823

int main(int argc, char **argv) {
  // Socket initialisation
  int fdsocket;
  if ((fdsocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    printf("Error : Unable to create socket\n");
    return EXIT_FAILURE;
  }
  int opt = 1;
  if (setsockopt(fdsocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt)) != 0) {
    printf("Error : Unable to set socket options\n");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in address;

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if (bind(fdsocket, (struct sockaddr *)&address, sizeof(address)) != 0) {
    printf("Error : Unable to bind socket\n");
    exit(EXIT_FAILURE);
  }

  if (listen(fdsocket, 10) != 0) {
    printf("Error : Unable to listen\n");
    exit(EXIT_FAILURE);
  }

  printf("Listening on port %d\n", PORT);

  struct sockaddr client_address;
  unsigned int addrlen = sizeof(client_address);

  init_arrays();
  srand(time(NULL));

  while (1) {
    // We accept any incoming connection and spawn a thread to handle it
    int client_socket = accept(fdsocket, &client_address, &addrlen);
    pthread_t thread_id;
    struct ConnInfo conn_info = {
      .client_socket = client_socket,
      .client_address = * (struct sockaddr_in *) &client_address // Cast to more friendly data type
    };
    pthread_create(&thread_id, NULL, handle_connection, &conn_info);
  }
}
