#include<stdio.h>
#include<unistd.h>
#include "ping.h"

int ping(char *p);

int main(int argc, char *argv[]){
    if (argc != 2) {
        printf("Usage: %s <ipv4>\n", argv[0]);

        return 1;
    }


    while(1){
        sleep(1);
        ping(argv[1]);
        sleep(1);
    }

    return 0;
}

int ping(char *p){
    printf("ping to %s: waiting for response ...\n", p);
    sleep(1);
    printf("response from %s: time=1ms\n", p);

    return 0;
}
