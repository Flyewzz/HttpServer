//
//  common.h
//  HttpServer
//
//  Created by Alexey Halaidzhy on 09.03.15.
//  Copyright (c) 2015 Alexey Halaidzhy. All rights reserved.
//
//  In that module there are common constants and types
//
#ifndef HttpServer_common_h
#define HttpServer_common_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>
#include <limits.h>
#include <fcntl.h>

typedef enum {false, true} bool;
extern const bool DEBUG_MODE;
extern char ROOTDIR[PATH_MAX + 1];
extern int CORES;
extern const int SERVER_PORT;
extern const int CONNECTION_QUEUE_SIZE;
extern const int BUFFER_SIZE;
#endif
