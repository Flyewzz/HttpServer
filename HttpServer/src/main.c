//
//  main.c
//  HttpServer
//
//  Created by Alexey Halaidzhy on 08.03.15.
//  Copyright (c) 2015 Alexey Halaidzhy. All rights reserved.
//

#include "serverconfig.h"
#include <stdlib.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include "ev.h"

static const int SERVER_PORT = 8080;
static const int CONNECTION_QUEUE_SIZE = 10;
extern int CORES;

int set_nonblock(int sock, bool flag) {
    int fl = fcntl(sock, F_GETFL);
    if (fl == -1) {
        return fl;
    }
    if (flag) {
        fl |= O_NONBLOCK;
    }
    else {
        fl &= ~O_NONBLOCK;
    }
    return fcntl(sock, F_SETFL, fl);
}

void create_workers() {
    int pid = 1;
    for (int i = 0; i < CORES - 1; ++i) {
        if (pid > 0) {
            pid = fork();
        }
    }
}

int main(int argc, char *argv[]) {
    if (!setup_input_args(argc, argv)) {
        usage(argv[0]);
        return 0;
    }
    
    struct sockaddr_in *sa = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
    sa->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    sa->sin_family = AF_INET;
    sa->sin_port = htons(SERVER_PORT);
    
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation error\n");
        exit(1);
    }
    int flag = true;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    long fl = fcntl(sock, F_GETFL);
    if (fl == -1) {
        perror("Can't get fcntl sock option\n");
        exit(1);
    }
    
    set_nonblock(sock, true);
    
    if (bind(sock, (struct sockaddr *)sa, sizeof(struct sockaddr_in)) == -1) {
        perror("Bind error\n");
        exit(1);
    }
    
    if (listen(sock, CONNECTION_QUEUE_SIZE) == -1) {
        perror("Listen error\n");
        exit(1);
    }
    
    create_workers();
    printf("Hi from worker with pid %d and ppid %d\n", getpid(), getppid());
    
    free_memory_for_input_args();
    return 0;
}
