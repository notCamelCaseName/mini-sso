#ifndef H_HANDLER
#define H_HANDLER

#include <arpa/inet.h>
#include <bits/pthreadtypes.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define HASH_TABLE_SIZE 8191

void init_arrays();

// Struct to pass connection info to handler function
struct ConnInfo {
  int client_socket;
  struct sockaddr_in client_address;
};

struct User {
  char pseudo[BUFFER_SIZE];
  char password[BUFFER_SIZE];
  int token;
};

struct HashMapNode {
  char key[BUFFER_SIZE];
  struct User *user;
  struct HashMapNode *next;
};

void *handle_connection(void *args);

void clear_buf(char *buf, size_t len);

int gen_cookie();

unsigned long hash(char *str);

struct User *create_user(char *pseudo, char *password);

struct HashMapNode *get_node_p(char *key);

void insert_hashmap(struct User *user);
#endif
