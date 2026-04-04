#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int ping(char *p);

int main(int argc, char *argv[]){
    if (argc != 2) {
        printf("Usage: %s <ipv4>\n", argv[0]);

        return 1;
    }


    while(1){
        sleep(1);
        ping(argv[1]);
    }

    return 0;
}


