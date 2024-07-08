#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <errno.h>
#include <string.h>
#include <asm-generic/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "utils.h"
#include "struct.h"

#define BACKLOG 3


int main(int argc, char *argv[]) {

    int fdsocket;
    if ((fdsocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "Error during the socket creation : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    int opt;
    if (setsockopt(fdsocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
        fprintf(stderr, "Error during the socket option : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8000);

    if (bind(fdsocket, (struct sockaddr *) &address, sizeof(address)) != 0) {
	    printf("Error during binding: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
    }

    if (listen(fdsocket, BACKLOG) != 0) {
	    printf("Error during listening: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
    }   


    for (char again = 'y'; again != 'N'; printf("Again? (y/N)"), again=getc(stdin), getc(stdin), fflush(stdin)) {
        int clientSocket;
        struct sockaddr_in clientAddress;
        unsigned int addrLen = sizeof(clientAddress);
        if ((clientSocket = accept(fdsocket, (struct sockaddr *) &clientAddress, &addrLen)) != -1) {
            char buffer[BUFFER_SIZE];
            // Convertion de l'IP en texte
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(clientAddress.sin_addr), ip, INET_ADDRSTRLEN);
            printf("Connexion de %s:%i\n", ip, clientAddress.sin_port);
            int len = read(clientSocket, buffer, BUFFER_SIZE-1);
            buffer[len] = '\x00';
            printf("%d bytes read\n", len);
            write(clientSocket, "Talina la GOAT", 14);

            printf("\nBuffer : \n%s\nEnd of buffer\n", buffer);
            for (int i = 0; i < len; i++) {
                printf("%02x-", buffer[i]);
            }

            request* r = alloc_request();
            if (parse_request(r, buffer, BUFFER_SIZE)) {
                fprintf(stderr, "Error during parsing\n");
                return -1;
            }
            print_request(r);

            char header[BUFFER_SIZE];
            if (get_header(r, "Host", header) != -1) {
                printf("Host : %s\n", header);
            }

            if (get_header(r, "User-Agent", header) != -1) {
                printf("User-Agent : %s\n", header);
            }

            if (get_header(r, "Connection", header) != -1) {
                printf("Connection : %s\n", header);
            }
            
            free_request(r);
            
            
            
            close(clientSocket);
            
        }
    }

    



    close(fdsocket);
    printf("End!\n");
    return 0;
}