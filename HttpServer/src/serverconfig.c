//
//  serverconfig.c
//  HttpServer
//
//  Created by Alexey Halaidzhy on 08.03.15.
//  Copyright (c) 2015 Alexey Halaidzhy. All rights reserved.
//

#include "serverconfig.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>
#include <limits.h>

char *ROOTDIR = NULL;
int CORES = 0;

void usage(const char *const filename) {
    printf("Usage: %s\n"
           "\t -r <ROOTDIR>     -- path to root directory\n"
           "\t -c <CORES>       -- amount of cores\n", filename);
}

bool setup_root_dir(const char *const root_dir) {
    char absolute_path[PATH_MAX + 1];
    realpath(root_dir, absolute_path);
    if (!access(absolute_path, X_OK)) {
        ROOTDIR = (char *)malloc((strlen(absolute_path) + 1) * sizeof(char));
        strcpy(ROOTDIR, absolute_path);
        printf("ROOTDIR setted to '%s'\n", ROOTDIR);
        return true;
    }
    else {
        switch (errno) {
            case ENOENT:
                printf("'%s' doesn't exist\n", absolute_path);
                break;

            case EACCES:
                printf("Permission denied on '%s'\n", absolute_path);
                break;

            case ENOTDIR:
                printf("'%s' is not a directory\n", absolute_path);
                break;

            default:
                printf("An error with code %d occured while getting '%s'\n", errno, absolute_path);
                break;
        }
        return false;
    }
}

bool setup_cores(const char *const cores) {
    int cores_count = atoi(cores);
    long min_available_cores_count = 1;
    long max_available_cores_count = sysconf(_SC_NPROCESSORS_ONLN);
    if (cores_count >= min_available_cores_count && cores_count <= max_available_cores_count) {
        CORES = cores_count;
        printf("CORES setted to %d\n", CORES);
        return true;
    }
    else {
        printf("Incorrect value. One should use value between %ld and %ld\n", min_available_cores_count, max_available_cores_count);
        return false;
    }
}

char setup_input_args(int argc, char *argv[]) {
    bool r_exists = false;
    bool c_exists = false;
    while (1) {
        int c;
        if ((c = getopt(argc, argv, "r:c:")) == -1) {
            break;
        }
        switch (c) {
            case 'r':
                if (!r_exists) {
                    r_exists = setup_root_dir(optarg);
                }
                else {
                    printf("'-r' parameter already setted\n");
                }
                break;

            case 'c':
                if (!c_exists) {
                    c_exists = setup_cores(optarg);
                }
                else {
                    printf("'-c' parameter already setted\n");
                }
                break;
        }
    }
    return r_exists && c_exists;
}

void free_memory_for_input_args() {
    free(ROOTDIR);
}
