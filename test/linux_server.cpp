#include <fstream>

#include "rtp.hpp"
// #include "RTMPController.hpp"

#ifdef __linux__

#include <stdio.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>

#endif

#include <string.h>

#define DEFAULT_BUFFER_SIZE 4096
#define PORT 1935

int main()
{
    int server_fd, new_socket, valread; 
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[DEFAULT_BUFFER_SIZE];

    char* hello = "Hello from server";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)))
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0)
    {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    valread = read(new_socket, buffer, DEFAULT_BUFFER_SIZE);
    printf("%s\n", buffer);
    send(new_socket, hello, strlen(hello), 0);
    printf("Message sent.");
}

