//
//  serverconfig.h
//  HttpServer
//
//  Created by Alexey Halaidzhy on 08.03.15.
//  Copyright (c) 2015 Alexey Halaidzhy. All rights reserved.
//
//  In that module there are functions for validation and setting input args for server configuration
//
#ifndef HttpServer_serverconfig_h
#define HttpServer_serverconfig_h
#include "common.h"

void usage(const char *const);
bool setup_root_dir(const char *const);
bool setup_cores(const char *const);
char setup_input_args(int, char *[]);
#endif
