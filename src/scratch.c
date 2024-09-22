#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>

jmp_buf env;


void foo(void) {
    puts("hello from foo");

    longjmp(env, 1);

    while (1) { puts("looping..."); }

}

int main(void) {
    int val = setjmp(env);

    if (val == 0)
        foo();

}
