#include "struct.h"

#include "utils.h"
#include "string.h"

#include <stdlib.h>
#include <stdio.h>

request* alloc_request() {
    request *r = malloc(sizeof(request));
    r->n_header = 0;
    return r;
}

void free_request(request* r) {
    for (int i = 0; i < r->n_header; i++) {
        free(r->headers[i]);
    }
    free(r);

}



int parse_request(request* r, char* request_buffer, int buffer_size) {
    printf("Parsing :\n");
    char *copy = malloc(BUFFER_SIZE);

    // METHOD, PATH and PROTOCOL
    strncpy(copy, request_buffer, BUFFER_SIZE);
    char* new_line = strchr(copy, '\r');
    *new_line = '\x00';
    *(new_line+1) = '\x00';
    sscanf(copy, "%s %s %s", r->method, r->path, r->protocol);

    copy = new_line+2;

    //Loop over headers
    for (new_line = strchr(copy, '\r'), *new_line = '\x00', *(new_line+1) = '\x00'; new_line != copy; (copy = new_line+2), new_line = strchr(copy, '\r'), *new_line = '\x00', *(new_line+1) = '\x00') {
        if (r->n_header >= MAX_HEADERS) {
            fprintf(stderr, "Too many headers\n");
            return -1;
        }
        int size = strlen(copy) >= BUFFER_SIZE ? BUFFER_SIZE : strlen(copy); 
        char* header_line = malloc(size+1);
        strncpy(header_line, copy, size);
        header_line[size] = 0;
        r->headers[r->n_header++] = header_line;
    }
    return 0;

}

int get_header(request* r, char* name, char* dest) {
    int size = strlen(name);
    if (size + 2 >= BUFFER_SIZE) {
        fprintf(stderr, "Too large header");
        return -1;
    }
    char header_name[BUFFER_SIZE]; 
    strncpy(header_name, name, BUFFER_SIZE-2);
    strcat(header_name, ": ");
    size += 2;

    for (int i = 0; i < r->n_header; i++) {
        if (strncmp(r->headers[i], header_name, size) == 0) {
            strncpy(dest, r->headers[i] + size, BUFFER_SIZE-1);
            return 0;
        }
    }
    return -1;
}



void print_request(request* r) {
    printf("Printing :\n");
    printf("Method : %s\n", r->method);
    printf("Path : %s\n", r->path);
    printf("Protocol : %s\n", r->protocol);
    for (int i = 0; i < r->n_header; i++) {
        printf("%s\n", r->headers[i]);
    }
}
