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

typedef int (*fct_t)(int);

fct_t fct = NULL;

int __attribute__ ((noinline)) empty (int it) {
    if(it-1 == 0)
        return 5;
    else
        return fct(it-1);
}
void warmup(){
    for(int i = 0; i < 10000000; i++);
}

int main(int argc, char const *argv[])
{
    fct = empty;
    warmup();
    int i = 0;
    INIT_TIMER(jmp);
    unsigned long long sum = 0;
    int indirect_jumps = 0;

    if(argc < 2) {
        printf("too few arguments.\nUsage:\n\t%s <num indrect jumps per iteration>\n", argv[0]);
        exit(1);
    }

    indirect_jumps = atoi(argv[1]);

#ifdef IBPB
    int btbf = open(COOL_DEVICE_PATH, 0);
    if(btbf < 0) {
        printf("Can't find btb flush device.\nPlease insmod cool.ko first.\n");
        exit(1);
    }
#endif

    // train btb
    for(i = 0; i < TRAINING_ITERATIONS; i++) {
        fct(indirect_jumps);
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
        fct(indirect_jumps);
        END_TIMER(jmp);
        sum += SPEND_TIME(jmp);
    }

#ifdef IBPB
    printf("with IBPB \t\t %d \t %lld \t %f\n", indirect_jumps, sum, ((float) sum) / ITERATIONS);
#else
    printf("wo IBPB \t\t %d \t %lld \t %f\n", indirect_jumps, sum, ((float) sum) / ITERATIONS);
#endif

#ifdef IBPB
    close(btbf);
#endif

    return 0;
}
