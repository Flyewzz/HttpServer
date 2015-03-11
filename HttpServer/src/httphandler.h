//
//  httphandler.h
//  HttpServer
//
//  Created by Alexey Halaidzhy on 09.03.15.
//  Copyright (c) 2015 Alexey Halaidzhy. All rights reserved.
//
//  In that module there are functions for parsing http request and creation of http response
//
#ifndef HttpServer_httphandler_h
#define HttpServer_httphandler_h
#include <sys/types.h>

typedef enum {_200, _400, _403, _404, _405} http_status;
struct http_response {
    http_status status;
    int data_fd;
    off_t data_size;
};

void **get_response(char *);
off_t do_sendfile(int, int, off_t, off_t);
#endif
