#ifndef STRUCT_H
#define STRUCT_H

#define METHOD_BUFFER_SIZE 16
#define PATH_BUFFER_SIZE 1024
#define PROTOCOL_BUFFER_SIZE 1024
#define MAX_HEADERS 100

typedef struct {

    char method[METHOD_BUFFER_SIZE];
    char path[PATH_BUFFER_SIZE];
    char protocol[PROTOCOL_BUFFER_SIZE];

    char* headers[MAX_HEADERS];
    int n_header;

} request;


request* alloc_request();
void free_request(request* r);
int parse_request(request* r, char* request_buffer, int buffer_size);
int get_header(request* r, char* name, char* dest);
void print_request(request* r);

#endif