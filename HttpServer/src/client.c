//
//  client.c
//  HttpServer
//
//  Created by Alexey Halaidzhy on 09.03.15.
//  Copyright (c) 2015 Alexey Halaidzhy. All rights reserved.
//
//  Simple realisation of client for test

#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include "common.h"

int main() {
    int sd;
    struct sockaddr_in addr;
    int addr_len = sizeof(addr);
    char buffer[BUFFER_SIZE];
    bzero(buffer, BUFFER_SIZE);

    // Create client socket
    if((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
        return -1;
    }

    bzero(&addr, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    addr.sin_addr.s_addr =  htonl(INADDR_LOOPBACK);

    // Connect to server socket
    if(connect(sd, (struct sockaddr *)&addr, sizeof addr) < 0)
    {
        perror("Connect error");
        return -1;
    }

    while (strcmp(buffer, "q") != 0)
    {
        // Read input from user and send message to the server
        printf("input string: ");
        gets(buffer);
        if (strlen(buffer) > 0) {
            send(sd, buffer, strlen(buffer), 0);
            // Receive message from the server
            recv(sd, buffer, BUFFER_SIZE, 0);
            printf("message: %s\n", buffer);
        }
    }

    return 0;
}
