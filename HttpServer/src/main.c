//
//  main.c
//  HttpServer
//
//  Created by Alexey Halaidzhy on 08.03.15.
//  Copyright (c) 2015 Alexey Halaidzhy. All rights reserved.
//

#include "serverconfig.h"
#include "evserver.h"

int main(int argc, char *argv[]) {
    if (!setup_input_args(argc, argv)) {
        usage(argv[0]);
        return 0;
    }
    start_server();
    return 0;
}
