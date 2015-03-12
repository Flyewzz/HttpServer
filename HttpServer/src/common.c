//
//  common.c
//  HttpServer
//
//  Created by Alexey Halaidzhy on 09.03.15.
//  Copyright (c) 2015 Alexey Halaidzhy. All rights reserved.
//

#include "common.h"

const bool DEBUG_MODE = true;
char ROOTDIR[PATH_MAX + 1];
int CORES = 0;
const int SERVER_PORT = 80;
const int CONNECTION_QUEUE_SIZE = 1024;
const int BUFFER_SIZE = 1024;
