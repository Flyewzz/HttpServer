//
//  main.c
//  HttpServer
//
//  Created by Alexey Halaidzhy on 08.03.15.
//  Copyright (c) 2015 Alexey Halaidzhy. All rights reserved.
//

#include "serverconfig.h"

int main(int argc, char *argv[]) {
    if (!setup_input_args(argc, argv)) {
        usage(argv[0]);
        return 0;
    }
    free_memory_for_input_args();
    return 0;
}
