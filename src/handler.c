#include "handler.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

struct HashMapNode *users[HASH_TABLE_SIZE];
struct User *cookies[HASH_TABLE_SIZE];

void init_arrays() {
  for (int i = 0; i<HASH_TABLE_SIZE; i++) {
    users[i] = NULL;
    cookies[i] = NULL;
  }
}

// Handler function for incoming connection
void *handle_connection(void *args) {
  // Cast function argument and extract its data
  struct ConnInfo *conn_info = args;
  int client_socket = conn_info->client_socket;
  struct sockaddr_in client_address = conn_info->client_address;

  char ip[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &(client_address.sin_addr), ip, INET_ADDRSTRLEN);
  printf("Connection from : %s\n", ip);
  char buffer[3*BUFFER_SIZE];
  clear_buf(buffer, BUFFER_SIZE);
  char type[BUFFER_SIZE];
  char pseudo[BUFFER_SIZE];
  char password[BUFFER_SIZE];

  // Read and parse input
  read(client_socket, buffer, BUFFER_SIZE);
  sscanf(buffer, "%s %s", type, pseudo);
  strcpy(password, buffer + strlen(type) + strlen(pseudo) + 2);

  // Debug
  printf("[ip] : %s", buffer);
  printf("type : %s\n", type);
  printf("pseudo : %s\n", pseudo);
  printf("password : %s\n", password);

  struct HashMapNode *entry = get_node_p(pseudo);

  if (strncmp(type, "register", 9) == 0) {
    printf("Registering\n");
    if (entry == NULL) {
      struct User *new_user = create_user(pseudo, password);
      insert_hashmap(new_user);
    } else {
      write(client_socket, "Gros caca qui pue", 18);
    }
  } else if (strncmp(type, "creds", 6) == 0) {
    printf("Connecting with creds\n");
    if (entry == NULL) {
      write(client_socket, "Gros caca qui pue", 18);
    } else if (strncmp(entry->user->password, password, BUFFER_SIZE) == 0) {
      write(client_socket, "OK!\n", 5);
      char tmp[15];
      snprintf(tmp, 15, "%d\n", entry->user->token);
      write(client_socket, tmp, 15);
      clear_buf(tmp, 15);
      int cookie = gen_cookie();
      cookies[cookie] = entry->user;
      snprintf(tmp, 15, "%d\n", cookie);
      write(client_socket, tmp, 15);

    } else {
      write(client_socket, "Gros caca qui pue", 18);
    }
  } else if (strncmp(type, "cookie", 7) == 0) {
    printf("Connecting with cookie\n");
    int cookie = atoi(pseudo);
    if (cookie < HASH_TABLE_SIZE && cookies[cookie] != NULL) {
      write(client_socket, "OK!\n", 5);
      char tmp[15];
      snprintf(tmp, 15, "%d\n", cookies[cookie]->token);
      write(client_socket, tmp, 15);

    } else {
      write(client_socket, "Gros caca qui pue", 18);
    }

  } else {
    printf("wtf\n");
    write(client_socket, "Gros caca qui pue", 18);
  }

  // Graceful (?) exit
  close(client_socket);
  return NULL;
}

// Utility function
void clear_buf(char *buf, size_t len) {
  for (int i = 0; i < len; i++)
    buf[i] = 0;
}

int gen_cookie() {
  int res;
  do {
    res = rand() % HASH_TABLE_SIZE;
  } while (cookies[res] != NULL && res != 0);
  return res;
}

unsigned long hash(char *str) {
  unsigned long hash = 5381;
  int c;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}

struct User *create_user(char *pseudo, char *password) {
  struct User *ret = malloc(sizeof(struct User));
  strncpy(ret->pseudo, pseudo, BUFFER_SIZE);
  strncpy(ret->password, password, BUFFER_SIZE);
  ret->token = rand();
  return ret;
}

struct HashMapNode *get_node_p(char *key) {
  int key_hash = hash(key) % HASH_TABLE_SIZE;
  struct HashMapNode *res = users[key_hash];
  if (res == NULL)
    return res;

  if (res->next == NULL)
    return res;

  while (res != NULL && strncmp(key, res->key, BUFFER_SIZE)) {
    res = res->next;
  }

  return res;
}

void insert_hashmap(struct User *user) {
  struct HashMapNode *new_entry = malloc(sizeof(struct HashMapNode));
  new_entry->user = user;
  new_entry->next = NULL;
  strncpy(new_entry->key, user->pseudo, BUFFER_SIZE);
  int key_hash = hash(user->pseudo) % HASH_TABLE_SIZE;
  if (users[key_hash] != NULL) {
    new_entry->next = users[key_hash];
  }
  users[key_hash] = new_entry;
}
