#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include "module/cool_header.h"

typedef unsigned long long CYCLES;

#define getCCP(name)	do { unsigned long long rax=0,rdx=0;\
	asm volatile ( "rdtscp\n" : "=a" (rax), "=d" (rdx): : "%ecx" );\
	name = (rdx << 32) + rax; \
 	 } while(0)

#define INIT_TIMER(name) CYCLES t_start_##name = 0.0, t_end_##name = 0.0
#define START_TIMER(name) getCCP(t_start_##name)
#define END_TIMER(name) getCCP(t_end_##name)
#define SPEND_TIME(name) (t_end_##name - t_start_##name)

int empty (int arg) {
    int a = 5;
    return a + arg;
}

void warmup(){
    for(int i = 0; i < 10000000; i++);
}

int main(int argc, char const *argv[])
{
    warmup();
    int i = 0;
    int (*fct) () = empty;
    INIT_TIMER(jmp);
    unsigned long long sum = 0;
    int dump = 0;

#ifdef IBPB
    int btbf = open(COOL_DEVICE_PATH, 0);
    if(btbf < 0) {
        printf("Can't find btb flush device.\nPlease insmod cool.ko first.\n");
        exit(1);
    }
#endif

    // train btb
    for(i = 0; i < TRAINING_ITERATIONS; i++) {
        dump = fct(dump);
    }

    // in a loop btb-flush, then measure the call/jmp
    for (i = 0; i < ITERATIONS; i++) {
#ifdef IBPB
        if(ioctl(btbf, COOL_IOCTL_CMD_BTBF, 0) < 0) {
            printf("Failed to execute ibpb.\n");
            exit(1);
        }
#endif

        START_TIMER(jmp);
        dump = fct(dump);
        END_TIMER(jmp);
        sum += SPEND_TIME(jmp);
    }

#ifdef IBPB
    printf("with IBPB \t\t %lld \t %f\n", sum, ((float) sum) / ITERATIONS);
#else
    printf("%d ITERATIONS \t sum \t\t avg\n", ITERATIONS);
    printf("wo IBPB \t\t %lld \t %f\n", sum, ((float) sum) / ITERATIONS);
#endif

#ifdef IBPB
    close(btbf);
#endif

    return 0;
}
