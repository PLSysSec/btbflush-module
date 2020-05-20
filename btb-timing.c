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

#define ITERATIONS 10000

int empty (int arg) {
    int a = 5;
    return a + arg;
}

int main(int argc, char const *argv[])
{
    int i = 0;
    int (*fct) () = empty;
    INIT_TIMER(jmp);
    unsigned long long sum_woibpb = 0;
    unsigned long long sum_ibpb = 0;
    int dump = 0;

    int btbf = open(COOL_DEVICE_PATH, 0);
    if(btbf == -1) {
        printf("Can't find btb flush device.\nPlease insmod cool.ko first.\n");
        exit(1);
    }

    // train btb
    for(i = 0; i < 1000; i++) {
        dump = fct(dump);
    }

    // in a loop do btb-based call/jmp
    // measure time of each invocation
    for (i = 0; i < ITERATIONS; i++) {
        START_TIMER(jmp);
        dump = fct(dump);
        END_TIMER(jmp);
        sum_woibpb += SPEND_TIME(jmp);
    }

    // in a loop btb-flush, then measure the call/jmp
    for (i = 0; i < ITERATIONS; i++) {
        ioctl(btbf, COOL_IOCTL_CMD_BTBF, 0);

        START_TIMER(jmp);
        dump = fct(dump);
        END_TIMER(jmp);
        sum_ibpb += SPEND_TIME(jmp);
    }

    printf("%d ITERATIONS \t sum \t\t avg\n", ITERATIONS);
    printf("wo IBPB \t\t %lld \t %f\n", sum_woibpb, ((float) sum_woibpb) / ITERATIONS);
    printf("with IBPB \t\t %lld \t %f\n", sum_ibpb, ((float) sum_ibpb) / ITERATIONS);

    close(btbf);

    return 0;
}
